// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <windowsx.h>
#include <time.h>
#include <commdlg.h>//控件头文件
#include <Shlobj.h> // 引入 添加文件夹//FileInit函数必需
#include <mmsystem.h> // 引入 mciSendString 函数
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h> 

// TODO:  在此处引用程序需要的其他头文件

#include "shellapi.h" //引入 NOTIFYICONDATA
#include "resource.h"
#include "COMMCTRL.h"
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"winmm.lib") //音乐库文件

//当前状态
//TCHAR status[MAX_PATH];
//int pMethod; //自动播放方式

#define WM_MYMESSAGE WM_USER+100 // 自定义消息,响应最小化托盘的
// 循环模式的宏定义
#define ORDER_LOOP 0 // 顺序播放
#define SINGLE_LOOP 1 // 单曲循环
#define CIRCULATION_LOOP 2 // 列表循环
#define RANDOM_LOOP 3 // 随机播放
static int iLoopType=0 ;// 循环模式，初始为顺序播放



