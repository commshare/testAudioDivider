// InjectDll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include<assert.h>

BYTE NewCode[5]; //�����滻ԭ��ڴ�����ֽ� (jmp xxxx)

typedef int (_cdecl* getsub)(int x,int y);	//typedef �Ƚ��ٵ��÷� �ܶ�һ������ָ������ ��getsub 
getsub mySub = NULL; //���¶�������Ͷ���һ������
FARPROC pfar_sub;   //ָ��mySub������Զָ��

HANDLE hProcess = NULL; //�������̵ľ��
DWORD pid;    //��������ID
//end of ��������

//�޸�mySub��ڴ�����
void modify()
{ 
	assert(hProcess != NULL);

	DWORD dwTemp=0;
	DWORD dwOldProtect;
	VirtualProtectEx(hProcess, pfar_sub, 5, PAGE_READWRITE, &dwOldProtect); //���ڴ汣��ģʽ��Ϊ�ɶ�д,ԭ����ģʽ������dwOldProtect
	WriteProcessMemory(hProcess, pfar_sub, NewCode, 5, 0);
	VirtualProtectEx(hProcess, pfar_sub, 5, dwOldProtect, &dwTemp); //�ָ��ڴ汣��ģʽ
}

int new_sub(int x,int y)
{
	return 1;
}

int inject()
{
	//��ǰ����id
	DWORD dwPid = ::GetCurrentProcessId();
	//�򿪵�ǰ����
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid); 

	int addr_farpointer = 0;

	//��ȡMy_sub()������ַ
	//��ȡadd.dll�е�add()����
	//����һ��dll��
	/*
	һ��������ָ��DLL�ľ���������ڿ��Դ�DLL�����»�ú�����Ϊ���������������ʹ�ú���GetProcAddress()��
	����DLL�ľ���������ʹ��HINSTANCE���ͺ�����������Ϊ������������ú���ָ������GetProcAddress()���ص�ֵ��
	ͬʱ����轫GetProcAddress()ת��Ϊ�Ǹ���������ĺ���ָ�롣�ٸ����ӣ�����Add()����������轫GetProcAddress()ת��ΪAddFunc��
	�������֪������������ֵ��ԭ��
	���ڣ������ȷ������ָ���Ƿ����NULL�Լ�����ӵ��DLL�ĺ�������ֻ��һ���򵥵�if��䣻�������һ������NULL���������ǰ�����ͷſ⡣
	*/ //DLLģ����
	HMODULE hmod = ::LoadLibrary(_T("E:\\vs2010project\\My_Sub\\Debug\\My_Sub.dll"));
	//�ӿ����õ�����mySub�ĵ�ַ
	mySub = (getsub)::GetProcAddress(hmod, "mySub"); //������
	pfar_sub=(FARPROC)mySub;
	addr_farpointer = (int)pfar_sub;
		
	// //ָ��mySub������Զָ��
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
	//�޸�mySub��ڴ�����
	modify(); 
	MessageBox(NULL, TEXT("Modified SUCCESSFULLY!!"), TEXT("info"), MB_OK);
	return TRUE;
}

