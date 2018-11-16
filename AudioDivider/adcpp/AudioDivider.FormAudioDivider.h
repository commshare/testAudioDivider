#pragma once

#include <string>
#include <vector>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Communication; }
namespace AudioDivider { class Injector; }
namespace AudioDivider { class Configuration; }
namespace AudioDivider { class AutoInjectHandler; }
namespace AudioDivider { class ProgramInfo; }


namespace AudioDivider
{
	class FormAudioDivider : public System::Windows::Forms::Form
	{

	private:
		Communication *communication;
		Injector *injector;
		Configuration *configuration;
		AutoInjectHandler *autoInjectHandler;

	public:
		virtual ~FormAudioDivider()
		{
			this->Dispose(true);

			delete communication;
			delete injector;
			delete configuration;
			delete autoInjectHandler;
			delete components;
			delete treeSound;
			delete timerRefresh;
			delete combo_Devices;
			delete chk_Controlled;
			delete btn_Set;
			delete chk_OnlyShowActive;
			delete chk_AutoControl;
		}

		FormAudioDivider(Configuration *configuration, Communication *communication);

	private:
		void MainForm_Load(void *sender, EventArgs *e);

		std::vector<ProgramInfo*> controlledPrograms;
		std::vector<ProgramInfo*> runningPrograms;

		ProgramInfo *GetRunningProgram(ProgramInfo *program);

		void RefreshSoundInfo();

		void UpdateForm();


		void MainForm_FormClosing(void *sender, FormClosingEventArgs *e);


		void timerRefresh_Tick(void *sender, EventArgs *e);

		bool changedbyUser = true;
		void chk_Controlled_CheckedChanged(void *sender, EventArgs *e);

		void treeSound_AfterSelect(void *sender, TreeViewEventArgs *e);

		void btn_Set_Click(void *sender, EventArgs *e);

		void chk_OnlyShowActive_CheckedChanged(void *sender, EventArgs *e);

		void chk_AutoControl_CheckedChanged(void *sender, EventArgs *e);

		void UpdateState();



		/// <summary>
		/// Required designer variable.
		/// </summary>
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

		System::Windows::Forms::TreeView *treeSound;
		System::Windows::Forms::Timer *timerRefresh;
		System::Windows::Forms::ComboBox *combo_Devices;
		System::Windows::Forms::CheckBox *chk_Controlled;
		System::Windows::Forms::Button *btn_Set;
		System::Windows::Forms::CheckBox *chk_OnlyShowActive;
		System::Windows::Forms::CheckBox *chk_AutoControl;
	};
}
