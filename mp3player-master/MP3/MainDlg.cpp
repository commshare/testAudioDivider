#include "stdafx.h"
#include "MainDlg.h"

void OnIntialComBox(HWND hwnd)
{
	HWND hwndcomb = GetDlgItem(hwnd, IDC_MODEL);
	static int i = 0;
	if (i == 0)
	{
		//ComboBox_InsertString向ComboBox控件里插入某个选项
		ComboBox_InsertString(hwndcomb, -1, TEXT("顺序播放"));
		ComboBox_InsertString(hwndcomb, -1, TEXT("单曲循环"));	
		ComboBox_InsertString(hwndcomb, -1, TEXT("列表循环"));
		ComboBox_InsertString(hwndcomb, -1, TEXT("随机播放"));
		ComboBox_SetCurSel(hwndcomb, iLoopType); // 设置默认显示内容
		i++;
	}
}

extern int iVolume;
TCHAR DEFAULTLISTFILE[MAX_PATH];//保存默认路径
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	HICON hIcon =LoadIcon((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE), MAKEINTRESOURCE(IDI_ICON1));
	 SendMessage(hwnd, WM_SETICON, TRUE,
		(LPARAM)hIcon);
	 SetVolumeTrack(hwnd);//设置音量条
	 OnIntialComBox(hwnd);//ComBox控件初始化	
	 ReadList(hwnd); // 读取列表文件
	return TRUE;
}


void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	DWORD MESSAGE_INPUT = codeNotify;
	switch (id)
	{
	case IDC_ADD://添加
	{
		addSong(hwnd);
		SaveList(hwnd);//保存列表信息
		break;
	}
	case ID_ADD://添加
	{
		addSong(hwnd);
		SaveList(hwnd);//保存列表信息
		break;
	}
	case IDC_START://播放
	{	
		PlayMusic(hwnd);
		break;
	}
	case IDC_PAUSE://暂停
	{
		PauseMusic(hwnd);
		break;
	}
	case IDC_STOP://停止
	{
		StopMusic(hwnd);
		break;
	}
	case IDC_LAST://上一首
	{
		Jumpmusic(hwnd,0);
		break;
	}
	case IDC_NEXT://下一首
	{
		Jumpmusic(hwnd,1);
		break;
	}
	case IDC_DELETE://删除
	{
		DELETESONG(hwnd);
		SaveList(hwnd);
		break;
	}
	
	case IDC_QINGKONG://清空
	{	int ret = MessageBox(hwnd, TEXT("亲，是否清空列表信息"), TEXT("温馨提示"), MB_YESNO);
		if (IDNO == ret)
		{
		}
		else if (IDYES == ret)
		{
			ClearListAll(hwnd);
			SaveList(hwnd);
		}	 	
		 break;
	}
	case IDC_SONG://歌曲列表
	{
		if (LBN_DBLCLK == codeNotify)//双击播放列表里的一首歌曲时
		{
//			StopMusic(hwnd);
			getCurrentPoint(hwnd);//获得选中的歌曲
			PlayMusic(hwnd);
		}
		break;
	}
	case ID_SAVE:
	{
		SaveList(hwnd);//保存列表信息
		MessageBox(hwnd, TEXT("您的列表已保存！"), TEXT("温馨提示"), MB_OK );	
		break;
	}
	case ID_LOAD:
	{
		ReadList(hwnd);//读取列表信息
		MessageBox(hwnd, TEXT("已载入播放列表！"), TEXT("温馨提示"), MB_OK);
		break;
	}
	case ID_SET://设置
	{
		MessageBox(hwnd, TEXT("  该功能已逃跑\n  详情请咨询作者QQ1399284825"), TEXT("Are you kidding me？"), MB_OK|MB_ICONWARNING);	break;
	}
	case ID_SHUOMING: //说明
	{
		MessageBox(hwnd, TEXT("欢迎阅读简要说明：\n\n文件操作可以保存歌曲列表信息到本地，或者从本地读取到列表中；\n\n点击手动添加歌曲可以添加本地歌曲到列表中，并自动保存到本地；\n\n界面模式中的迷你模式更加简洁，默认标准模式；\n\n友情链接便于大家在互联网搜索歌曲;\n\n点击删除和清空按钮，程序都会自动保存列表中歌曲的删减，不需手动保存；\n\n程序在正常关闭时会自动保存相关信息，便于打开时自动读取保存\n\t请勿随意删除相关文件"), TEXT("说明"), MB_OK);	break;
	}
	case ID_ABOUT: //关于
	{
		MessageBox(hwnd, TEXT("\t此软件由 *吴飞雄* 制作，版权所有！\n\t 感谢你的使用！后续版本会添加歌词功能。\n\t 它是一款相当给力的播放器。\n\t 欢迎大家提出宝贵意见！\n\t 谢谢\n\t\t2016/07"), TEXT("关于软件"), MB_OK);	break;
	}
	case ID_BAIDU: //去百度
	{
		ShellExecute(hwnd, "open", "http://baidu.com/", NULL, NULL, SW_NORMAL);
		break;
	}
	case ID_WANGYI: //去网易云音乐
	{
		ShellExecute(hwnd, "open", "http://music.163.com/", NULL, NULL, SW_NORMAL);
		break;
	}
	case ID_KUGOU: //去酷狗
	{
		ShellExecute(hwnd, "open", "http://www.kugou.com/", NULL, NULL, SW_NORMAL);
		break;
	}
	case ID_MINI://迷你
	{
		SetMiniMode(hwnd);
		break;
	}
	case ID_BIAOZHUN://标准
	{
		SetBIAOZHUNMode(hwnd);
		break;
	}
	default:break;
	}
}

void Main_OnClose(HWND hwnd)
{	
	int ret = MessageBox(hwnd, TEXT("亲，点击确定最小化到托盘\n否则退出该应用程序"), TEXT("温馨提示"), MB_YESNOCANCEL | MB_ICONQUESTION);
	if(IDYES==ret)
	{ 
		MinimizeToTray(hwnd);
		ShowWindow(hwnd, SW_HIDE);
	}
	else if (IDNO == ret)
	{
		SaveList(hwnd);//保存播放列表信息;
		EndDialog(hwnd, 0);
	}
}