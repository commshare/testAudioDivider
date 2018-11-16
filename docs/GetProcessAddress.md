GetProcAddress用法
2012年09月05日 15:32:16 nemo2011 阅读数：13871 标签： dll windows null callback api 更多
个人分类： VC
函数功能描述:GetProcAddress函数检索指定的动态链接库(DLL)中的输出库函数地址。

函数原型：
FARPROC GetProcAddress(
  HMODULE hModule,    // DLL模块句柄
  LPCSTR lpProcName   // 函数名
);

参数：
hModule
  [in] 包含此函数的DLL模块的句柄。LoadLibrary或者GetModuleHandle函数可以返回此句柄。
lpProcName
  [in] 包含函数名的以NULL结尾的字符串，或者指定函数的序数值。如果此参数是一个序数值，它必须在一个字的底字节，高字节必须为0。

返回值：
  如果函数调用成功，返回值是DLL中的输出函数地址。
  如果函数调用失败，返回值是NULL。得到进一步的错误信息，调用函数GetLastError。

注释：
  GetProcAddress函数被用来检索在DLL中的输出函数地址。
  lpProcName指针指向的函数名，拼写和大小写必须和DLL源代码中的模块定义文件(.DEF)中输出段(EXPORTS)中指定的相同。Win32 API函数的输出名可能不同于你在代码中调用的这些函数名，这个不同被宏隐含在相关的SDK头文件中。如果想得到更多信息，请参考Win32函数原型(Win32 Function Prototypes)。
  lpProcName参数能够识别DLL中的函数，通过指定一个与函数相联系的序数值(在.DEF中的EXPORTS段)。GetProcAddress函数验证那个指定的序数值是否在输出的序数1和最高序数值之间(在.DEF中)。函数用这个序数值作为索引从函数表中读函数地址，假如.DEF 文件不连续地定义函数的序数值，如从1到N(N是输出的函数序数值)，错误将会发生，GetProcAddress将会返回一个错误的、非空的地址，虽然指定的序数没有对应的函数。
  为了防止函数不存在，函数应该通过名字指定而不是序数值。

要求：
  Windows NT/2000: 要求Windows NT 3.1 或以后版本。
  Windows 95/98: 要求Windows 95 或以后版本。
  头文件: 在Winbase.h中声明，include Windows.h。
  库文件: Use Kernel32.lib。

参看：
动态链接库纵览(Dynamic-Link Libraries Overview), 动态链接库函数(Dynamic-Link Library Functions),FreeLibrary, GetModuleHandle, LoadLibrary

示例代码：

  调用KERNEL32.DLL中的RegisterServiceProcess(仅在Windows98中适用)

  HMODULE hModule=GetModuleHandle("kernel32.dll");
  if (hModule)
  {
     typedef DWORD (CALLBACK *LPFNREGISTER)(DWORD,DWORD);
     LPFNREGISTER lpfnRegister;
     lpfnRegister=(LPFNREGISTER)GetProcAddress(hModule,"RegisterServiceProcess");
     if (lpfnRegister)
     {
       (*lpfnRegister)(NULL,1L);
     }
  }

