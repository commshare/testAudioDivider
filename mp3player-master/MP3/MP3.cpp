#include "stdafx.h"
#include "MainDlg.h"


BOOL WINAPI Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_INITDIALOG, Main_OnInitDialog);
		HANDLE_MSG(hWnd, WM_SYSCOMMAND, Main_OnSysCommand);//检测最小化
		HANDLE_MSG(hWnd, WM_HSCROLL, Main_HSCROLL); // 响应音乐滑动条被拖动
		HANDLE_MSG(hWnd, WM_VSCROLL, Main_VSCROll); // 响应音量滑动条被拖动
		HANDLE_MSG(hWnd, WM_COMMAND, Main_OnCommand);//控件事件
		HANDLE_MSG(hWnd, WM_CLOSE, Main_OnClose);//关闭
		case WM_MYMESSAGE:
		{
			if (lParam == WM_LBUTTONDOWN|| lParam == WM_RBUTTONDOWN) //在托盘图标上左/右单击
			{
				ShowWindow(hWnd, SW_SHOW);
			}
		}
	} 
	return FALSE;
}




int APIENTRY wWinMain(HINSTANCE hInstance,
	 HINSTANCE hPrevInstance,
	 LPWSTR    lpCmdLine,
	 int       nCmdShow)
{
	InitCommonControls();
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MP3), NULL, Main_Proc);
	return 0;
}

