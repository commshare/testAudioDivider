#pragma once

#include <string>
#include <vector>


namespace AudioDivider
{
	class Native
	{

		// #################### values for pipes ####################
	public:
		static void* INVALID_HANDLE_VALUE;
		static constexpr int PIPE_ACCESS_DUPLEX = 0x00000003;
		static constexpr int PIPE_WAIT = 0x00000000;
		static constexpr int PIPE_TYPE_MESSAGE = 0x00000004;
		static constexpr int PIPE_READMODE_MESSAGE = 0x00000002;
		static constexpr int PIPE_REJECT_REMOTE_CLIENTS = 0x00000008;
		static constexpr int PIPE_UNLIMITED_INSTANCES = 255;
		static constexpr int ERROR_NO_DATA = 232;

		static constexpr int WRITE_OWNER = 0x00080000;

		// #################### Values for memory ####################
		static constexpr int PROCESS_VM_WRITE = 0x0020;
		static constexpr int PROCESS_VM_READ = 0x0010;
		static constexpr int PROCESS_VM_OPERATION = 0x0008;
		static constexpr int PROCESS_CREATE_THREAD = 0x0002;
		static constexpr int PROCESS_QUERY_INFORMATION = 0x0400;

		static constexpr int MEM_COMMIT = 0x00001000;
		static constexpr int MEM_RESERVE = 0x00002000;

		static constexpr int PAGE_READWRITE = 0x04;

		// #################### Values for security ####################
		static constexpr unsigned int OWNER_SECURITY_INFORMATION = 0x00000001;
		static constexpr unsigned int GROUP_SECURITY_INFORMATION = 0x00000002;
		static constexpr unsigned int DACL_SECURITY_INFORMATION = 0x00000004;
		static constexpr unsigned int SACL_SECURITY_INFORMATION = 0x00000008;
		static constexpr unsigned int LABEL_SECURITY_INFORMATION = 0x00000010;

		static constexpr unsigned int PROTECTED_DACL_SECURITY_INFORMATION = 0x80000000;
		static constexpr unsigned int PROTECTED_SACL_SECURITY_INFORMATION = 0x40000000;
		static constexpr unsigned int UNPROTECTED_DACL_SECURITY_INFORMATION = 0x20000000;
		static constexpr unsigned int UNPROTECTED_SACL_SECURITY_INFORMATION = 0x10000000;

		static constexpr int ACCESS_SYSTEM_SECURITY = 0x01000000;
		static constexpr int WRITE_DAC = 0x00040000;

		static constexpr int SDDL_REVISION_1 = 1;

		static constexpr int TOKEN_QUERY = 0x0008;
		static constexpr int TOKEN_ADJUST_PRIVILEGES = 0x0020;

		static constexpr int SE_PRIVILEGE_ENABLED_BY_DEFAULT = 0x00000001;
		static constexpr unsigned int SE_PRIVILEGE_ENABLED = 0x00000002;
		static constexpr unsigned int SE_PRIVILEGE_REMOVED = 0X00000004;
		static constexpr unsigned int SE_PRIVILEGE_USED_FOR_ACCESS = 0x80000000;

		static constexpr int SECURITY_DESCRIPTOR_REVISION = 1;

		// #################### Structures and enums ####################
	public:
		enum class SE_OBJECT_TYPE
		{
			SE_UNKNOWN_OBJECT_TYPE = 0,
			SE_FILE_OBJECT,
			SE_SERVICE,
			SE_PRINTER,
			SE_REGISTRY_KEY,
			SE_LMSHARE,
			SE_KERNEL_OBJECT,
			SE_WINDOW_OBJECT,
			SE_DS_OBJECT,
			SE_DS_OBJECT_ALL,
			SE_PROVIDER_DEFINED_OBJECT,
			SE_WMIGUID_OBJECT,
			SE_REGISTRY_WOW64_32KEY,
		};

	public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential, Pack = 1)] public struct LUID
		class LUID
		{
		public:
			int LowPart = 0;
			int HighPart = 0;
		};

	public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential, Pack = 1)] public struct LUID_AND_ATTRIBUTES
		class LUID_AND_ATTRIBUTES
		{
		public:
			LUID Luid;
			int Attributes = 0;
		};

	public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential, Pack = 1)] public struct TOKEN_PRIVILEGES
		class TOKEN_PRIVILEGES
		{
		public:
			int PrivilegeCount = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1)] public LUID_AND_ATTRIBUTES[] Privileges;
			std::vector<LUID_AND_ATTRIBUTES> Privileges;
		};

	public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential)] public struct SECURITY_DESCRIPTOR
		class SECURITY_DESCRIPTOR
		{
		public:
			unsigned char Revision = 0;
			unsigned char Sbz1 = 0;
			short Control = 0;
			void* Owner;
			void* Group;
			void* Sacl;
			void* Dacl;
		};

	public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential)] public struct SECURITY_ATTRIBUTES
		class SECURITY_ATTRIBUTES
		{
		public:
			int nLength = 0;
			void* lpSecurityDescriptor;
			int bInheritHandle = 0;
		};

		// #################### Methods for pipe communication ####################
//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32", SetLastError = true)]
	public:
		__declspec(dllimport) static void* CreateNamedPipe(const std::wstring &lpName, int dwOpenMode, int dwPipeMode, int nMaxInstances, int nOutBufferSize, int nInBufferSize, int nDefaultTimeOut, SECURITY_ATTRIBUTES &lpSecurityAttributes);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32", SetLastError = true)]
		__declspec(dllimport) static int ConnectNamedPipe(void* hNamedPipe, void* lpOverlapped);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static int WriteFile(void* hFile, void* lpBuffer, int nNumberOfBytesToWrite, int &lpNumberOfBytesWritten, void* lpOverlapped);

		// #################### Methods for injection ####################
//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static void* GetCurrentProcess();

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static void* OpenProcess(int dwDesiredAccess, int bInheritHandle, int dwProcessId);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static void* LoadLibraryA(const std::wstring &lpFileName);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static void* GetProcAddress(void* hModule, const std::wstring &lpProcName);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static void* VirtualAllocEx(void* hProcess, void* lpAddress, void* dwSize, int flAllocationType, int flProtect);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static int WriteProcessMemory(void* hProcess, void* lpBaseAddress, void* lpBuffer, void* nSize, void* &lpNumberOfBytesWritten);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32", SetLastError = true)]
		__declspec(dllimport) static void* CreateRemoteThread(void* hProcess, void* lpThreadAttributes, void* dwStackSize, void* lpStartAddress, void* lpParameter, int dwCreationFlags, int &lpThreadId);

		// #################### Methods regarding security ####################
//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("advapi32")]
		__declspec(dllimport) static int OpenProcessToken(void* ProcessHandle, int DesiredAccess, void* &TokenHandle);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("Advapi32.dll")]
		__declspec(dllimport) static int InitializeSecurityDescriptor(SECURITY_DESCRIPTOR &refpSecurityDescriptor, int dwRevision);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("Advapi32.dll")]
		__declspec(dllimport) static int SetSecurityDescriptorDacl(SECURITY_DESCRIPTOR &pSecurityDescriptor, int bDaclPresent, void* pDacl, int bDaclDefaulted);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("Advapi32")]
		__declspec(dllimport) static int SetSecurityInfo(void* handle, SE_OBJECT_TYPE ObjectType, unsigned int SecurityInfo, void* psidOwner, void* psidGroup, void* pDacl, void* pSacl);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("advapi32")]
		__declspec(dllimport) static int LookupPrivilegeValue(const std::wstring &lpSystemName, const std::wstring &lpName, LUID &lpLuid);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("advapi32")]
		__declspec(dllimport) static int AdjustTokenPrivileges(void* TokenHandle, int DisableAllPrivileges, TOKEN_PRIVILEGES &NewState, int BufferLength, void* PreviousState, void* ReturnLength);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("Advapi32")]
		__declspec(dllimport) static int ConvertStringSecurityDescriptorToSecurityDescriptor(const std::wstring &StringSecurityDescriptor, int StringSDRevision, void* &SecurityDescriptor, unsigned int &SecurityDescriptorSize);
//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("Advapi32")]
		__declspec(dllimport) static int GetSecurityDescriptorSacl(void* pSecurityDescriptor, int &lpbSaclPresent, void* &pSacl, int &lpbSaclDefaulted);

		// #################### Misc methods ####################
//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32.dll")]
		__declspec(dllimport) static int IsWow64Process(void* hProcess, int &Wow64Process);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static int TerminateThread(void* hThread, int dwExitCode);

//C# TO C++ CONVERTER NOTE: The following .NET DllImport attribute was converted using the Microsoft-specific __declspec(dllimport):
//[DllImport("kernel32")]
		__declspec(dllimport) static int GetLastError();

	};
}
