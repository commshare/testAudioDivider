using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace AudioDivider
{
    public partial class FormAudioDivider : Form
    {

        Communication communication;
        Injector injector;
        Configuration configuration;
        AutoInjectHandler autoInjectHandler;

        public FormAudioDivider(Configuration configuration, Communication communication)
        {
            this.configuration = configuration;
            InitializeComponent();
            this.communication = communication;
            injector = new Injector();
            communication.ServerStart();

            autoInjectHandler = new AutoInjectHandler(configuration, communication);

            chk_OnlyShowActive.Checked = configuration.ShowOnlyActivePrograms;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            timerRefresh.Start();
        }

        List<ProgramInfo> controlledPrograms = new List<ProgramInfo>();
        //这个是正在运行的程序吧
        List<ProgramInfo> runningPrograms = new List<ProgramInfo>();

        ProgramInfo GetRunningProgram(ProgramInfo program)
        { //program被填充：Path："D:\\Program Files (x86)\\Tencent\\QQMusic\\QQMusic.exe" deviceId "{0.0.0.00000000}.{5e15fcfe-b0ad-4d3d-8f71-ffd890b3144c}"
            foreach (var runningProgram in runningPrograms)
            {
                if (runningProgram.pid == program.pid)
                    return runningProgram;
            }
            return null;
        }
        //刷新声音信息
        void RefreshSoundInfo()
        {
            if (configuration.ShowOnlyActivePrograms)
                runningPrograms.Clear();

            runningPrograms.RemoveAll(program => !program.IsAlive());

            List<SoundInfoDevice> devices = SoundHandler.getSoundInfo();
            //遍历所有设备
            foreach (var device in devices)
            {
                foreach (var program in device.sessions)
                {
                    ProgramInfo runningProgram = new ProgramInfo(program.windowName, program.pid, device.ID);
                    ProgramInfo runningProgramExisting = GetRunningProgram(runningProgram);
                    if (runningProgramExisting == null)
                        runningPrograms.Add(runningProgram);
                    else
                        runningProgramExisting.deviceID = runningProgram.deviceID;
                    //要自动注入么？
                    autoInjectHandler.RunDelayedInject(program.pid);
                }
            }
        }

        void UpdateForm()
        {
            treeSound.BeginUpdate();

            string selectedNodeName = null;
            if (treeSound.SelectedNode != null)
            {
                selectedNodeName = treeSound.SelectedNode.Text;
            }
            string selectedDevice = (string)combo_Devices.SelectedItem;


            treeSound.Nodes.Clear();
            combo_Devices.Items.Clear();


            List<SoundInfoDevice> devices = SoundHandler.getSoundInfo();
            foreach (var device in devices)
            {
                combo_Devices.Items.Add(device.name);
                TreeNode nodeDevice = treeSound.Nodes.Add(device.name);
                for (int i = 0; i < runningPrograms.Count; i++)
                {
                    if (runningPrograms[i].deviceID == device.ID)
                    {
                        TreeNode nodeProgram = nodeDevice.Nodes.Add(runningPrograms[i].name + " (" + runningPrograms[i].pid + ")");
                        nodeProgram.Tag = i;
                    }
                }
            }

            treeSound.ExpandAll();


            foreach (TreeNode nodeDevice in treeSound.Nodes)
            {
                if (nodeDevice.Text == selectedNodeName)
                    treeSound.SelectedNode = nodeDevice;
                foreach (TreeNode nodeProgram in nodeDevice.Nodes)
                {
                    if (nodeProgram.Text == selectedNodeName)
                        treeSound.SelectedNode = nodeProgram;
                }
            }
            combo_Devices.SelectedItem = selectedDevice;
            treeSound.EndUpdate();
        }


        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            communication.ServerStop();
            autoInjectHandler.Stop();
        }


        private void timerRefresh_Tick(object sender, EventArgs e)
        {
            changedbyUser = false;
            RefreshSoundInfo();
            UpdateForm();
            changedbyUser = true;
        }

        bool changedbyUser = true;
        private void chk_Controlled_CheckedChanged(object sender, EventArgs e)
        {
            if (!changedbyUser)
                return;

            ProgramInfo activeProgram = runningPrograms[(int)treeSound.SelectedNode.Tag];

            if (chk_Controlled.Checked)
            {
                controlledPrograms.Add(new ProgramInfo(activeProgram.name, activeProgram.pid, null));

                injector.Inject(activeProgram.pid);
            }
            else
            {
                controlledPrograms.RemoveAll(program=>program.pid == activeProgram.pid);
            }

            UpdateState();
        }

        private void treeSound_AfterSelect(object sender, TreeViewEventArgs e)
        {
            if (!changedbyUser)
                return;

            UpdateState();
        }

        private void btn_Set_Click(object sender, EventArgs e)
        {
            if (treeSound.SelectedNode != null && combo_Devices.SelectedItem != null)
            {   //所有设备及其声音信息
                List<SoundInfoDevice> devices = SoundHandler.getSoundInfo();
                //拿出一个进程？
                int pid = runningPrograms[(int)treeSound.SelectedNode.Tag].pid;
                string deviceId = devices[combo_Devices.SelectedIndex].ID;//拿出一个设备id？
                //将播放声音的进程与某个输出设备绑定，1是什么
                //AOL 		deviceId	"{0.0.0.00000000}.{44486876-6dc6-4247-bddd-b6a3e13007c1}"	string
                //扬声器		deviceId	"{0.0.0.00000000}.{5a537756-fa6e-47e0-8af7-5d6aa48f1b8a}"	string
                communication.ServerSend(pid, 1, deviceId);
            }

            SoundHandler.switchDefaultDevice();
            Thread.Sleep(100);

            RefreshSoundInfo();
        }

        private void chk_OnlyShowActive_CheckedChanged(object sender, EventArgs e)
        {
            configuration.ShowOnlyActivePrograms = chk_OnlyShowActive.Checked;
        }

        private void chk_AutoControl_CheckedChanged(object sender, EventArgs e)
        {
            if (!changedbyUser)
                return;

            ProgramInfo activeProgram = runningPrograms[(int)treeSound.SelectedNode.Tag];
            if (chk_AutoControl.Checked)
            {
                FormAutoControl autoControl = new FormAutoControl(new ProgramInfo(activeProgram.name, activeProgram.pid, null));
                if (autoControl.ShowDialog() == DialogResult.OK)
                {
                    configuration.AutoControlAdd(autoControl.GetProgramAutoInfo());
                    autoInjectHandler.RunInjectsAll();
                }
            }
            else
            {
                configuration.AutoControlRemove(activeProgram.Path, activeProgram.name);
            }

            UpdateState();
        }

        void UpdateState()
        {
            combo_Devices.SelectedIndex = -1;

            changedbyUser = false;

            if (treeSound.SelectedNode == null || treeSound.SelectedNode.Tag == null) // not a program selected
            {
                chk_Controlled.Enabled = false;
                chk_Controlled.Checked = false;

                chk_AutoControl.Enabled = false;
                chk_AutoControl.Checked = false;

                combo_Devices.Enabled = false;

                btn_Set.Enabled = false;
                changedbyUser = true;
                return;
            }

            ProgramInfo activeProgram = runningPrograms[(int)treeSound.SelectedNode.Tag];

            bool alreadyControlled = false;
            foreach (var controlledProgram in controlledPrograms)
            {
                if (controlledProgram.pid == activeProgram.pid)
                    alreadyControlled = true;
            }

            if (alreadyControlled)
            {
                chk_Controlled.Enabled = true;
                chk_Controlled.Checked = true;

                chk_AutoControl.Enabled = false;
                chk_AutoControl.Checked = false;

                combo_Devices.Enabled = true;

                btn_Set.Enabled = true;
            }
            else
            {
                if (configuration.AutoControlExists(activeProgram.Path, activeProgram.name))
                {
                    chk_Controlled.Enabled = false;
                    chk_Controlled.Checked = false;

                    chk_AutoControl.Enabled = true;
                    chk_AutoControl.Checked = true;

                    combo_Devices.Enabled = false;
                    btn_Set.Enabled = false;
                }
                else
                {
                    chk_Controlled.Enabled = true;
                    chk_Controlled.Checked = false;

                    chk_AutoControl.Enabled = true;
                    chk_AutoControl.Checked = false;

                    combo_Devices.Enabled = false;
                    btn_Set.Enabled = false;
                }
            }

            changedbyUser = true;
        }

    }
}
