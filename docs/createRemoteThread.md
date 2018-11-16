使用CreateRemoteThread把代码远程注入指定exe执行
由于本人也是新手，如果有朋友不懂windows api相关知识，我相信查阅书籍或者百度会比我说有帮助的多，下面就我所做简单复述一下过程，欢迎指正缺点。
https://www.cnblogs.com/inva/p/4971331.html
效果图示如下：

做的这个例子首先是创建了一个MFC 对话框程序，然后自己创建了一个 带有导出函数 （简单的减法） 的DLL ， 这个Calc按键就是调用自己DLL里面的减法函数 计算1 - 1，
其次创建一个拥有能够修改进程中某函数的入口代码功能的DLL（为什么是DLL，网上有大量详细的资料，），
然后我们还创建一个windows控制台程序 也就是 调用CreateRemoteThread  让Calc 加载我们准备好的DLL（修改函数入口代码），
 因为DLL被加载时我们可以在DLLMain中指定要执行的代码。

未注入代码之前:





执行我的 注入程序createremotethread.exe 之后 如下图:





可以看到注入成功了， 注入的是什么呢？ 这就是 CreateRemoteThread的作用了， 我们向Calc.exe 注入了一个线程，然后这个线程执行了LoadLibrary 函数，

我告诉Calc 要Load 我指定的 DLL， 为什么呢？ 因为 我指定的这个DLL 是我们自己创建的， 它里面的函数代码能找到Calc进程中 原来sub函数的地址，然后并

修改sub函数地址处的入口代码，也就是改成 jmp xxx 使Calc 执行我们指定的代码。



最重要的CreateRemoteThread：

复制代码
 　　HANDLE hThread;
    char szLibPath[_MAX_PATH] = "E:\\vs2010project\\InjectDll\\Release\\InjectDll.dll"; //指定注入之后目标进程要加载的DLL

    DWORD hLibModule;

    HANDLE hProcess = NULL;
    hProcess = GetProcessByName("Calc.exe");
    DWORD ERRO = GetLastError();

    if(hProcess == NULL)
        return 0;

    HMODULE modHandle = GetModuleHandle(_T("Kernel32")); //因为kernel32 每一个windows程序进程空间中都有 所以让他调用LOADLIBRARY不成问题

　　void* pLibRemote = VirtualAllocEx(hProcess, NULL, sizeof(szLibPath), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    LPTHREAD_START_ROUTINE addr = (LPTHREAD_START_ROUTINE)GetProcAddress(modHandle, "LoadLibraryA");
    WriteProcessMemory( hProcess, pLibRemote, (void*)szLibPath, sizeof(szLibPath), NULL);

    hThread = CreateRemoteThread(hProcess, NULL, 0,
                                addr,
                                pLibRemote,
                                0,
                                NULL);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
复制代码




## 根据进程名字获取进程句柄也是本例子非常关键的部分，可以通过微软提供的api 拍摄进程列表快照， 然后再遍历寻找，代码如下：

复制代码
HANDLE GetProcessByName(const char* name)
{
    EnableDebugPriv();
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);
    if (Process32First(snapshot, &process))
    {
        do
        {
            if( stricmp(UnicodeToAnsi(process.szExeFile), name) == 0)
            {
                pid = process.th32ProcessID;
                break;
            }
        }while(Process32Next(snapshot, &process));
    }
    CloseHandle(snapshot);

    if(pid != 0)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        return hProcess;
    }

    return NULL;

}
复制代码




其中 关于 CreateRemoteThread 部分的核心代码有许多需要注意的事情 主要有 执行OPENPROCESS之前需要开启特权 代码如下：

复制代码
 1 void EnableDebugPriv()
 2 {
 3     HANDLE hToken;
 4     LUID luid;
 5     TOKEN_PRIVILEGES tkp;
 6
 7     OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
 8
 9     LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);
10
11     tkp.PrivilegeCount = 1;
12     tkp.Privileges[0].Luid = luid;
13     tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
14
15     AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);
16
17     CloseHandle(hToken);
18 }
复制代码
接下来我们就可以调用openprocess 打开目标进程 进行进一步的操作（调用CreateRemoteThread）达到使目标进程加载我们指定DLL，进而指定DLL被加载时能执行指定代码。



最后 关于修改进程中 导入 dll 中的函数的入口处代码 的代码如下：

复制代码
复制代码
BYTE NewCode[5]; //用来替换原入口代码的字节 (jmp xxxx)

typedef int (_cdecl* getsub)(int x,int y);    //typedef 比较少的用法 能定一个函数指针类型 即getsub
getsub mySub = NULL; //用新定义的类型定义一个变量
FARPROC pfar_sub;   //指向mySub函数的远指针

HANDLE hProcess = NULL; //所处进程的句柄
DWORD pid;    //所处进程ID
复制代码


//修改mySub入口处代码
void modify()
{
    assert(hProcess != NULL);

    DWORD dwTemp=0;
    DWORD dwOldProtect;

    VirtualProtectEx(hProcess, pfar_sub, 5, PAGE_READWRITE, &dwOldProtect); //将内存保护模式改为可读写,原保护模式保存入dwOldProtect
    WriteProcessMemory(hProcess, pfar_sub, NewCode, 5, 0);
    VirtualProtectEx(hProcess, pfar_sub, 5, dwOldProtect, &dwTemp); //恢复内存保护模式
}

int new_sub(int x,int y)
{
    return 1;
}

int inject()
{
    DWORD dwPid = ::GetCurrentProcessId();
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid);

    int addr_farpointer = 0;


    //获取My_sub()函数地址
    HMODULE hmod = ::LoadLibrary(_T("E:\\vs2010project\\My_Sub\\Debug\\My_Sub.dll"));
    mySub = (getsub)::GetProcAddress(hmod, "mySub");
    pfar_sub=(FARPROC)mySub;
    addr_farpointer = (int)pfar_sub;

    if (pfar_sub == NULL)
    {
        MessageBox(NULL, TEXT("locate mySub failed!!"), TEXT("info"), MB_OK);
        return FALSE;
    }

    NewCode[0] = 0xe9;//0xe9 == jmp

    _asm
    {
        lea eax, new_sub
        mov ebx, pfar_sub
        sub eax, ebx
        sub eax, 5
        mov dword ptr [NewCode + 1],eax
    }

    modify();
    MessageBox(NULL, TEXT("Modified SUCCESSFULLY!!"), TEXT("info"), MB_OK);
    return TRUE;
}
复制代码




整体流程清楚了实践应该没啥问题（还是需要折腾的， 折腾也是进步的过程）。

附上主要工程文件链接 http://pan.baidu.com/s/1dD4WhZN








# https://www.cnblogs.com/Reyzal/p/5482611.html

CreateRemoteThread远程线程注入Dll与Hook
CreateRemoteThread虽然很容易被检测到，但是在有些场合还是挺有用的。每次想用的时候总想着去找以前的代码，现在在这里记录一下。

CreateRemoteThread远程注入
复制代码
DWORD dwOffect,dwArgu;

BOOL CreateRemoteDll(const char *DllFullPath, const DWORD dwRemoteProcessId ,DWORD dwOffect,DWORD dwArgu)
{
    HANDLE hToken;
    if ( OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken) )
    {
        TOKEN_PRIVILEGES tkp;

        LookupPrivilegeValue( NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid );//修改进程权限
        tkp.PrivilegeCount=1;
        tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges( hToken,FALSE,&tkp,sizeof tkp,NULL,NULL );//通知系统修改进程权限
        CloseHandle(hToken);
    }

    HANDLE hRemoteProcess;

    //打开远程线程
    if( (hRemoteProcess = OpenProcess( PROCESS_CREATE_THREAD |    //允许远程创建线程
        PROCESS_VM_OPERATION |                //允许远程VM操作
        PROCESS_VM_WRITE,                    //允许远程VM写
        FALSE, dwRemoteProcessId ) )== NULL )
    {
        return FALSE;
    }

    char *pszLibFileRemote;
    //在远程进程的内存地址空间分配DLL文件名缓冲区
    pszLibFileRemote = (char *) VirtualAllocEx( hRemoteProcess, NULL, lstrlen(DllFullPath)+1,
        MEM_COMMIT, PAGE_READWRITE);
    if(pszLibFileRemote == NULL)
    {
        CloseHandle(hRemoteProcess);
        return FALSE;
    }

    //将DLL的路径名复制到远程进程的内存空间
    if( WriteProcessMemory(hRemoteProcess,
        pszLibFileRemote, (void *) DllFullPath, lstrlen(DllFullPath)+1, NULL) == 0)
    {
        CloseHandle(hRemoteProcess);
        return FALSE;
    }

    //计算LoadLibraryA的入口地址
    PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)
        GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");

    if(pfnStartAddr == NULL)
    {
        return FALSE;
    }

    HANDLE hRemoteThread;
    hRemoteThread = CreateRemoteThread( hRemoteProcess, NULL, 0,
        pfnStartAddr, pszLibFileRemote, 0, NULL);
    WaitForSingleObject(hRemoteThread,INFINITE);
    if( hRemoteThread == NULL)
    {

        CloseHandle(hRemoteProcess);
        return FALSE;
    }
    DWORD dwDllAddr;
    GetExitCodeThread(hRemoteThread,&dwDllAddr);
    if(dwDllAddr!=0)
    {
        dwDllAddr += dwOffect;
        HANDLE hHookFunc;
        hHookFunc = CreateRemoteThread( hRemoteProcess, NULL, 0,
            (PTHREAD_START_ROUTINE)dwDllAddr, (LPVOID)dwArgu, 0, NULL);
        WaitForSingleObject(hHookFunc,INFINITE);
        if( hHookFunc == NULL)
        {
            CloseHandle(hRemoteThread);
            CloseHandle(hRemoteProcess);
            return FALSE;
        }
        CloseHandle(hHookFunc);

    }
    else
    {
        CloseHandle(hRemoteProcess);
        CloseHandle(hRemoteThread);
        return FALSE;

    }
    CloseHandle(hRemoteProcess);
    CloseHandle(hRemoteThread);
    return TRUE;
}

void Hook(int dwPid)
{
    char curpath[260];
    GetModuleFileName(NULL,curpath,260);
    *strrchr(curpath,'\\') = '\0';
    strcat(curpath,"\\this.dll");
    HMODULE hTmpDll = LoadLibrary(curpath);
    dwOffect = (DWORD)GetProcAddress(hTmpDll,"HookFun");
    dwOffect -= (DWORD)hTmpDll;
    FreeLibrary(hTmpDll);
    CreateRemoteDll(curpath,dwPid,dwOffect,dwArgu);
}
复制代码
Hook代码

复制代码
__declspec(naked) void MyHookGetRes()
{
    __asm
    {
        pushad
        pushfd
    }
    MyFun();
    __asm
    {
        popfd
        popad
        add esp,0xc
        jmp uRetAddr
    }
}

ULONG uHookAddr = 0x11111  + (DWORD)hModule;
HANDLE handle = GetCurrentProcess();
char MyJMP[5]={0};
MyJMP[0]=(char)0xe9;
ULONG uTempAddr=(ULONG)MyJMP;
uRetAddr = uHookAddr + 5;
ULONG uSkillJmp=(ULONG)MyHookGetRes-uHookAddr-5;
__asm
{
    mov eax,uSkillJmp
        mov ebx, uTempAddr
        add ebx ,1
        mov [ebx],eax
        mov ecx,[ebx]
}
WriteProcessMemory(handle,(LPVOID)(uHookAddr),(LPVOID)MyJMP,5,NULL);
复制代码