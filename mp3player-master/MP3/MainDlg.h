


void OnIntialComBox(HWND hwnd);
void SetVolumeTrack(HWND hwnd);

BOOL Main_VSCROll(HWND hwnd, HWND hwndctl, UINT code, int pos);
BOOL Main_HSCROLL(HWND hwnd, HWND hwndctl, UINT code, int pos);
BOOL Main_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void Main_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void Main_OnClose(HWND hwnd);
void SetVolume(HWND hwnd, int volume);
int getVolume(HWND hwnd);
void addSong(HWND hwnd);
void GetSongName(TCHAR *temp);
void PlayMusic(HWND hwnd);
void StopMusic(HWND hwnd);
void PauseMusic(HWND hwnd);
int  getCurrentPoint(HWND hwnd);
void CALLBACK SetProgressBar(HWND hwnd, UINT message, UINT iTimerId, DWORD dwTime);//播放歌曲时滑动条自动滚动
void Jumpmusic(HWND hwnd, int num);
void playControl(HWND hwnd);
void SaveList(HWND hwnd);
void ReadList(HWND hwnd);
void DELETESONG(HWND hwnd);
void ClearListAll(HWND hwnd);

void Main_OnSysCommand(HWND hwnd, UINT cmd, int x, int y);// 检测系统最小化消息
void MinimizeToTray(HWND hwnd);//最小化到托盘
void SetMiniMode(HWND hwnd);//迷你模式
void SetBIAOZHUNMode(HWND hwnd);//标准模式





