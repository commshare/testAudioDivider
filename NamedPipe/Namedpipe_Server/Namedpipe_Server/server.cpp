#include<iostream>
#include<Windows.h>

LPCWSTR pipeName = L"\\\\.\\pipe\\SoundInjectControllermp3";
#define LINE1 "{0.0.0.00000000}.{c6132bf1-8f49-46d2-ab60-d86e8bb95050}"
#define AOC "{0.0.0.00000000}.{44486876-6dc6-4247-bddd-b6a3e13007c1}"
#pragma pack(push, 1)  //#pragma pack(1) 使结构体按1字节方式对齐
struct ClientMessage
{
	int pid;
	int action;
	char data[200];
	int dataLen;
};
#pragma pack(pop)
using namespace std;
/*
服务器端命名管道实现的步骤：
1、创建命名管道CreateNamedPipe
2、等待客户端连接ConnectNamedPipe
3、接收客户端发送数据ReadFile & 向客户端发送数据WriteFile
4、关闭管道CloseHandle
*/
int main()
{
	char buf[256] = "";
	DWORD rLen = 0;
	DWORD wLen = 0;
	HANDLE hPipe = NULL;
	/*
	//创建命名管道的函数的使用
CreateNamedPipe("\\\\.\\Pipe\\Test",PIPE_ACCESS_DUPLEX,PIPE_NOWAIT,10,1024,1024,100,NULL)
1、为创建的管道命名
2、指定管道的访问方式、重叠方式、写直通方式以及管道句柄的安全访问方式（PIPE_ACCESS_DUPLEX这里指双向模式）
3、指定管道句柄的类型、读取和等待方式（PIPE_NOWAIT指允许非阻塞方式）
4、指定管道能够创建的实例的最大数目
5、指定为输出缓冲区所保留的字节数
6、指定为输入缓冲区所保留的字节数
7、指定默认超时时间，单位ms，同一管道的不同实例指定值需要相同
8、指向SECURITY_ATTRIBUTES结构的指针，该结构指定了命名管道的安全描述符

原文：https://blog.csdn.net/qq_15029743/article/details/79508568
	*/
	hPipe = CreateNamedPipe(
		/*TEXT("\\\\.\\Pipe\\pipeTest"),*/pipeName,							//管道名
		PIPE_ACCESS_DUPLEX,										//管道类型，双向通信
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,  //管道参数
		PIPE_UNLIMITED_INSTANCES,								//管道能创建的最大实例数量
		0,														//输出缓冲区长度 0表示默认
		0,														//输入缓冲区长度 0表示默认
		NMPWAIT_WAIT_FOREVER,									//超时时间,NMPWAIT_WAIT_FOREVER为不限时等待
		NULL);													//指定一个SECURITY_ATTRIBUTES结构,或者传递零值.
	if (INVALID_HANDLE_VALUE == hPipe)
		cout << "CreateNamedPipe fail: " << GetLastError() << endl;
	else
	{
		cout << "server is running...:" << endl;
		cout << "wait for clients.." << endl;
		////服务器等待客户端的连接请求的到来（并非连接服务器端的命名管道！）
		/*
1、指向一个命名管道实例的服务器的句柄，该句柄由CreateNamedPipe函数返回
2、指向OVERLAPPED结构体的指针，默认为NULL，表明使用默认的同步IO方式1
		*/
		printf("Server is now running \n");

		if (!ConnectNamedPipe(hPipe, NULL))						//阻塞等待客户端连接。
		{
			cout << "connect fail!" << endl;
			printf("ConnectNamePipe failed with error %x \n", GetLastError());
			CloseHandle(hPipe);
			return 1;
		}
		else
			cout << "connect with client ok!" << endl;
		/*
		接收客户端发送数据ReadFile & 向客户端发送数据WriteFile：


//文件的写入
WriteFile(hPipe, strMessage, sizeof(strMessage), &wLen, 0)
1、指定要写入数据的文件的句柄
2、指向包含将要将要写入文件的数据的缓冲区的指针
3、指明要向文件中写入的字节数
4、用来接收实际写入到文件中的字节数
5、指向OVERLAPPED结构体的指针，默认为NULL，表明使用默认的同步IO方式

//文件的读取
ReadFile(hPipe, buf, 256, &rLen, NULL)
1、指定要读取数据的文件的句柄
2、指向包含将要将要接收的文件中读取数据的缓冲区的指针
3、指明要向文件中读取的字节数
4、用来接收实际读取到的字节数
5、指向OVERLAPPED结构体的指针，默认为NULL，表明使用默认的同步IO方式
---------------------

http://www.itboth.com/d/VjyAvi
同步情况下，ReadFile是阻塞读，遇到下列一个情况返回：

（1）A write operation completes on the write end of the pipe.
（2）The number of bytes requested is read.
（3）An error occurs.
		*/
#if 0
		if (!ReadFile(hPipe, buf, 256, &rLen, NULL))			//接受客户端发送数据
		{
			cout << "read from client!" << endl;
			printf("ReadFile failed with error %x \n", GetLastError());
			CloseHandle(hPipe);
			return 2;
		}
		else {
			cout << "client data ： " << buf << endl << "len: " << rLen << endl;
		}
#endif
		cout << "send to client ..."<< endl;
		//int pid = GetCurrentProcessId();
		//int action = 1;
		ClientMessage message;// = ClientMessage(pid, action, messageStr);
		memset(&message,0,sizeof(ClientMessage));
		message.pid = GetCurrentProcessId();
		message.action = 1;
		std::string str = std::string(AOC);
		cout << "send to client pid:" << message.pid <<"strsize: "<< str.size() <<" str:" << str.c_str()<< endl;
#if 0
		std::copy(str.begin(), str.end(), message.data);
		message.data[str.size()] = '\0';
#else
		strcpy_s(message.data,str.c_str());
		printf("message data : %s \n",message.data);
#endif
		message.dataLen = str.size();
		//char strMessage[] = "命名管道测试程序";
		//refer to https://github.com/marianlupascu/Bitdefender-Tech-Challenge/blob/dd7751578c035cb62a49cbc5c86cfda140228fb7/Curs%206-7/Pipeline/P1/P1.cpp
		WriteFile(hPipe, &message, sizeof(message), &wLen, 0); //向客户端发送数据

		printf("AFTER MESSAGE SEND\n");

		//CloseHandle(hPipe);											//关闭管道句柄
	}
	system("pause");
	cout << "DisconnectNamedPipe..." << endl;
	if (DisconnectNamedPipe(hPipe) == 0)
	{
		printf("DisconnectNamedPipe failed with error %x \n", GetLastError());
		return 0;
	}
	cout << "CloseHandle..." << endl;
	CloseHandle(hPipe);
	return 0;
}