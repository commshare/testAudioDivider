using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace AudioDivider
{
    public class ProgramInfo
    {
        Process process;
        public string deviceID; //"{0.0.0.00000000}.{5e15fcfe-b0ad-4d3d-8f71-ffd890b3144c}"
        public string name; //"Ascent - Teminite   " 当前正在播放qq音乐的歌曲名称
        public int pid; //11500
        public ProgramInfo(string name, int pid, string deviceID)
        {
            this.name = name;
            this.pid = pid;
            this.deviceID = deviceID;
            //可以获取到qq音乐这个进程
            process = Process.GetProcessById(pid);
        }

        public bool IsAlive()
        {
            try
            {
                return !process.HasExited;
            }
            catch (Exception)
            {
                return true;
            }
        }

        public string Path
        {
            get
            {
                return process.Modules[0].FileName;
            }
        }
    }
}
