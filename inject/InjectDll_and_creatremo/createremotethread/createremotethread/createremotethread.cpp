// createremotethread.cpp : �������̨Ӧ�ó������ڵ㡣
//	

// CreateRemoteThread�������ˣ� ������Calc.exe ע����һ���̣߳�Ȼ������߳�ִ����LoadLibrary ������
	//�Ҹ���Calc ҪLoad ��ָ���� DLL

#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>


//�����ֽ�char*ת��Ϊ���ֽ�wchar_t*  
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

//�����ֽ�wchar_t*ת��Ϊ���ֽ�char*  
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
//CreateRemoteThreadԶ���߳�ע��Dll��Hook ��ִ��OPENPROCESS֮ǰ��Ҫ������Ȩ
void EnableDebugPriv()
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkp;
	//ִ��OPENPROCESS֮ǰ��Ҫ������Ȩ
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	//�޸Ľ���Ȩ��
    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = luid;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	//֪ͨϵͳ�޸Ľ���Ȩ��
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
		// //��Զ���߳�
		//����openprocess ��Ŀ����� ���н�һ���Ĳ���������CreateRemoteThread���ﵽʹĿ����̼�������ָ��DLL������ָ��DLL������ʱ��ִ��ָ�����롣
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid/*dwRemoteProcessId*/);		
		return hProcess;
	}

	return NULL;
	
}



//createremotethread.exes ��ע�����
int _tmain(int argc, _TCHAR* argv[])
{
	

	HANDLE hThread;
	//��ָ�������DLL  //ָ��ע��֮��Ŀ�����Ҫ���ص�DLL
	char szLibPath[_MAX_PATH] = "E:\\vs2010project\\InjectDll\\Release\\InjectDll.dll";

	DWORD hLibModule;

	HANDLE hProcess = NULL;
	//���ݽ������ֻ�ȡ���̾��Ҳ�Ǳ����ӷǳ��ؼ��Ĳ��֣�����ͨ��΢���ṩ��api ��������б���գ� Ȼ���ٱ���Ѱ��
	hProcess = GetProcessByName("Calc.exe");
	DWORD ERRO = GetLastError();

	if(hProcess == NULL)
		return 0;
	//LoadLibrary(_T("remote_hook.dll"));
	//ERRO = GetLastError();
	//LPCWSTR mod = AnsiToUnicode("heihei");

	// //��Ϊkernel32 ÿһ��windows������̿ռ��ж��� ������������LOADLIBRARY��������
	HMODULE modHandle = GetModuleHandle(_T("Kernel32"));
	ERRO = GetLastError();

	//��Զ�̽��̵��ڴ��ַ�ռ���佫Ҫע����Լ���DLL���ļ���������
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
///����������һλͬѧ��https://www.cnblogs.com/Reyzal/p/5482611.html
#if 0

DWORD dwOffect, dwArgu;

BOOL CreateRemoteDll(const char *DllFullPath, const DWORD dwRemoteProcessId, DWORD dwOffect, DWORD dwArgu)
{
	HANDLE hToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tkp;

		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);//�޸Ľ���Ȩ��
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL);//֪ͨϵͳ�޸Ľ���Ȩ��
		CloseHandle(hToken);
	}

	HANDLE hRemoteProcess;

	//��Զ���߳�
	if ((hRemoteProcess = OpenProcess(PROCESS_CREATE_THREAD |    //����Զ�̴����߳�
		PROCESS_VM_OPERATION |                //����Զ��VM����
		PROCESS_VM_WRITE,                    //����Զ��VMд
		FALSE, dwRemoteProcessId)) == NULL)
	{
		return FALSE;
	}

	char *pszLibFileRemote;
	//��Զ�̽��̵��ڴ��ַ�ռ����DLL�ļ���������
	pszLibFileRemote = (char *)VirtualAllocEx(hRemoteProcess, NULL, lstrlen(DllFullPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);
	if (pszLibFileRemote == NULL)
	{
		CloseHandle(hRemoteProcess);
		return FALSE;
	}

	//��DLL��·�������Ƶ�Զ�̽��̵��ڴ�ռ�
	if (WriteProcessMemory(hRemoteProcess,
		pszLibFileRemote, (void *)DllFullPath, lstrlen(DllFullPath) + 1, NULL) == 0)
	{
		CloseHandle(hRemoteProcess);
		return FALSE;
	}

	//����LoadLibraryA����ڵ�ַ
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
	// ��HMODULE LoadLibraryW(LPCWSTR)��: �޷������� 1 �ӡ�char [260]��ת��Ϊ��LPCWSTR��
	HMODULE hTmpDll = LoadLibrary(curpath);
	dwOffect = (DWORD)GetProcAddress(hTmpDll, "HookFun");
	dwOffect -= (DWORD)hTmpDll;
	FreeLibrary(hTmpDll);
	CreateRemoteDll(curpath, dwPid, dwOffect, dwArgu);
}

#endif