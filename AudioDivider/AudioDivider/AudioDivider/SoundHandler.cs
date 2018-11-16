using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Threading;
using System.IO;
using AudioDivider.WASAPI;

namespace AudioDivider
{
    class SoundInfoDevice
    {
        public string name; // Name of the device as shown in device manager
        public string ID; // Unique sound device ID, for internal use
        public List<SoundInfoSession> sessions = new List<SoundInfoSession>(); // Sessions playing on this device
    }

    class SoundInfoSession
    {
        public int pid; // Process ID
        public string windowName; // Name of the audio session, currently only the window name
    }

    static class SoundHandler
    {
        //切换默认设备
        // Switches default device to current default devices. Doesn't change the device, but sends an event so the program needs to aquire the device again, this time using our hook.
        public static void switchDefaultDevice()
        {
            List<SoundInfoDevice> devices = getSoundInfo();
            if (devices.Count >= 2)
            {
                IMMDeviceEnumerator enumerator = (IMMDeviceEnumerator)new DeviceEnumerator();
                string id = enumerator.GetDefaultAudioEndpoint(EDataFlow.eRender, ERole.eMultimedia).GetId();

                IPolicyConfig policyConfig = (IPolicyConfig)new IPolicyConfigClass();
                policyConfig.SetDefaultEndpoint(id, ERole.eMultimedia);
            }
        }

        // Lists all devices, and for each device all processes that are currently playing sound using that device
        public static List<SoundInfoDevice> getSoundInfo()
        {
            List<SoundInfoDevice> soundInfoDevices = new List<SoundInfoDevice>();
            //创建一个设备枚举器
            DeviceEnumerator enumerator = new DeviceEnumerator();
            //设备枚举
            IMMDeviceEnumerator deviceEnumerator = (IMMDeviceEnumerator)enumerator;
            //所有设备集合，被激活的设备
            IMMDeviceCollection deviceCollection = deviceEnumerator.EnumAudioEndpoints(EDataFlow.eRender, DeviceStatemask.DEVICE_STATE_ACTIVE);
            //设备数量
            uint deviceCount = deviceCollection.GetCount();
            //对每个设备
            for (uint i = 0; i < deviceCount; i++)
            {
                //创建一个内部的声音设备信息数据结构
                SoundInfoDevice soundInfoDevice = new SoundInfoDevice();
                //加入list中
                soundInfoDevices.Add(soundInfoDevice);
                //设备
                IMMDevice device = deviceCollection.Item(i);
                //读取设备信息，存储到内部结构体
                string deviceId = device.GetId();
                soundInfoDevice.ID = deviceId;
                //设备属性？
                IMMPropertyStore propertyStore = device.OpenPropertyStore(ProperyStoreMode.STGM_READ);
                PropertyKey propertyKeyDeviceDesc = new PropertyKey();
                propertyKeyDeviceDesc.fmtid = new Guid(0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0);
                propertyKeyDeviceDesc.pid = 2;
                PropVariant deviceNamePtr = propertyStore.GetValue(ref propertyKeyDeviceDesc);
                string deviceName = Marshal.PtrToStringUni(deviceNamePtr.pszVal);
                //通过以上情况获取到设备名
                soundInfoDevice.name = deviceName;
                //这个是自己搞的guid么？
                Guid guidAudioSessionManager2 = new Guid("77AA99A0-1BD6-484F-8BC7-2C654C9A9B6F");
                //这个管理音频会话的是干啥的？
                IAudioSessionManager2 audioSessionManager = (IAudioSessionManager2)device.Activate(ref guidAudioSessionManager2, (int)ClsCtx.CLSCTX_ALL, IntPtr.Zero);

                //枚举音频回话
                IAudioSessionEnumerator sessionEnumerator = audioSessionManager.GetSessionEnumerator();

                //回话的数量
                int sessionCount = sessionEnumerator.GetCount();
                for (int j = 0; j < sessionCount; j++)
                {
                    //每个session
                    IAudioSessionControl audioSessionControl = sessionEnumerator.GetSession(j);
                    IAudioSessionControl2 audioSessionControl2 = (IAudioSessionControl2)audioSessionControl;
                    AudioSessionState state = audioSessionControl.GetState();
                    //只看激活状态的
                    if (state == AudioSessionState.AudioSessionStateActive)
                    {
                        //回话
                        SoundInfoSession soundInfoSession = new SoundInfoSession();
                        soundInfoDevice.sessions.Add(soundInfoSession);

                        //通过回话控制得到回话的名字
                        string displayName = audioSessionControl.GetDisplayName();
                        string iconPath = audioSessionControl.GetIconPath();
                        //回话还能拿到进程的id
                        int processId = audioSessionControl2.GetProcessId();
                        //拿到进程的名字 "Let Me Know - 豆瓣FM - Google Chrome" 或者 "Nekozilla - Different Heaven   "
                        string processName = Process.GetProcessById(processId).MainWindowTitle;
                        //会话的进程的id
                        soundInfoSession.pid = processId;
                        //进程名字作为窗口名字？
                        soundInfoSession.windowName = processName;
                    }
                }
            }

            return soundInfoDevices;
        }
    }
}
