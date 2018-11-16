#include<iostream>
#include<Windows.h>
using namespace std;
/*
�������������ܵ�ʵ�ֵĲ��裺
1�����������ܵ�CreateNamedPipe
2���ȴ��ͻ�������ConnectNamedPipe
3�����տͻ��˷�������ReadFile & ��ͻ��˷�������WriteFile
4���رչܵ�CloseHandle
*/
int main()
{
	char buf[256] = "";
	DWORD rLen = 0;
	DWORD wLen = 0;
	HANDLE hPipe = NULL;
	/*
	//���������ܵ��ĺ�����ʹ��
CreateNamedPipe("\\\\.\\Pipe\\Test",PIPE_ACCESS_DUPLEX,PIPE_NOWAIT,10,1024,1024,100,NULL)
1��Ϊ�����Ĺܵ�����
2��ָ���ܵ��ķ��ʷ�ʽ���ص���ʽ��дֱͨ��ʽ�Լ��ܵ�����İ�ȫ���ʷ�ʽ��PIPE_ACCESS_DUPLEX����ָ˫��ģʽ��
3��ָ���ܵ���������͡���ȡ�͵ȴ���ʽ��PIPE_NOWAITָ�����������ʽ��
4��ָ���ܵ��ܹ�������ʵ���������Ŀ
5��ָ��Ϊ������������������ֽ���
6��ָ��Ϊ���뻺�������������ֽ���
7��ָ��Ĭ�ϳ�ʱʱ�䣬��λms��ͬһ�ܵ��Ĳ�ͬʵ��ָ��ֵ��Ҫ��ͬ
8��ָ��SECURITY_ATTRIBUTES�ṹ��ָ�룬�ýṹָ���������ܵ��İ�ȫ������

ԭ�ģ�https://blog.csdn.net/qq_15029743/article/details/79508568 
	*/
	hPipe = CreateNamedPipe(
		TEXT("\\\\.\\Pipe\\pipeTest"),							//�ܵ���  
		PIPE_ACCESS_DUPLEX,										//�ܵ����ͣ�˫��ͨ��  
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,  //�ܵ�����  
		PIPE_UNLIMITED_INSTANCES,								//�ܵ��ܴ��������ʵ������  
		0,														//������������� 0��ʾĬ��  
		0,														//���뻺�������� 0��ʾĬ��  
		NMPWAIT_WAIT_FOREVER,									//��ʱʱ��,NMPWAIT_WAIT_FOREVERΪ����ʱ�ȴ�
		NULL);													//ָ��һ��SECURITY_ATTRIBUTES�ṹ,���ߴ�����ֵ.
	if (INVALID_HANDLE_VALUE == hPipe)
		cout << "�����ܵ�ʧ��: " << GetLastError() << endl;
	else
	{
		cout << "���������ܵ����Գ����еķ�������" << endl;
		cout << "���ڵȴ��ͻ�������..." << endl;
		////�������ȴ��ͻ��˵���������ĵ������������ӷ������˵������ܵ�����
		/*
1��ָ��һ�������ܵ�ʵ���ķ������ľ�����þ����CreateNamedPipe��������
2��ָ��OVERLAPPED�ṹ���ָ�룬Ĭ��ΪNULL������ʹ��Ĭ�ϵ�ͬ��IO��ʽ1
		*/
		if (!ConnectNamedPipe(hPipe, NULL))						//�����ȴ��ͻ������ӡ�  
		{
			cout << "����ʧ��!" << endl;
			return 1;
		}
		else
			cout << "���ӳɹ�!" << endl;
		/*
		���տͻ��˷�������ReadFile & ��ͻ��˷�������WriteFile��


//�ļ���д��
WriteFile(hPipe, strMessage, sizeof(strMessage), &wLen, 0)
1��ָ��Ҫд�����ݵ��ļ��ľ��
2��ָ�������Ҫ��Ҫд���ļ������ݵĻ�������ָ��
3��ָ��Ҫ���ļ���д����ֽ���
4����������ʵ��д�뵽�ļ��е��ֽ���
5��ָ��OVERLAPPED�ṹ���ָ�룬Ĭ��ΪNULL������ʹ��Ĭ�ϵ�ͬ��IO��ʽ

//�ļ��Ķ�ȡ
ReadFile(hPipe, buf, 256, &rLen, NULL)
1��ָ��Ҫ��ȡ���ݵ��ļ��ľ��
2��ָ�������Ҫ��Ҫ���յ��ļ��ж�ȡ���ݵĻ�������ָ��
3��ָ��Ҫ���ļ��ж�ȡ���ֽ���
4����������ʵ�ʶ�ȡ�����ֽ���
5��ָ��OVERLAPPED�ṹ���ָ�룬Ĭ��ΪNULL������ʹ��Ĭ�ϵ�ͬ��IO��ʽ
---------------------
���ߣ�����gold
��Դ��CSDN
ԭ�ģ�https://blog.csdn.net/qq_15029743/article/details/79508568
��Ȩ����������Ϊ����ԭ�����£�ת���븽�ϲ������ӣ�
		*/
		if (!ReadFile(hPipe, buf, 256, &rLen, NULL))			//���ܿͻ��˷�������
		{
			cout << "�ӿͻ��˽��ղ���ȡ����!" << endl;
			return 2;
		}
		else
			cout << "�ͻ��˽��յ�����Ϊ �� " << buf << endl << "���ݳ���Ϊ " << rLen << endl;

		char strMessage[] = "�����ܵ����Գ���";
		WriteFile(hPipe, strMessage, sizeof(strMessage), &wLen, 0); //��ͻ��˷�������
		CloseHandle(hPipe);											//�رչܵ���� 
	}
	system("pause");
	return 0;
}