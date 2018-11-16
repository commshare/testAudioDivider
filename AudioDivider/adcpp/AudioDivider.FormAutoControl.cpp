#include "AudioDivider.FormAutoControl.h"
#include "ProgramAutoInfo.h"
#include "ProgramInfo.h"
#include "SoundHandler.h"


namespace AudioDivider
{

	FormAutoControl::FormAutoControl(ProgramInfo *programInfo)
	{
		this->programInfo = programInfo;
		InitializeComponent();

		std::vector<SoundInfoDevice*> devices = SoundHandler::getSoundInfo();
		for (auto device : devices)
		{
			combo_Devices->Items->Add(device->name);
		}
	}

	void FormAutoControl::btn_Ok_Click(void *sender, EventArgs *e)
	{
		std::vector<SoundInfoDevice*> devices = SoundHandler::getSoundInfo();
		ProgramAutoInfo::SelectBy selectby = static_cast<ProgramAutoInfo::SelectBy>(radio_ProcessPath->Checked ? 0 : 1);
		programAutoInfo = new ProgramAutoInfo(selectby, programInfo->getPath(), programInfo->name, devices[combo_Devices->SelectedIndex]->ID, radio_ProgramStarts->Checked);
		this->Hide();
	}

	void FormAutoControl::btn_Cancel_Click(void *sender, EventArgs *e)
	{
		this->Hide();
	}

	void FormAutoControl::combo_Devices_SelectedIndexChanged(void *sender, EventArgs *e)
	{
		RefreshButton();
	}

	void FormAutoControl::RefreshButton()
	{
		btn_Ok->Enabled = combo_Devices->SelectedIndex != -1 && (radio_ProcessPath->Checked || radio_WindowName->Checked);
	}

	void FormAutoControl::radio_WindowName_CheckedChanged(void *sender, EventArgs *e)
	{
		RefreshButton();
	}

	void FormAutoControl::radio_ProcessPath_CheckedChanged(void *sender, EventArgs *e)
	{
		RefreshButton();
	}

	ProgramAutoInfo *FormAutoControl::GetProgramAutoInfo()
	{
		return programAutoInfo;
	}

	void FormAutoControl::Dispose(bool disposing)
	{
		if (disposing && (components != nullptr))
		{
			delete components;
		}
//C# TO C++ CONVERTER NOTE: There is no explicit call to the base class destructor in C++:
//		base.Dispose(disposing);
	}

	void FormAutoControl::InitializeComponent()
	{
		this->radio_ProcessPath = new System::Windows::Forms::RadioButton();
		this->group_SelectBy = new System::Windows::Forms::GroupBox();
		this->radio_WindowName = new System::Windows::Forms::RadioButton();
		this->btn_Ok = new System::Windows::Forms::Button();
		this->btn_Cancel = new System::Windows::Forms::Button();
		this->combo_Devices = new System::Windows::Forms::ComboBox();
		this->lbl_SwitchTo = new System::Windows::Forms::Label();
		this->group_ControlWhen = new System::Windows::Forms::GroupBox();
		this->radio_ProgramPlayAudio = new System::Windows::Forms::RadioButton();
		this->radio_ProgramStarts = new System::Windows::Forms::RadioButton();
		this->group_SelectBy->SuspendLayout();
		this->group_ControlWhen->SuspendLayout();
		this->SuspendLayout();
		// 
		// radio_ProcessPath
		// 
		this->radio_ProcessPath->AutoSize = true;
		this->radio_ProcessPath->Location = System::Drawing::Point(6, 19);
		this->radio_ProcessPath->Name = L"radio_ProcessPath";
		this->radio_ProcessPath->Size = System::Drawing::Size(85, 17);
		this->radio_ProcessPath->TabIndex = 0;
		this->radio_ProcessPath->TabStop = true;
		this->radio_ProcessPath->Text = L"Programpath";
		this->radio_ProcessPath->UseVisualStyleBackColor = true;
		this->radio_ProcessPath->CheckedChanged += new System::EventHandler(this->radio_ProcessPath_CheckedChanged);
		// 
		// group_SelectBy
		// 
		this->group_SelectBy->Controls->Add(this->radio_WindowName);
		this->group_SelectBy->Controls->Add(this->radio_ProcessPath);
		this->group_SelectBy->Location = System::Drawing::Point(99, 19);
		this->group_SelectBy->Name = L"group_SelectBy";
		this->group_SelectBy->Size = System::Drawing::Size(200, 72);
		this->group_SelectBy->TabIndex = 1;
		this->group_SelectBy->TabStop = false;
		this->group_SelectBy->Text = L"Identify by";
		// 
		// radio_WindowName
		// 
		this->radio_WindowName->AutoSize = true;
		this->radio_WindowName->Location = System::Drawing::Point(6, 42);
		this->radio_WindowName->Name = L"radio_WindowName";
		this->radio_WindowName->Size = System::Drawing::Size(90, 17);
		this->radio_WindowName->TabIndex = 1;
		this->radio_WindowName->TabStop = true;
		this->radio_WindowName->Text = L"Windowname";
		this->radio_WindowName->UseVisualStyleBackColor = true;
		this->radio_WindowName->CheckedChanged += new System::EventHandler(this->radio_WindowName_CheckedChanged);
		// 
		// btn_Ok
		// 
		this->btn_Ok->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->btn_Ok->Enabled = false;
		this->btn_Ok->Location = System::Drawing::Point(12, 153);
		this->btn_Ok->Name = L"btn_Ok";
		this->btn_Ok->Size = System::Drawing::Size(78, 23);
		this->btn_Ok->TabIndex = 2;
		this->btn_Ok->Text = L"OK";
		this->btn_Ok->UseVisualStyleBackColor = true;
		this->btn_Ok->Click += new System::EventHandler(this->btn_Ok_Click);
		// 
		// btn_Cancel
		// 
		this->btn_Cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->btn_Cancel->Location = System::Drawing::Point(486, 149);
		this->btn_Cancel->Name = L"btn_Cancel";
		this->btn_Cancel->Size = System::Drawing::Size(78, 23);
		this->btn_Cancel->TabIndex = 3;
		this->btn_Cancel->Text = L"Cancel";
		this->btn_Cancel->UseVisualStyleBackColor = true;
		this->btn_Cancel->Click += new System::EventHandler(this->btn_Cancel_Click);
		// 
		// combo_Devices
		// 
		this->combo_Devices->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
		this->combo_Devices->FormattingEnabled = true;
		this->combo_Devices->Location = System::Drawing::Point(99, 97);
		this->combo_Devices->Name = L"combo_Devices";
		this->combo_Devices->Size = System::Drawing::Size(391, 21);
		this->combo_Devices->TabIndex = 6;
		this->combo_Devices->SelectedIndexChanged += new System::EventHandler(this->combo_Devices_SelectedIndexChanged);
		// 
		// lbl_SwitchTo
		// 
		this->lbl_SwitchTo->AutoSize = true;
		this->lbl_SwitchTo->Location = System::Drawing::Point(36, 100);
		this->lbl_SwitchTo->Name = L"lbl_SwitchTo";
		this->lbl_SwitchTo->Size = System::Drawing::Size(54, 13);
		this->lbl_SwitchTo->TabIndex = 7;
		this->lbl_SwitchTo->Text = L"Switch to:";
		// 
		// group_ControlWhen
		// 
		this->group_ControlWhen->Controls->Add(this->radio_ProgramPlayAudio);
		this->group_ControlWhen->Controls->Add(this->radio_ProgramStarts);
		this->group_ControlWhen->Location = System::Drawing::Point(319, 19);
		this->group_ControlWhen->Name = L"group_ControlWhen";
		this->group_ControlWhen->Size = System::Drawing::Size(200, 72);
		this->group_ControlWhen->TabIndex = 2;
		this->group_ControlWhen->TabStop = false;
		this->group_ControlWhen->Text = L"Control when";
		// 
		// radio_ProgramPlayAudio
		// 
		this->radio_ProgramPlayAudio->AutoSize = true;
		this->radio_ProgramPlayAudio->Location = System::Drawing::Point(6, 42);
		this->radio_ProgramPlayAudio->Name = L"radio_ProgramPlayAudio";
		this->radio_ProgramPlayAudio->Size = System::Drawing::Size(157, 17);
		this->radio_ProgramPlayAudio->TabIndex = 1;
		this->radio_ProgramPlayAudio->TabStop = true;
		this->radio_ProgramPlayAudio->Text = L"Program starts playing audio";
		this->radio_ProgramPlayAudio->UseVisualStyleBackColor = true;
		// 
		// radio_ProgramStarts
		// 
		this->radio_ProgramStarts->AutoSize = true;
		this->radio_ProgramStarts->Location = System::Drawing::Point(6, 19);
		this->radio_ProgramStarts->Name = L"radio_ProgramStarts";
		this->radio_ProgramStarts->Size = System::Drawing::Size(92, 17);
		this->radio_ProgramStarts->TabIndex = 0;
		this->radio_ProgramStarts->TabStop = true;
		this->radio_ProgramStarts->Text = L"Program starts";
		this->radio_ProgramStarts->UseVisualStyleBackColor = true;
		// 
		// FormAutoControl
		// 
		this->setAcceptButton(this->btn_Ok);
		this->setAutoScaleDimensions(System::Drawing::SizeF(6.0F, 13.0F));
		this->setAutoScaleMode(System::Windows::Forms::AutoScaleMode::Font);
		this->setCancelButton(this->btn_Cancel);
		this->setClientSize(System::Drawing::Size(576, 188));
		this->getControls()->Add(this->group_ControlWhen);
		this->getControls()->Add(this->lbl_SwitchTo);
		this->getControls()->Add(this->combo_Devices);
		this->getControls()->Add(this->btn_Cancel);
		this->getControls()->Add(this->btn_Ok);
		this->getControls()->Add(this->group_SelectBy);
		this->setName(L"FormAutoControl");
		this->setStartPosition(System::Windows::Forms::FormStartPosition::CenterParent);
		this->setText(L"AutoControl");
		this->group_SelectBy->ResumeLayout(false);
		this->group_SelectBy->PerformLayout();
		this->group_ControlWhen->ResumeLayout(false);
		this->group_ControlWhen->PerformLayout();
		this->ResumeLayout(false);
		this->PerformLayout();

	}
}
