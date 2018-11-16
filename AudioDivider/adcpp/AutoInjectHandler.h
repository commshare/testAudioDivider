#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Configuration; }
namespace AudioDivider { class Communication; }
namespace AudioDivider { class Logger; }


namespace AudioDivider
{
	class AutoInjectHandler
	{
	private:
		Configuration *configuration;
		Communication *communication;

		ManagementEventWatcher *managementEventWatcher1;
		ManagementEventWatcher *managementEventWatcher2;
		Logger *logger;

	public:
		virtual ~AutoInjectHandler()
		{
			delete configuration;
			delete communication;
			delete managementEventWatcher1;
			delete managementEventWatcher2;
			delete logger;
		}

		AutoInjectHandler(Configuration *configuration, Communication *communication);

		void Stop();

	private:
		void EventProcessStart1(void *sender, EventArrivedEventArgs *e);

		void EventProcessStart2(void *sender, EventArrivedEventArgs *e);

		void RunInject(int pid, bool always);

		std::vector<int> delayedInjectPids;

	public:
		void RunDelayedInject(int pid);

		void RunInjectsAll();
	};
}
