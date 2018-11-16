#include<iostream>
#include<Windows.h>
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
		TEXT("\\\\.\\Pipe\\pipeTest"),							//管道名  
		PIPE_ACCESS_DUPLEX,										//管道类型，双向通信  
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,  //管道参数  
		PIPE_UNLIMITED_INSTANCES,								//管道能创建的最大实例数量  
		0,														//输出缓冲区长度 0表示默认  
		0,														//输入缓冲区长度 0表示默认  
		NMPWAIT_WAIT_FOREVER,									//超时时间,NMPWAIT_WAIT_FOREVER为不限时等待
		NULL);													//指定一个SECURITY_ATTRIBUTES结构,或者传递零值.
	if (INVALID_HANDLE_VALUE == hPipe)
		cout << "创建管道失败: " << GetLastError() << endl;
	else
	{
		cout << "这是命名管道测试程序中的服务器端" << endl;
		cout << "现在等待客户端连接..." << endl;
		////服务器等待客户端的连接请求的到来（并非连接服务器端的命名管道！）
		/*
1、指向一个命名管道实例的服务器的句柄，该句柄由CreateNamedPipe函数返回
2、指向OVERLAPPED结构体的指针，默认为NULL，表明使用默认的同步IO方式1
		*/
		if (!ConnectNamedPipe(hPipe, NULL))						//阻塞等待客户端连接。  
		{
			cout << "连接失败!" << endl;
			return 1;
		}
		else
			cout << "连接成功!" << endl;
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
作者：土豪gold
来源：CSDN
原文：https://blog.csdn.net/qq_15029743/article/details/79508568
版权声明：本文为博主原创文章，转载请附上博文链接！
		*/
		if (!ReadFile(hPipe, buf, 256, &rLen, NULL))			//接受客户端发送数据
		{
			cout << "从客户端接收并读取数据!" << endl;
			return 2;
		}
		else
			cout << "客户端接收的数据为 ： " << buf << endl << "数据长度为 " << rLen << endl;

		char strMessage[] = "命名管道测试程序";
		WriteFile(hPipe, strMessage, sizeof(strMessage), &wLen, 0); //向客户端发送数据
		CloseHandle(hPipe);											//关闭管道句柄 
	}
	system("pause");
	return 0;
}