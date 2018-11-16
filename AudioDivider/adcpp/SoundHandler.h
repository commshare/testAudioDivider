#pragma once

#include <string>
#include <vector>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class SoundInfoSession; }

using namespace AudioDivider::WASAPI;

namespace AudioDivider
{
	class SoundInfoDevice
	{
	public:
		std::wstring name; // Name of the device as shown in device manager
		std::wstring ID; // Unique sound device ID, for internal use
		std::vector<SoundInfoSession*> sessions; // Sessions playing on this device
	};

	class SoundInfoSession
	{
	public:
		int pid = 0; // Process ID
		std::wstring windowName; // Name of the audio session, currently only the window name
	};

	class SoundHandler final
	{

		// Switches default device to current default devices. Doesn't change the device, but sends an event so the program needs to aquire the device again, this time using our hook.
	public:
		static void switchDefaultDevice();

		// Lists all devices, and for each device all processes that are currently playing sound using that device
		static std::vector<SoundInfoDevice*> getSoundInfo();
	};
}
