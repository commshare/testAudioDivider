#pragma once

#include <vector>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class ProgramAutoInfo; }
namespace AudioDivider { class ProgramInfo; }


namespace AudioDivider
{
	class FormAutoControl : public System::Windows::Forms::Form
	{

	private:
		ProgramAutoInfo *programAutoInfo;
		ProgramInfo *programInfo;

	public:
		virtual ~FormAutoControl()
		{
			this->Dispose(true);

			delete programAutoInfo;
			delete programInfo;
			delete components;
			delete radio_ProcessPath;
			delete group_SelectBy;
			delete radio_WindowName;
			delete btn_Ok;
			delete btn_Cancel;
			delete combo_Devices;
			delete lbl_SwitchTo;
			delete group_ControlWhen;
			delete radio_ProgramPlayAudio;
			delete radio_ProgramStarts;
		}

		FormAutoControl(ProgramInfo *programInfo);

	private:
		void btn_Ok_Click(void *sender, EventArgs *e);

		void btn_Cancel_Click(void *sender, EventArgs *e);

		void combo_Devices_SelectedIndexChanged(void *sender, EventArgs *e);

		void RefreshButton();

		void radio_WindowName_CheckedChanged(void *sender, EventArgs *e);

		void radio_ProcessPath_CheckedChanged(void *sender, EventArgs *e);

	public:
		ProgramAutoInfo *GetProgramAutoInfo();



		/// <summary>
		/// Required designer variable.
		/// </summary>
	private:
		System::ComponentModel::IContainer *components = nullptr;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		void Dispose(bool disposing);

//		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent();

//		#endregion

		System::Windows::Forms::RadioButton *radio_ProcessPath;
		System::Windows::Forms::GroupBox *group_SelectBy;
		System::Windows::Forms::RadioButton *radio_WindowName;
		System::Windows::Forms::Button *btn_Ok;
		System::Windows::Forms::Button *btn_Cancel;
		System::Windows::Forms::ComboBox *combo_Devices;
		System::Windows::Forms::Label *lbl_SwitchTo;
		System::Windows::Forms::GroupBox *group_ControlWhen;
		System::Windows::Forms::RadioButton *radio_ProgramPlayAudio;
		System::Windows::Forms::RadioButton *radio_ProgramStarts;
	};
}
