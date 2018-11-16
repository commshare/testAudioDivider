#include "AutoInjectHandler.h"
#include "Configuration.h"
#include "Communication.h"
#include "Logging.h"
#include "ProgramAutoInfo.h"
#include "Inject.h"
#include "SoundHandler.h"


namespace AudioDivider
{

	AutoInjectHandler::AutoInjectHandler(Configuration *configuration, Communication *communication)
	{
		this->logger = Logger::getLogger();

		this->configuration = configuration;
		this->communication = communication;

		managementEventWatcher1 = new ManagementEventWatcher();
		managementEventWatcher1->EventArrived += EventProcessStart1;
		managementEventWatcher1->Query = new WqlEventQuery(L"__InstanceCreationEvent", TimeSpan(0, 0, 1), L"TargetInstance isa 'Win32_Process'");
		managementEventWatcher1->Start();

		managementEventWatcher2 = new ManagementEventWatcher();
		managementEventWatcher2->EventArrived += EventProcessStart2;
		managementEventWatcher2->Query = new WqlEventQuery(L"Win32_ProcessStartTrace", TimeSpan(0, 0, 1));
		managementEventWatcher2->Start();

		RunInjectsAll();
	}

	void AutoInjectHandler::Stop()
	{
		managementEventWatcher1->Stop();
		managementEventWatcher2->Stop();
	}

	void AutoInjectHandler::EventProcessStart1(void *sender, EventArrivedEventArgs *e)
	{
		try
		{
			unsigned int processID = 0;
			for (auto property_Renamed : e->NewEvent->Properties)
			{
				if (property_Renamed->Name == L"TargetInstance")
				{
					auto dataCollection = static_cast<ManagementBaseObject*>(property_Renamed->Value);
					processID = static_cast<unsigned int>(dataCollection[L"ProcessID"]);
					RunInject(static_cast<int>(processID), false);
				}
			}
		}
		catch (const std::exception &e1)
		{
		}
	}

	void AutoInjectHandler::EventProcessStart2(void *sender, EventArrivedEventArgs *e)
	{
		try
		{
			unsigned int processID = static_cast<unsigned int>(e->NewEvent->Properties[L"ProcessID"]->Value);
			RunInject(static_cast<int>(processID), false);
		}
		catch (const std::exception &e1)
		{
		}

	}

	void AutoInjectHandler::RunInject(int pid, bool always)
	{
		Process *process = Process::GetProcessById(pid);
		auto autoControls = configuration->getGetAutoControls();

		for (auto autoControl : autoControls)
		{
			try
			{
				if (process->Modules[0]->FileName == autoControl->programPath && autoControl->selectBy == ProgramAutoInfo::SelectBy::ProgramPath || process->MainWindowTitle == autoControl->windowName && autoControl->selectBy == ProgramAutoInfo::SelectBy::Windowname)
				{
					if (autoControl->instantHook || always)
					{
						Injector *injector = new Injector();
						injector->Inject(process->Id);
						delay(500);

						communication->ServerSend(process->Id, 1, autoControl->deviceId);
						SoundHandler::switchDefaultDevice();
					}
					else
					{
						if (!std::find(delayedInjectPids.begin(), delayedInjectPids.end(), pid) != delayedInjectPids.end())
						{
							delayedInjectPids.push_back(pid);
						}
					}
				}
			}
			catch (const std::exception &e1)
			{
			}
		}
	}

	void AutoInjectHandler::RunDelayedInject(int pid)
	{
		if (std::find(delayedInjectPids.begin(), delayedInjectPids.end(), pid) != delayedInjectPids.end())
		{
			RunInject(pid, true);
		}
		delayedInjectPids.Remove(pid);
	}

	void AutoInjectHandler::RunInjectsAll()
	{
		auto autoControls = configuration->getGetAutoControls();

		std::vector<Process*> processes = Process::GetProcesses();
		for (auto process : processes)
		{
			RunInject(process->Id, true);
		}
	}
}
