#include "AudioDivider.FormAudioDivider.h"
#include "Communication.h"
#include "Inject.h"
#include "Configuration.h"
#include "AutoInjectHandler.h"
#include "ProgramInfo.h"
#include "SoundHandler.h"
#include "AudioDivider.FormAutoControl.h"


namespace AudioDivider
{

	FormAudioDivider::FormAudioDivider(Configuration *configuration, Communication *communication)
	{
		this->configuration = configuration;
		InitializeComponent();
		this->communication = communication;
		injector = new Injector();
		communication->ServerStart();

		autoInjectHandler = new AutoInjectHandler(configuration, communication);

		chk_OnlyShowActive->Checked = configuration->getShowOnlyActivePrograms();
	}

	void FormAudioDivider::MainForm_Load(void *sender, EventArgs *e)
	{
		timerRefresh->Start();
	}

	ProgramInfo *FormAudioDivider::GetRunningProgram(ProgramInfo *program)
	{
		for (auto runningProgram : runningPrograms)
		{
			if (runningProgram->pid == program->pid)
			{
				return runningProgram;
			}
		}
		return nullptr;
	}

	void FormAudioDivider::RefreshSoundInfo()
	{
		if (configuration->getShowOnlyActivePrograms())
		{
			runningPrograms.clear();
		}

		runningPrograms.RemoveAll([&] (void *program)
		{
			!program::IsAlive();
		});

		std::vector<SoundInfoDevice*> devices = SoundHandler::getSoundInfo();

		for (auto device : devices)
		{
			for (auto program : device->sessions)
			{
				ProgramInfo *runningProgram = new ProgramInfo(program->windowName, program->pid, device->ID);
				ProgramInfo *runningProgramExisting = GetRunningProgram(runningProgram);
				if (runningProgramExisting == nullptr)
				{
					runningPrograms.push_back(runningProgram);
				}
				else
				{
					runningProgramExisting->deviceID = runningProgram->deviceID;
				}
				autoInjectHandler->RunDelayedInject(program->pid);
			}
		}
	}

	void FormAudioDivider::UpdateForm()
	{
		treeSound->BeginUpdate();

		std::wstring selectedNodeName = L"";
		if (treeSound->SelectedNode != nullptr)
		{
			selectedNodeName = treeSound->SelectedNode->Text;
		}
		std::wstring selectedDevice = static_cast<std::wstring>(combo_Devices->SelectedItem);


		treeSound->Nodes->Clear();
		combo_Devices->Items->Clear();


		std::vector<SoundInfoDevice*> devices = SoundHandler::getSoundInfo();
		for (auto device : devices)
		{
			combo_Devices->Items->Add(device->name);
			TreeNode *nodeDevice = treeSound->Nodes->Add(device->name);
			for (int i = 0; i < runningPrograms.size(); i++)
			{
				if (runningPrograms[i]->deviceID == device->ID)
				{
					TreeNode *nodeProgram = nodeDevice->Nodes->Add(runningPrograms[i]->name + L" (" + std::to_wstring(runningPrograms[i]->pid) + L")");
					nodeProgram->Tag = i;
				}
			}
		}

		treeSound->ExpandAll();


		for (auto nodeDevice : treeSound->Nodes)
		{
			if (nodeDevice->Text == selectedNodeName)
			{
				treeSound->SelectedNode = nodeDevice;
			}
			for (auto nodeProgram : nodeDevice->Nodes)
			{
				if (nodeProgram->Text == selectedNodeName)
				{
					treeSound->SelectedNode = nodeProgram;
				}
			}
		}
		combo_Devices->SelectedItem = selectedDevice;
		treeSound->EndUpdate();
	}

	void FormAudioDivider::MainForm_FormClosing(void *sender, FormClosingEventArgs *e)
	{
		communication->ServerStop();
		autoInjectHandler->Stop();
	}

	void FormAudioDivider::timerRefresh_Tick(void *sender, EventArgs *e)
	{
		changedbyUser = false;
		RefreshSoundInfo();
		UpdateForm();
		changedbyUser = true;
	}

	void FormAudioDivider::chk_Controlled_CheckedChanged(void *sender, EventArgs *e)
	{
		if (!changedbyUser)
		{
			return;
		}

		ProgramInfo *activeProgram = runningPrograms[static_cast<int>(treeSound->SelectedNode->Tag)];

		if (chk_Controlled->Checked)
		{
			ProgramInfo tempVar(activeProgram->name, activeProgram->pid, L"");
			controlledPrograms.push_back(&tempVar);

			injector->Inject(activeProgram->pid);
		}
		else
		{
			controlledPrograms.RemoveAll([&] (void *program)
			{
				return program->pid == activeProgram->pid;
			});
		}

		UpdateState();
	}

	void FormAudioDivider::treeSound_AfterSelect(void *sender, TreeViewEventArgs *e)
	{
		if (!changedbyUser)
		{
			return;
		}

		UpdateState();
	}

	void FormAudioDivider::btn_Set_Click(void *sender, EventArgs *e)
	{
		if (treeSound->SelectedNode != nullptr && combo_Devices->SelectedItem != nullptr)
		{
			std::vector<SoundInfoDevice*> devices = SoundHandler::getSoundInfo();

			//进程id
			int pid = runningPrograms[static_cast<int>(treeSound->SelectedNode->Tag)]->pid;
			//设备id
			std::wstring deviceId = devices[combo_Devices->SelectedIndex]->ID;
            //发送之后，要切换默认设备么？
			communication->ServerSend(pid, 1, deviceId);
		}

		SoundHandler::switchDefaultDevice();
		delay(100);

		RefreshSoundInfo();
	}

	void FormAudioDivider::chk_OnlyShowActive_CheckedChanged(void *sender, EventArgs *e)
	{
		configuration->setShowOnlyActivePrograms(chk_OnlyShowActive->Checked);
	}

	void FormAudioDivider::chk_AutoControl_CheckedChanged(void *sender, EventArgs *e)
	{
		if (!changedbyUser)
		{
			return;
		}

		ProgramInfo *activeProgram = runningPrograms[static_cast<int>(treeSound->SelectedNode->Tag)];
		if (chk_AutoControl->Checked)
		{
			ProgramInfo tempVar(activeProgram->name, activeProgram->pid, L"");
			FormAutoControl *autoControl = new FormAutoControl(&tempVar);
			if (autoControl->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				configuration->AutoControlAdd(autoControl->GetProgramAutoInfo());
				autoInjectHandler->RunInjectsAll();
			}
		}
		else
		{
			configuration->AutoControlRemove(activeProgram->getPath(), activeProgram->name);
		}

		UpdateState();
	}

	void FormAudioDivider::UpdateState()
	{
		combo_Devices->SelectedIndex = -1;

		changedbyUser = false;

		if (treeSound->SelectedNode == nullptr || treeSound->SelectedNode->Tag == nullptr) // not a program selected
		{
			chk_Controlled->Enabled = false;
			chk_Controlled->Checked = false;

			chk_AutoControl->Enabled = false;
			chk_AutoControl->Checked = false;

			combo_Devices->Enabled = false;

			btn_Set->Enabled = false;
			changedbyUser = true;
			return;
		}

		ProgramInfo *activeProgram = runningPrograms[static_cast<int>(treeSound->SelectedNode->Tag)];

		bool alreadyControlled = false;
		for (auto controlledProgram : controlledPrograms)
		{
			if (controlledProgram->pid == activeProgram->pid)
			{
				alreadyControlled = true;
			}
		}

		if (alreadyControlled)
		{
			chk_Controlled->Enabled = true;
			chk_Controlled->Checked = true;

			chk_AutoControl->Enabled = false;
			chk_AutoControl->Checked = false;

			combo_Devices->Enabled = true;

			btn_Set->Enabled = true;
		}
		else
		{
			if (configuration->AutoControlExists(activeProgram->getPath(), activeProgram->name))
			{
				chk_Controlled->Enabled = false;
				chk_Controlled->Checked = false;

				chk_AutoControl->Enabled = true;
				chk_AutoControl->Checked = true;

				combo_Devices->Enabled = false;
				btn_Set->Enabled = false;
			}
			else
			{
				chk_Controlled->Enabled = true;
				chk_Controlled->Checked = false;

				chk_AutoControl->Enabled = true;
				chk_AutoControl->Checked = false;

				combo_Devices->Enabled = false;
				btn_Set->Enabled = false;
			}
		}

		changedbyUser = true;
	}

	void FormAudioDivider::Dispose(bool disposing)
	{
		if (disposing && (components != nullptr))
		{
			delete components;
		}
//C# TO C++ CONVERTER NOTE: There is no explicit call to the base class destructor in C++:
//		base.Dispose(disposing);
	}

	void FormAudioDivider::InitializeComponent()
	{
		this->components = new System::ComponentModel::Container();
		this->treeSound = new System::Windows::Forms::TreeView();
		this->timerRefresh = new System::Windows::Forms::Timer(this->components);
		this->combo_Devices = new System::Windows::Forms::ComboBox();
		this->chk_Controlled = new System::Windows::Forms::CheckBox();
		this->btn_Set = new System::Windows::Forms::Button();
		this->chk_OnlyShowActive = new System::Windows::Forms::CheckBox();
		this->chk_AutoControl = new System::Windows::Forms::CheckBox();
		this->SuspendLayout();
		// 
		// treeSound
		// 
		this->treeSound->HideSelection = false;
		this->treeSound->Location = System::Drawing::Point(12, 55);
		this->treeSound->Name = L"treeSound";
		this->treeSound->Size = System::Drawing::Size(532, 267);
		this->treeSound->TabIndex = 0;
		this->treeSound->AfterSelect += new System::Windows::Forms::TreeViewEventHandler(this->treeSound_AfterSelect);
		// 
		// timerRefresh
		// 
		this->timerRefresh->Interval = 1000;
		this->timerRefresh->Tick += new System::EventHandler(this->timerRefresh_Tick);
		// 
		// combo_Devices
		// 
		this->combo_Devices->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->combo_Devices->Enabled = false;
		this->combo_Devices->FormattingEnabled = true;
		this->combo_Devices->Location = System::Drawing::Point(550, 137);
		this->combo_Devices->Name = L"combo_Devices";
		this->combo_Devices->Size = System::Drawing::Size(391, 21);
		this->combo_Devices->TabIndex = 5;
		// 
		// chk_Controlled
		// 
		this->chk_Controlled->AutoSize = true;
		this->chk_Controlled->Enabled = false;
		this->chk_Controlled->Location = System::Drawing::Point(550, 104);
		this->chk_Controlled->Name = L"chk_Controlled";
		this->chk_Controlled->Size = System::Drawing::Size(73, 17);
		this->chk_Controlled->TabIndex = 6;
		this->chk_Controlled->Text = L"Controlled";
		this->chk_Controlled->UseVisualStyleBackColor = true;
		this->chk_Controlled->CheckedChanged += new System::EventHandler(this->chk_Controlled_CheckedChanged);
		// 
		// btn_Set
		// 
		this->btn_Set->Enabled = false;
		this->btn_Set->Location = System::Drawing::Point(947, 137);
		this->btn_Set->Name = L"btn_Set";
		this->btn_Set->Size = System::Drawing::Size(35, 23);
		this->btn_Set->TabIndex = 7;
		this->btn_Set->Text = L"Set";
		this->btn_Set->UseVisualStyleBackColor = true;
		this->btn_Set->Click += new System::EventHandler(this->btn_Set_Click);
		// 
		// chk_OnlyShowActive
		// 
		this->chk_OnlyShowActive->AutoSize = true;
		this->chk_OnlyShowActive->Location = System::Drawing::Point(12, 32);
		this->chk_OnlyShowActive->Name = L"chk_OnlyShowActive";
		this->chk_OnlyShowActive->Size = System::Drawing::Size(196, 17);
		this->chk_OnlyShowActive->TabIndex = 8;
		this->chk_OnlyShowActive->Text = L"Show only programs currently active";
		this->chk_OnlyShowActive->UseVisualStyleBackColor = true;
		this->chk_OnlyShowActive->CheckedChanged += new System::EventHandler(this->chk_OnlyShowActive_CheckedChanged);
		// 
		// chk_AutoControl
		// 
		this->chk_AutoControl->AutoSize = true;
		this->chk_AutoControl->Enabled = false;
		this->chk_AutoControl->Location = System::Drawing::Point(689, 104);
		this->chk_AutoControl->Name = L"chk_AutoControl";
		this->chk_AutoControl->Size = System::Drawing::Size(81, 17);
		this->chk_AutoControl->TabIndex = 10;
		this->chk_AutoControl->Text = L"AutoControl";
		this->chk_AutoControl->UseVisualStyleBackColor = true;
		this->chk_AutoControl->CheckedChanged += new System::EventHandler(this->chk_AutoControl_CheckedChanged);
		// 
		// FormAudioDivider
		// 
		this->setAutoScaleDimensions(System::Drawing::SizeF(6.0F, 13.0F));
		this->setAutoScaleMode(System::Windows::Forms::AutoScaleMode::Font);
		this->setClientSize(System::Drawing::Size(1009, 461));
		this->getControls()->Add(this->chk_AutoControl);
		this->getControls()->Add(this->chk_OnlyShowActive);
		this->getControls()->Add(this->btn_Set);
		this->getControls()->Add(this->chk_Controlled);
		this->getControls()->Add(this->combo_Devices);
		this->getControls()->Add(this->treeSound);
		this->setName(L"FormAudioDivider");
		this->setText(L"AudioDivider");
		this->FormClosing += new System::Windows::Forms::FormClosingEventHandler(this->MainForm_FormClosing);
		this->Load += new System::EventHandler(this->MainForm_Load);
		this->ResumeLayout(false);
		this->PerformLayout();

	}
}
