#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Logger; }
namespace AudioDivider { class Security; }


namespace AudioDivider
{
   class Communication
   {

	private:
		Logger *logger;
		Security *security;
	public:
		virtual ~Communication()
		{
			delete logger;
			delete security;
			delete serverThread;
		}

		Communication();

		// Message format, as used in the DLLs
	private:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto, Pack = 1)] struct ClientMessage
		class ClientMessage
		{
		public:
			int pid = 0;
			int action = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [MarshalAs(UnmanagedType.ByValArray, SizeConst = 200)] public byte[] data;
			std::vector<unsigned char> data;
			int dataLen = 0;
			ClientMessage(int pid, int action, std::vector<unsigned char> &mesData);

			ClientMessage(int pid, int action, const std::wstring &message);
		};

	private:
		std::vector<void*> Clients;

		int PipeBufferSize = 1000;
		std::wstring pipeName = L"\\\\.\\pipe\\SoundInjectController";

		Thread *serverThread;
//C# TO C++ CONVERTER TODO TASK: 'volatile' has a different meaning in C++:
//ORIGINAL LINE: volatile bool serverRunning;
		bool serverRunning = false;

		void PipeServerThread();

	public:
		void ServerStart();

		void ServerStop();

		// Sends a message to all hooked processes. The DLLs decide whether the message should be processed using the process ID
		void ServerSend(int pid, int action, const std::wstring &messageStr);

   };
}
