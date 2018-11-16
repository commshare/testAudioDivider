#include "Communication.h"
#include "Logging.h"
#include "Security.h"
#include "Native.h"


namespace AudioDivider
{

	Communication::Communication()
	{
		logger = Logger::getLogger();
		security = new Security();
	}

	Communication::ClientMessage::ClientMessage(int pid, int action, std::vector<unsigned char> &mesData)
	{
		this->pid = pid;
		this->action = action;
		this->data = std::vector<unsigned char>(200);
		for (int i = 0; i < mesData.size(); i++)
		{
			this->data[i] = mesData[i];
		}
		this->dataLen = mesData.size();
	}

	Communication::ClientMessage::ClientMessage(int pid, int action, const std::wstring &message) : ClientMessage(pid, action, Encoding::Unicode->GetBytes(message))
	{
	}

	void Communication::PipeServerThread()
	{
		try
		{
			logger->Log(L"Waiting for clients.");
			serverRunning = true;
			bool first = true;
			while (serverRunning)
			{
				Native::SECURITY_DESCRIPTOR SD = Native::SECURITY_DESCRIPTOR();
				SD.Revision = Native::SECURITY_DESCRIPTOR_REVISION;
				Native::InitializeSecurityDescriptor(SD, Native::SECURITY_DESCRIPTOR_REVISION);
				Native::SetSecurityDescriptorDacl(SD, 1, void*::Zero, 0);
				Native::SECURITY_ATTRIBUTES SA = Native::SECURITY_ATTRIBUTES();
				SA.nLength = System::Runtime::InteropServices::Marshal::SizeOf(SA);
				void* SDPtr = Marshal::AllocHGlobal(System::Runtime::InteropServices::Marshal::SizeOf(SD));
				Marshal::StructureToPtr(SD, SDPtr, false);
				SA.lpSecurityDescriptor = SDPtr;
				SA.bInheritHandle = 1;

				void* hPipe;
				if (first)
				{
					first = false;
					hPipe = Native::CreateNamedPipe(pipeName, Native::PIPE_ACCESS_DUPLEX | Native::WRITE_DAC | Native::ACCESS_SYSTEM_SECURITY | Native::WRITE_OWNER, Native::PIPE_TYPE_MESSAGE | Native::PIPE_READMODE_MESSAGE | Native::PIPE_WAIT | Native::PIPE_REJECT_REMOTE_CLIENTS, Native::PIPE_UNLIMITED_INSTANCES, PipeBufferSize, PipeBufferSize, 0, SA);
					security->SetLowIntegrity(hPipe);
				}
				else // only first pipe can change dacl/sacl
				{
					hPipe = Native::CreateNamedPipe(pipeName, Native::PIPE_ACCESS_DUPLEX, Native::PIPE_TYPE_MESSAGE | Native::PIPE_READMODE_MESSAGE | Native::PIPE_WAIT | Native::PIPE_REJECT_REMOTE_CLIENTS, Native::PIPE_UNLIMITED_INSTANCES, PipeBufferSize, PipeBufferSize, 0, SA);
				}
				if (hPipe == Native::INVALID_HANDLE_VALUE)
				{
					logger->Log(L"CreateNamedPipe failed: ", Marshal::GetLastWin32Error());
					delay(1000);
				}



				if (0 == Native::ConnectNamedPipe(hPipe, void*::Zero))
				{
					if (Marshal::GetLastWin32Error() == 535) // ERROR_PIPE_CONNECTED
					{
						Clients.push_back(hPipe);
						logger->Log(L"AcceptedClient.");
					}
					else
					{
						logger->Log(L"ConnectNamedPipe failed: ", Marshal::GetLastWin32Error());
					}
				}
				else
				{
					Clients.push_back(hPipe);
					logger->Log(L"AcceptedClient.");
				}
			}
		}
		catch (const std::exception &e)
		{
			logger->Error(e.what());
		}

	}

	void Communication::ServerStart()
	{
		serverThread = new Thread([&] () {PipeServerThread();});
		serverThread->IsBackground = true;
		serverThread->Start();
	}

	void Communication::ServerStop()
	{
		serverRunning = false;
		serverThread->Abort();

	}

	void Communication::ServerSend(int pid, int action, const std::wstring &messageStr)
	{
		std::wstring mes;
		std::getline(std::wcin, mes);
		for (int i = 0; i < Clients.size(); i++)
		{
			ClientMessage message = ClientMessage(pid, action, messageStr);
			void* nativeMessage = Marshal::AllocHGlobal(System::Runtime::InteropServices::Marshal::SizeOf(message));
			Marshal::StructureToPtr(message, nativeMessage, false);

			int numBytesWritten;
			if (0 == Native::WriteFile(Clients[i], nativeMessage, System::Runtime::InteropServices::Marshal::SizeOf(message), numBytesWritten, void*::Zero))
			{
				if (Native::GetLastError() == Native::ERROR_NO_DATA)
				{
					logger->Log(L"Client disconnected.");
					Clients.erase(Clients.begin() + i);
					i--;
				}
				else
				{
					logger->Log(L"WriteFile failed: ", Native::GetLastError());
				}
			}
			Marshal::FreeHGlobal(nativeMessage);
		}
	}
}
