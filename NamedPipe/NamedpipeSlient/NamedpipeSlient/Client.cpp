// NamedPipeClt.cpp : 定义控制台应用程序的入口点。
//

#include<Windows.h>
#include<iostream>

using namespace std;
/*
客户端命名管道实现的步骤：
1、判断是否有可以用的命名管道WaitNamedPipe
2、打开管道CreateFile
3、接收客户端发送数据ReadFile & 向客户端发送数据WriteFile
4、关闭管道CloseHandle
*/
int main()
{
	cout << "这是命名管道测试程序的客户端" << endl;
	char buf[256] = "";
	DWORD rLen = 0;
	DWORD wLen = 0;
	Sleep(1000);						//等待管道创建成功！  
	/*
	1、指定命名管道的名称
2、指定超时间隔，NMPWAIT_WAIT_FOREVER表示一直等待，直到出现了一个可用的命名管道的实例
	*/
	if (!WaitNamedPipe(TEXT("\\\\.\\Pipe\\pipeTest"), NMPWAIT_WAIT_FOREVER))
	{
		cout << "connect the namedPipe failed!" << endl;
		return 1;
	}

	/*
	CreateFile("\\\\.\\Pipe\\Test",GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
1、指定用于创建或打开的对象的名称
2、指定对象的访问方式，GENERIC_WRITE表示指定对象具有写访问
3、指定共享方式，如果此参数为0，表示对象不能被共享
4、指向SECURITY_ATTRIBUTES结构的指针，该结构指定了命名管道的安全描述符，如果没有特殊的需求，默认值为NULL
5、指定如何创建文件（OPEN_EXISTING表示打开文件，如果文件不存在，则函数调用失败）
6、设置文件属性和标志（FILE_ATTRIBUTE_NORMAL表示该文件没有其他属性设置）
7、指定具有GENERIC_READ访问方式的模板文件的句柄
--------------------- 
	*/
	HANDLE hPipe = CreateFile(          //创建管道文件，即链接管道  
		TEXT("\\\\.\\Pipe\\pipeTest"),	//管道名称  
		GENERIC_READ | GENERIC_WRITE,   //文件模式  
		0,                              //是否共享  
		NULL,                           //指向一个SECURITY_ATTRIBUTES结构的指针  
		OPEN_EXISTING,                  //创建参数  
		FILE_ATTRIBUTE_NORMAL,          //文件属性，NORMAL为默认属性  
		NULL);                          //模板创建文件的句柄  

	if (INVALID_HANDLE_VALUE == hPipe)
	{
		cout << "打开通道失败!" << endl;
		return 2;
	}
	char strMessage[] = "命名管道测试程序";

	if (!WriteFile(hPipe, strMessage, sizeof(strMessage), &wLen, 0)) //向管道发送数据  
	{
		cout << "向通道写数据失败!" << endl;
		return 3;
	}
	if (!ReadFile(hPipe, buf, 256, &rLen, NULL))					//读取管道数据
	{
		cout << "从通道读数据失败!" << endl;
		return 4;
	}
	else
		cout << "从服务器端接收数据 ： " << buf << endl << "数据长度为：" << rLen << endl;

	Sleep(1000);   //执行挂起一段时间
	CloseHandle(hPipe);					//关闭管道  
	system("pause");
	return 0;
}

