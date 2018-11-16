#include "Inject.h"
#include "Logging.h"
#include "Security.h"
#include "Native.h"


namespace AudioDivider
{

	Injector::Injector()
	{
		logger = Logger::getLogger();
		security = new Security();
		Initialize();
	}

	void Injector::Initialize()
	{
		WorkingDirectory = FileSystem::getCurrentDirectory();

		if (!FileSystem::fileExists(WorkingDirectory + LR"(\Helper.exe)"))
		{
			System::Windows::Forms::MessageBox::Show(L"Helper.exe missing", L"Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}
		if (!FileSystem::fileExists(WorkingDirectory + LR"(\SoundHook32.dll)"))
		{
			System::Windows::Forms::MessageBox::Show(L"SoundHook32.dll missing", L"Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}

	#if !defined(RELEASE32)
		if (!FileSystem::fileExists(WorkingDirectory + LR"(\SoundHook64.dll)"))
		{
			System::Windows::Forms::MessageBox::Show(L"SoundHook64.dll missing", L"Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}
	#endif

		Process::EnterDebugMode();
		security->EnableSeSecurityPrivilege();

		Process *process = new Process();
		process->StartInfo->FileName = WorkingDirectory + LR"(\Helper.exe)";
		process->StartInfo->CreateNoWindow = true;
		process->Start();
		process->WaitForExit();

		LoadLibrary32Address = process->ExitCode;


	}

	void Injector::Inject(int pid)
	{
		int error;

		int isWow;
		void* hProcess = Native::OpenProcess(Native::PROCESS_VM_WRITE | Native::PROCESS_VM_READ | Native::PROCESS_VM_OPERATION | Native::PROCESS_CREATE_THREAD | Native::PROCESS_QUERY_INFORMATION, 0, pid);
		Native::IsWow64Process(hProcess, isWow);

		std::wstring dllPath32 = WorkingDirectory + LR"(\SoundHook32.dll)";
		std::wstring dllPath64 = WorkingDirectory + LR"(\SoundHook64.dll)";

		std::wstring dllPath;
		void* addressLoadLibrary;

	#if defined(RELEASE32)
		dllPath = dllPath32;
		addressLoadLibrary = new void*(LoadLibrary32Address);
	#else
		if (isWow == 1)
		{
			dllPath = dllPath32;
			addressLoadLibrary = new void*(LoadLibrary32Address);
		}
		else
		{
			dllPath = dllPath64;
			void* kernel32 = Native::LoadLibraryA(L"kernel32.dll");
			addressLoadLibrary = Native::GetProcAddress(kernel32, L"LoadLibraryA");
		}
	#endif



		// Write path to DLL into memory
		void tempVar(200);
		void* memPath = Native::VirtualAllocEx(hProcess, void*::Zero, &tempVar, Native::MEM_COMMIT | Native::MEM_RESERVE, Native::PAGE_READWRITE);
		if (memPath == void*::Zero)
		{
			logger->Error(L"'VirtualAllocEx' failed: " + Native::GetLastError());
		}
		void* bytesWritten;
		void* strPtr = Marshal::StringToHGlobalAnsi(dllPath);
		void tempVar2(dllPath.length() + 1);
		error = Native::WriteProcessMemory(hProcess, memPath, strPtr, &tempVar2, bytesWritten);
		if (error != 1)
		{
			logger->Error(L"'WriteProcessMemory' failed.");
		}

		int threadId;
		void* handleThread = Native::CreateRemoteThread(hProcess, void*::Zero, void*::Zero, addressLoadLibrary, memPath, 0, threadId);
		if (handleThread == void*::Zero)
		{
			logger->Log(L"inject failed : ", Marshal::GetLastWin32Error());
			System::Windows::Forms::MessageBox::Show(L"Failed to control the program.", L"Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}
		else
		{
			logger->Log(L"Inject successful");
		}
	}
}
