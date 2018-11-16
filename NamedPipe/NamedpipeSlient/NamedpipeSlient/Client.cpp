// NamedPipeClt.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include<Windows.h>
#include<iostream>

using namespace std;
/*
�ͻ��������ܵ�ʵ�ֵĲ��裺
1���ж��Ƿ��п����õ������ܵ�WaitNamedPipe
2���򿪹ܵ�CreateFile
3�����տͻ��˷�������ReadFile & ��ͻ��˷�������WriteFile
4���رչܵ�CloseHandle
*/
int main()
{
	cout << "���������ܵ����Գ���Ŀͻ���" << endl;
	char buf[256] = "";
	DWORD rLen = 0;
	DWORD wLen = 0;
	Sleep(1000);						//�ȴ��ܵ������ɹ���  
	/*
	1��ָ�������ܵ�������
2��ָ����ʱ�����NMPWAIT_WAIT_FOREVER��ʾһֱ�ȴ���ֱ��������һ�����õ������ܵ���ʵ��
	*/
	if (!WaitNamedPipe(TEXT("\\\\.\\Pipe\\pipeTest"), NMPWAIT_WAIT_FOREVER))
	{
		cout << "connect the namedPipe failed!" << endl;
		return 1;
	}

	/*
	CreateFile("\\\\.\\Pipe\\Test",GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
1��ָ�����ڴ�����򿪵Ķ��������
2��ָ������ķ��ʷ�ʽ��GENERIC_WRITE��ʾָ���������д����
3��ָ������ʽ������˲���Ϊ0����ʾ�����ܱ�����
4��ָ��SECURITY_ATTRIBUTES�ṹ��ָ�룬�ýṹָ���������ܵ��İ�ȫ�����������û�����������Ĭ��ֵΪNULL
5��ָ����δ����ļ���OPEN_EXISTING��ʾ���ļ�������ļ������ڣ���������ʧ�ܣ�
6�������ļ����Ժͱ�־��FILE_ATTRIBUTE_NORMAL��ʾ���ļ�û�������������ã�
7��ָ������GENERIC_READ���ʷ�ʽ��ģ���ļ��ľ��
--------------------- 
	*/
	HANDLE hPipe = CreateFile(          //�����ܵ��ļ��������ӹܵ�  
		TEXT("\\\\.\\Pipe\\pipeTest"),	//�ܵ�����  
		GENERIC_READ | GENERIC_WRITE,   //�ļ�ģʽ  
		0,                              //�Ƿ���  
		NULL,                           //ָ��һ��SECURITY_ATTRIBUTES�ṹ��ָ��  
		OPEN_EXISTING,                  //��������  
		FILE_ATTRIBUTE_NORMAL,          //�ļ����ԣ�NORMALΪĬ������  
		NULL);                          //ģ�崴���ļ��ľ��  

	if (INVALID_HANDLE_VALUE == hPipe)
	{
		cout << "��ͨ��ʧ��!" << endl;
		return 2;
	}
	char strMessage[] = "�����ܵ����Գ���";

	if (!WriteFile(hPipe, strMessage, sizeof(strMessage), &wLen, 0)) //��ܵ���������  
	{
		cout << "��ͨ��д����ʧ��!" << endl;
		return 3;
	}
	if (!ReadFile(hPipe, buf, 256, &rLen, NULL))					//��ȡ�ܵ�����
	{
		cout << "��ͨ��������ʧ��!" << endl;
		return 4;
	}
	else
		cout << "�ӷ������˽������� �� " << buf << endl << "���ݳ���Ϊ��" << rLen << endl;

	Sleep(1000);   //ִ�й���һ��ʱ��
	CloseHandle(hPipe);					//�رչܵ�  
	system("pause");
	return 0;
}

