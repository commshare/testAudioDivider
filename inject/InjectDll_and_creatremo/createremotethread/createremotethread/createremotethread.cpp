// createremotethread.cpp : 定义控制台应用程序的入口点。
//	

// CreateRemoteThread的作用了， 我们向Calc.exe 注入了一个线程，然后这个线程执行了LoadLibrary 函数，
	//我告诉Calc 要Load 我指定的 DLL

#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>


//将单字节char*转化为宽字节wchar_t*  
wchar_t* AnsiToUnicode( const char* szStr )  
{  
    int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0 );  
    if (nLen == 0)  
    {  
        return NULL;  
    }  
    wchar_t* pResult = new wchar_t[nLen];  
    MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen );  
    return pResult;  
} 

//将宽字节wchar_t*转化为单字节char*  
inline char* UnicodeToAnsi( const wchar_t* szStr )  
{  
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );  
    if (nLen == 0)  
    {  
        return NULL;  
    }  
    char* pResult = new char[nLen];  
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );  
    return pResult;  
}  
//CreateRemoteThread远程线程注入Dll与Hook ，执行OPENPROCESS之前需要开启特权
void EnableDebugPriv()
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;
	//执行OPENPROCESS之前需要开启特权
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	//修改进程权限
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//通知系统修改进程权限
    AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

    CloseHandle(hToken); 
}

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
		// //打开远程线程
		//调用openprocess 打开目标进程 进行进一步的操作（调用CreateRemoteThread）达到使目标进程加载我们指定DLL，进而指定DLL被加载时能执行指定代码。
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid/*dwRemoteProcessId*/);		
		return hProcess;
	}

	return NULL;
	
}



//createremotethread.exes 是注入程序
int _tmain(int argc, _TCHAR* argv[])
{
	

	HANDLE hThread;
	//我指定的这个DLL  //指定注入之后目标进程要加载的DLL
	char szLibPath[_MAX_PATH] = "E:\\vs2010project\\InjectDll\\Release\\InjectDll.dll";

	DWORD hLibModule;

	HANDLE hProcess = NULL;
	//根据进程名字获取进程句柄也是本例子非常关键的部分，可以通过微软提供的api 拍摄进程列表快照， 然后再遍历寻找
	hProcess = GetProcessByName("Calc.exe");
	DWORD ERRO = GetLastError();

	if(hProcess == NULL)
		return 0;
	//LoadLibrary(_T("remote_hook.dll"));
	//ERRO = GetLastError();
	//LPCWSTR mod = AnsiToUnicode("heihei");

	// //因为kernel32 每一个windows程序进程空间中都有 所以让他调用LOADLIBRARY不成问题
	HMODULE modHandle = GetModuleHandle(_T("Kernel32"));
	ERRO = GetLastError();

	//在远程进程的内存地址空间分配将要注入的自己的DLL的文件名缓冲区
	void* pLibRemote = VirtualAllocEx(hProcess, NULL, sizeof(szLibPath), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//
	LPTHREAD_START_ROUTINE addr = (LPTHREAD_START_ROUTINE)GetProcAddress(modHandle, "LoadLibraryA");
	ERRO = GetLastError();
	WriteProcessMemory( hProcess, pLibRemote, (void*)szLibPath, sizeof(szLibPath), NULL);
	ERRO = GetLastError();

	hThread = CreateRemoteThread(hProcess, NULL, 0,
								addr,
								pLibRemote,
								0,
								NULL);
	ERRO = GetLastError();
	WaitForSingleObject(hThread, INFINITE);
	ERRO = GetLastError();
	CloseHandle(hThread);
	//VirtualFreeEx(hProcess, pLibRemote, sizeof(szLibPath), MEM_RELEASE);


	return 0;
}

////////////////////////////////////////////////////////
///下面是另外一位同学的https://www.cnblogs.com/Reyzal/p/5482611.html
#if 0

DWORD dwOffect, dwArgu;

BOOL CreateRemoteDll(const char *DllFullPath, const DWORD dwRemoteProcessId, DWORD dwOffect, DWORD dwArgu)
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tkp;

		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);//修改进程权限
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);//通知系统修改进程权限
		CloseHandle(hToken);
	}

	HANDLE hRemoteProcess;

	//打开远程线程
	if ((hRemoteProcess = OpenProcess(PROCESS_CREATE_THREAD |    //允许远程创建线程
		PROCESS_VM_OPERATION |                //允许远程VM操作
		PROCESS_VM_WRITE,                    //允许远程VM写
		FALSE, dwRemoteProcessId)) == NULL)
	{
		return FALSE;
	}

	char *pszLibFileRemote;
	//在远程进程的内存地址空间分配DLL文件名缓冲区
	pszLibFileRemote = (char *)VirtualAllocEx(hRemoteProcess, NULL, lstrlen(DllFullPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);
	if (pszLibFileRemote == NULL)
	{
		CloseHandle(hRemoteProcess);
		return FALSE;
	}

	//将DLL的路径名复制到远程进程的内存空间
	if (WriteProcessMemory(hRemoteProcess,
		pszLibFileRemote, (void *)DllFullPath, lstrlen(DllFullPath) + 1, NULL) == 0)
	{
		CloseHandle(hRemoteProcess);
		return FALSE;
	}

	//计算LoadLibraryA的入口地址
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");

	if (pfnStartAddr == NULL)
	{
		return FALSE;
	}

	HANDLE hRemoteThread;
	hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0,
		pfnStartAddr, pszLibFileRemote, 0, NULL);
	WaitForSingleObject(hRemoteThread, INFINITE);
	if (hRemoteThread == NULL)
	{

		CloseHandle(hRemoteProcess);
		return FALSE;
	}
	DWORD dwDllAddr;
	GetExitCodeThread(hRemoteThread, &dwDllAddr);
	if (dwDllAddr != 0)
	{
		dwDllAddr += dwOffect;
		HANDLE hHookFunc;
		hHookFunc = CreateRemoteThread(hRemoteProcess, NULL, 0,
			(PTHREAD_START_ROUTINE)dwDllAddr, (LPVOID)dwArgu, 0, NULL);
		WaitForSingleObject(hHookFunc, INFINITE);
		if (hHookFunc == NULL)
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
	GetModuleFileName(NULL, curpath, 260);
	*strrchr(curpath, '\\') = '\0';
	strcat(curpath, "\\this.dll");
	// “HMODULE LoadLibraryW(LPCWSTR)”: 无法将参数 1 从“char [260]”转换为“LPCWSTR”
	HMODULE hTmpDll = LoadLibrary(curpath);
	dwOffect = (DWORD)GetProcAddress(hTmpDll, "HookFun");
	dwOffect -= (DWORD)hTmpDll;
	FreeLibrary(hTmpDll);
	CreateRemoteDll(curpath, dwPid, dwOffect, dwArgu);
}

#endif