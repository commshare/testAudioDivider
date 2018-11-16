// InjectDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include<assert.h>

BYTE NewCode[5]; //用来替换原入口代码的字节 (jmp xxxx)

typedef int (_cdecl* getsub)(int x,int y);	//typedef 比较少的用法 能定一个函数指针类型 即getsub 
getsub mySub = NULL; //用新定义的类型定义一个变量
FARPROC pfar_sub;   //指向mySub函数的远指针

HANDLE hProcess = NULL; //所处进程的句柄
DWORD pid;    //所处进程ID
//end of 变量定义

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
	//当前进程id
	DWORD dwPid = ::GetCurrentProcessId();
	//打开当前进程
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid); 

	int addr_farpointer = 0;

	//获取My_sub()函数地址
	//获取add.dll中的add()函数
	//加载一个dll库
	/*
	一旦你获得了指向DLL的句柄，你现在可以从DLL中重新获得函数。为了这样作，你必须使用函数GetProcAddress()，
	它将DLL的句柄（你可以使用HINSTANCE）和函数的名称作为参数。你可以让函数指针获得由GetProcAddress()返回的值，
	同时你必需将GetProcAddress()转换为那个函数定义的函数指针。举个例子，对于Add()函数，你必需将GetProcAddress()转换为AddFunc；
	这就是它知道参数及返回值的原因。
	现在，最好先确定函数指针是否等于NULL以及它们拥有DLL的函数。这只是一个简单的if语句；如果其中一个等于NULL，你必需如前所述释放库。
	*/ //DLL模块句柄
	HMODULE hmod = ::LoadLibrary(_T("E:\\vs2010project\\My_Sub\\Debug\\My_Sub.dll"));
	//从库里拿到函数mySub的地址
	mySub = (getsub)::GetProcAddress(hmod, "mySub"); //函数名
	pfar_sub=(FARPROC)mySub;
	addr_farpointer = (int)pfar_sub;
		
	// //指向mySub函数的远指针
	if (pfar_sub == NULL)
	{	
		MessageBox(NULL, TEXT("no locate add!!"), TEXT("info"), MB_OK);
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
	//修改mySub入口处代码
	modify(); 
	MessageBox(NULL, TEXT("Modified SUCCESSFULLY!!"), TEXT("info"), MB_OK);
	return TRUE;
}

