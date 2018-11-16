#include "stdafx.h"
#include "MainDlg.h"

TCHAR szFile[MAX_PATH]; //函数得到的原始的存放
TCHAR szPath[MAX_PATH]; //路径
TCHAR szFileName[50 * MAX_PATH]; //分离出来的每个文件路径和名字
//文件路径地址
TCHAR chMusic[MAX_PATH];
//当前播放文件短路径名
TCHAR chShortName[MAX_PATH];
//歌曲长度，状态返回值
TCHAR songLength[20];
static TCHAR musicTotalTime[256]; // 记录一首歌的总时间
static int totalSecond; // 记录一首歌的总秒
HWND hwndMusicSileder;//播放歌曲的进度条句柄
HWND hwndVolumeSilder;//音量控制的进度条句柄
int iVolume; //声音大小
//int count;//统计值
static int totalMusicCount = 0; // 记录列表的歌曲的总数
static int numplayingsong = 0;//继续当前播放的歌曲的序号 初始化为0
static int curTime; // 当前歌曲播放的位置
//int pMethod; //自动播放方式


void SetVolumeTrack(HWND hwnd)//初始化音量条
{
	SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETRANGEMIN, false, 0);
	SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETRANGEMAX, false, 1000);
	SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETPOS, true, 200);
	iVolume = 1000 - 200;
}
void SetVolume(HWND hwnd, int volume)
 {
	TCHAR cmd[MAX_PATH];
    //int v = volume;
	wsprintf(cmd, "setaudio mysound volume to %d", volume);
	mciSendString(cmd, (LPTSTR)NULL, 0, 0);


//	wsprintf(szVolume, "setaudio mysound volume to %d", iVolume);
//	mciSendString(szVolume, (LPTSTR)NULL, 0, 0);
	//SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETPOS, true, volume);
	//iVolume = volume;
}
int getVolume(HWND hwnd)////获取音量值
{
	TCHAR cmd[MAX_PATH];
	TCHAR buff[MAX_PATH];
//	wsprintf(cmd, "status %s volume", chShortName);
//	mciSendString(cmd, buff, sizeof(buff), 0);
	return 1000 - (SendMessage(GetDlgItem(hwnd, IDC_VOICE), TBM_GETPOS, 0, 0));

}

///////////////////////////////////////////////////////////////////
BOOL Main_VSCROll(HWND hwnd, HWND hwndctl, UINT code, int pos)//垂直滚动条（音量条）
{

		hwndVolumeSilder = GetDlgItem(hwnd, IDC_VOICE);
		if (hwndVolumeSilder == hwndctl)
		{
			if (TB_ENDTRACK == code) // 鼠标释放
			{
				iVolume = 1000 - (SendMessage(GetDlgItem(hwnd, IDC_VOICE), TBM_GETPOS, 0, 0));
				TCHAR szVolume[MAX_PATH];
				wsprintf(szVolume, "setaudio mysound volume to %d", iVolume);
				mciSendString(szVolume, (LPTSTR)NULL, 0, 0);
			}
		}
	return FALSE;
}



void JumpProcess(HWND hwnd, int pos)//拖动进度条从当前位置开始播放
{
	TCHAR cmd[MAX_PATH + 20];
	wsprintf(cmd, "play mysound from %ld", pos * 1000);
	mciSendString(cmd, "", 0, NULL);
}
BOOL Main_HSCROLL(HWND hwnd, HWND hwndctl, UINT code, int pos)//水平滚动条（歌曲进度条）
{
	TCHAR cmd[MAX_PATH];
	if (TB_ENDTRACK == code)
	{
	 HWND hwndTrack = GetDlgItem(hwnd, IDC_JINGDU);
	 int p = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);
	 wsprintf(cmd, "play %s from %ld", chShortName, p *1000);
	 mciSendString(cmd, "", 0, NULL);
	 }
	hwndMusicSileder = GetDlgItem(hwnd, IDC_JINGDU);
	if (hwndMusicSileder == hwndctl) //如果操作的控件就是滑块
	{
		if (TB_ENDTRACK == code) // 鼠标释放
		{
			int p = SendMessage(hwndctl, TBM_GETPOS, 0, 0);
			JumpProcess(hwnd, p);
		}
	}
	return 0;//临时
}


void addSong(HWND hwnd)
{
	//TCHAR szFile[MAX_PATH]; //函数得到的原始的存放
	//TCHAR szPath[MAX_PATH]; //路径
	//TCHAR szFileName[50 * MAX_PATH]; //分离出来的每个文件路径和名字
	TCHAR *p;
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0');
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("ALL\0*.*\0mp3\0*.mp3\0");
	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.hwndOwner = hwnd;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
	if (GetOpenFileName(&ofn))
	{
		lstrcpyn(szPath, szFile, ofn.nFileOffset);
		szPath[ofn.nFileOffset] = '\0';
		if (szPath[ofn.nFileOffset - 1] != '\\')
		{
			szPath[ofn.nFileOffset - 1] = '\\';
		}
		ZeroMemory(szFileName, sizeof(szFileName));
		p = szFile + ofn.nFileOffset;
		int i = 0;
		while (*p)
		{
			lstrcat(szFileName, szPath);
			lstrcat(szFileName, p);
			SendDlgItemMessage(hwnd, IDC_SONG, LB_INSERTSTRING, totalMusicCount, (LPARAM)szFileName);
			p += strlen(p) + 1;
			i++;
			ZeroMemory(szFileName, sizeof(szFileName));
		}
		totalMusicCount = SendDlgItemMessage(hwnd, IDC_SONG, LB_GETCOUNT, 0, 0);
	}
}



///////////////////////////////////////
//计算一首歌曲的时间
void MusicTotalTime(HWND hwnd)
{
	TCHAR musicTotalLength[256];
	int intSecond, intMinute;
	mciSendString("status mysound length", musicTotalLength, sizeof(musicTotalLength) / sizeof(TCHAR), NULL);//获得歌曲的长度
	totalSecond = atoi(musicTotalLength) / 1000; //总时间（毫秒）除以 1000 得到（整数）总秒数//转换格式
	intMinute = totalSecond / 60; //总秒数除以 6 得到（整数）分钟数
	intSecond = (totalSecond - intMinute * 60); //总秒数减去（整数）分钟*60 得到剩余秒数
	wsprintf(musicTotalTime, "%02d:%02d", intMinute, intSecond); //02s 表示时间分钟或秒数共两位数不足补 0；
	SetDlgItemText(hwnd, IDC_TOTALTIME, musicTotalTime);
}



void CALLBACK MusicNowTime(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{
	//获取歌曲播放的当前位置
	TCHAR curPosition[256];
	mciSendString("status mysound position", curPosition, sizeof(curPosition) / sizeof(TCHAR), NULL); //指定播放位置
	curTime = atol(curPosition);
	int iMinute = curTime / 60000;
	int iSecond = (curTime % 60000) / 1000;
	// 显示时间
	TCHAR sCurTime[256];
	wsprintf(sCurTime, "%02d:%02d", iMinute, iSecond);
	SetDlgItemText(hwnd, IDC_NOWTIME, sCurTime);
	if (0 == strcmp(sCurTime, musicTotalTime)) // 判断一首歌是否播完
	{
		Sleep(200);
		Jumpmusic(hwnd,1);//直接跳到下一首
	}
}



/*****
函数名称： SetProgressBar（回调函数）
函数功能： 播放歌曲时滑动条自动滚动
*****/
void CALLBACK SetProgressBar(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime)
{
	TCHAR nowMusicTime[256];
	TCHAR totalLength[256];
	mciSendString("status mysound length", totalLength, sizeof(totalLength) / sizeof(TCHAR), NULL);//获得歌曲的长度
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETRANGEMAX, false, (LPARAM)totalSecond);
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETRANGEMIN, false, (LPARAM)0);
	mciSendString("status mysound position", nowMusicTime, sizeof(nowMusicTime) / sizeof(TCHAR), NULL);
	int musicSliderTime = atoi(nowMusicTime);
	musicSliderTime = musicSliderTime / 1000;
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETPOS, true, (LPARAM)musicSliderTime);
}

////////////////////////////////////////
//播放音乐
static int flagPauseOrPlay = 1;//声明全局变量判断是暂停还是继续
void StopMusic(HWND hwnd)//停止播放
{
	HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
	if (1 == flagPauseOrPlay)
	{
		SetWindowText(hwndPause, TEXT("暂停II")); //修改按钮的文字
		flagPauseOrPlay = 0;
	}
	mciSendString("close mysound", NULL, 0, NULL);
}
void PlayMusic(HWND hwnd)
{
	HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
	if (1 == flagPauseOrPlay)
	{
		SetWindowText(hwndPause, TEXT("暂停II")); //修改按钮的文字
		flagPauseOrPlay = 0;
	}
	//首先停止当前播放的歌曲
	StopMusic(hwnd);
	//得到要播放的歌曲路径
	HWND listHwnd = GetDlgItem(hwnd, IDC_SONG);
	int curSelIndex = ListBox_GetCurSel(listHwnd);
	numplayingsong = curSelIndex;//将当前播放的歌曲记录到全局变量中
	TCHAR temp[MAX_PATH + 12] = "NULL";
	ListBox_GetText(listHwnd, curSelIndex, temp);
	if(strcmp(temp,"NULL")==0)
			MessageBox(hwnd, TEXT("当前没有选中歌曲或者列表为空！"), TEXT("温馨提示"), MB_OK);
	else
	{
	GetShortPathName(temp, chMusic, sizeof(chMusic));

	TCHAR musicName[MAX_PATH];  
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETTEXT, numplayingsong, (LPARAM)musicName);
	SetDlgItemText(hwnd, IDC_NAME, musicName); // 显示正在播放歌曲的名字

	TCHAR playCmd[MAX_PATH + 12];
	wsprintf(playCmd, "open %s alias mysound", chMusic);
	mciSendString(playCmd, NULL, 0, NULL);
	mciSendString("play mysound", NULL, 0, NULL);

	SetVolume(hwnd, iVolume);

	MusicTotalTime(hwnd); // 显示每首歌曲的总时间
	SetTimer(hwnd, 1, 1000, MusicNowTime);
	SetTimer(hwnd, 2, 1000, SetProgressBar);
	
	}

}
///////////获取去掉路径的歌曲名以便显示


//////////////////////////////////////////////////////////////
void PauseMusic(HWND hwnd)
{
		HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
		if (1 == flagPauseOrPlay)
		{
			SetWindowText(hwndPause, TEXT("暂停II")); //修改按钮的文字
	        //不用PlayMusic函数是避免继续播放时播放光标所在的歌曲
			//TCHAR playCmd[MAX_PATH + 12];
			//wsprintf(playCmd, "open %s alias mysound", chMusic);
			//mciSendString(playCmd, NULL, 0, NULL);//这三句会使清空列表后"继续"按钮有效,即可点击继续播放清空列表前正在播放的歌曲的bug
			mciSendString("play mysound", NULL, 0, NULL);
			flagPauseOrPlay = 0;
		}
		else if (0 == flagPauseOrPlay)
		{
			TCHAR playCmd[MAX_PATH + 12];
			wsprintf(playCmd, "open %s alias mysound", chMusic);
			mciSendString("pause mysound", NULL, 0, NULL);
			flagPauseOrPlay = 1;
			SetWindowText(hwndPause, TEXT("继续△")); //修改按钮的文字
		}
}
int getCurrentPoint(HWND hwnd)//获取当前（选中的）歌曲
{
	int i = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, 0, 0);
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, i, (LPARAM)chMusic);
	//curTime = i;
	return i;
}
//////////////////////////
///上下首歌

void Jumpmusic(HWND hwnd,int num)
{
	int playorstop = 0;
	if(flagPauseOrPlay==1)
	{
		flagPauseOrPlay = 0;
		HWND hwndPause = GetDlgItem(hwnd, IDC_PAUSE);
		SetWindowText(hwndPause, TEXT("暂停II")); //修改按钮的文字
	}
	playControl(hwnd);
	srand((unsigned)time(NULL));//随机数种子
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_SETCURSEL, numplayingsong, 0);
	numplayingsong = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, 0, 0);
	totalMusicCount = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCOUNT, 0, 0);
	if (num == 0)/////////////////////////////上一首//
	{
		if (RANDOM_LOOP == iLoopType)// 随机播放
		{
			int n= numplayingsong;///////避免重复播放
			while (n == numplayingsong)
			{
				numplayingsong = rand() % (totalMusicCount);
			}
		}
		else if (SINGLE_LOOP == iLoopType)//单曲循环
		{
			numplayingsong = numplayingsong;
		}
		else if (CIRCULATION_LOOP == iLoopType)//列表循环
		{
			if (numplayingsong == 0)
			{
				numplayingsong = totalMusicCount - 1;
			}
			else if (numplayingsong < totalMusicCount - 1)
			{
				numplayingsong = numplayingsong - 1;
			}
		}
		else if (ORDER_LOOP == iLoopType)//顺序播放
		{
			if (numplayingsong > 0)
			{
				numplayingsong = numplayingsong - 1;
			}
			else if (numplayingsong == 0)
			{
				playorstop = 1;
			}
		}
	}
	else if (num == 1)///////////////////////////////////////////////////////////下一首
	{
		if (RANDOM_LOOP == iLoopType)// 随机播放
		{
			int n = numplayingsong;///////避免重复播放
			while (n == numplayingsong)
			{
				numplayingsong = rand() % (totalMusicCount);
			}
		}
		else if (SINGLE_LOOP == iLoopType)//单曲循环
		{
			numplayingsong = numplayingsong;
		}
		else if (CIRCULATION_LOOP == iLoopType)//列表循环
		{
			if (numplayingsong < totalMusicCount - 1)
			{
				numplayingsong = numplayingsong + 1;
			}
			else if (numplayingsong == totalMusicCount - 1)
			{
				numplayingsong = 0;
			}
		}
		else if (ORDER_LOOP == iLoopType)//顺序播放
		{
			if (numplayingsong < totalMusicCount - 1)
			{
				numplayingsong = numplayingsong + 1;
			}
			else if (numplayingsong == totalMusicCount - 1)
			{
				playorstop = 1;
			}
		}
	}
	SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_SETCURSEL, numplayingsong, 0);
	numplayingsong = SendMessage(GetDlgItem(hwnd, IDC_SONG), LB_GETCURSEL, 0, 0);
	if(playorstop==0)
		PlayMusic(hwnd);
	else
	{
		StopMusic(hwnd);
	}
}


///////////////////////////////////////////////////
///播放模式
void playControl(HWND hwnd)
{
	HWND hwndcomb = GetDlgItem(hwnd, IDC_MODEL);
	iLoopType =  ComboBox_GetCurSel(hwndcomb);

}
///////////////////////////////////////
////保存列表信息
void SaveList(HWND hwnd)
{
	int i;
	FILE *fp;// 打开保存歌曲文件
	if ((fp = fopen("D:/666list.list", "w")) == NULL)
	{
		MessageBox(hwnd, "保存音乐列表失败！", "抱歉", MB_OK | MB_ICONWARNING);
	}
	else
	{
		iVolume = getVolume(hwnd);
		playControl(hwnd);
		fprintf(fp, "%d,%d,%d\n", iLoopType, totalMusicCount, iVolume); // 保存模式，歌曲数量，音量
		HWND hwndlist = GetDlgItem(hwnd, IDC_SONG);
		for (i = 0; i<totalMusicCount; i++)
		{
			TCHAR szFile[MAX_PATH];
			SendMessage(hwndlist, LB_GETTEXT, i, (LPARAM)szFile);//获得每首歌，并输出到文件
			fwrite(szFile, sizeof(char), MAX_PATH, fp);////fprintf(fp, "%s\n", szFile); 
		}
	} 
	fclose(fp);
}
//////////////////////读取本地信息
void ReadList(HWND hwnd)
{
	FILE *fp;
	if (NULL == (fp = fopen("D:/666list.list", "r")))
	{
		MessageBox(hwnd, "播放列表文件不存在", "警告", MB_OK |MB_ICONWARNING);
	}
	else
	{
			fscanf(fp, "%d,%d,%d\n", &iLoopType, &totalMusicCount, &iVolume);// 读取模式，歌曲总数，音量
			SendDlgItemMessage(hwnd, IDC_VOICE, TBM_SETPOS, true,  1000-iVolume);//设置音量条	
			ComboBox_SetCurSel(GetDlgItem(hwnd,IDC_MODEL), iLoopType);
			for (int i = 0; i<totalMusicCount; i++)
			{
				fread(szFile, sizeof(char), MAX_PATH, fp);
				SendDlgItemMessage(hwnd, IDC_SONG, LB_INSERTSTRING, i, (LPARAM)szFile);
			}
			//PauseMusic(hwnd);
	}
}
//////////////////删除歌曲
void DELETESONG(HWND hwnd)
{
	HWND hwndList = GetDlgItem(hwnd, IDC_SONG);
	int k = ListBox_GetCurSel(hwndList);
	ListBox_DeleteString(hwndList, k);
	if (k == numplayingsong)
	{
		StopMusic(hwnd);
		SetDlgItemText(hwnd, IDC_NAME, NULL); // 当前没有歌曲显示
		KillTimer(hwnd, 1); // 关闭显示当前播放时间的定时器
		KillTimer(hwnd, 2); // 关闭滑动条控制的定时器
		SetDlgItemText(hwnd, IDC_NOWTIME, NULL); // 清空开始时间的显示
		SetDlgItemText(hwnd, IDC_TOTALTIME, NULL); // 清空总时间的显示
		SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETPOS, true, (LPARAM)NULL); // 将滑动条拖回开始位置
		totalMusicCount --; // 将原来记录打开文件时歌曲的数量减一
	}
}
/////删除列表信息
void ClearListAll(HWND hwnd)
{
	HWND hwndList = GetDlgItem(hwnd, IDC_SONG);
	int k = ListBox_GetCount(hwndList);
	for (int i = k - 1; i >= 0; i--)
	{
		ListBox_DeleteString(hwndList, i);
	}
	StopMusic(hwnd);
	SetDlgItemText(hwnd, IDC_NAME, NULL); // 当前没有歌曲显示
	KillTimer(hwnd, 1); // 关闭显示当前播放时间的定时器
	KillTimer(hwnd, 2); // 关闭滑动条控制的定时器
	SetDlgItemText(hwnd, IDC_NOWTIME, NULL); // 清空开始时间的显示
	SetDlgItemText(hwnd, IDC_TOTALTIME, NULL); // 清空总时间的显示
	SendDlgItemMessage(hwnd, IDC_JINGDU, TBM_SETPOS, true, (LPARAM)NULL); // 将滑动条拖回开始位置
	totalMusicCount = 0; // 将原来记录打开文件时歌曲的序号清空
	numplayingsong = 0; // 将原来列表的序号清空
}


///////////////////////////////////
//迷你模式
static int iMiniModeFlag = 1; // 迷你模式的标记//1->标准//0->迷你
void SetMiniMode(HWND hwnd)
{
	HMENU hMenu = GetMenu(hwnd);
	//HWND hwndmodel = GetDlgItem(hwnd, ID_MINI);
	if (0 == iMiniModeFlag)
	{
		MessageBox(hwnd, "当前已是迷你模式！", "提示", MB_OK);
	}else
	{

		SetWindowPos(hwnd, 0, 122, 75, 473, 159, SWP_NOMOVE | SWP_NOZORDER);
		iMiniModeFlag = 0;
	}
}
void SetBIAOZHUNMode(HWND hwnd)
	{
		HMENU hMenu = GetMenu(hwnd);
		if (1 == iMiniModeFlag)
		{
			MessageBox(hwnd, "当前已是标准模式！", "提示", MB_OK );
		}
		else
		{
			SetWindowPos(hwnd, 0, 122, 75, 701, 423, SWP_NOMOVE | SWP_NOZORDER);
			iMiniModeFlag = 1;
		}
}


/////////////////////////////////////
//函数功能： 最小化到托盘
void MinimizeToTray(HWND hwnd)
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = hwnd;
	nid.uID = IDI_ICON1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_MYMESSAGE; //自定义消息
	HINSTANCE hin = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE); //获得程序句柄
	nid.hIcon = LoadIcon(hin, MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(nid.szTip, TEXT("播放器"));
	Shell_NotifyIcon(NIM_ADD, &nid);
	ShowWindow(hwnd, SW_HIDE); //隐藏窗口
}

/////////////////////////////
//函数功能： 检测系统最小化消息

void Main_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
	if (SC_MINIMIZE == cmd)
	{
		ShowWindow(hwnd, SW_HIDE);
		return;
	}
}