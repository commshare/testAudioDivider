#include "Main.h"
#include "Configuration.h"
#include "Logging.h"
#include "Communication.h"
#include "AudioDivider.FormAudioDivider.h"

using namespace Microsoft::Win32;

namespace AudioDivider
{

Configuration *Program::configuration;

	void Program::Main()
	{
		Logger tempVar(configuration);
		Logger::setLogger(&tempVar);

		Communication *communication = new Communication();

		std::wstring workingDirectory = FileSystem::getCurrentDirectory() + L"\\";

		configuration = new Configuration(workingDirectory);
		configuration->Load();

		Registry::SetValue(LR"(HKEY_CURRENT_USER\SOFTWARE\AudioDivider)", L"Path", configuration->getDataFolder());

		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		FormAudioDivider tempVar2(configuration, communication);
		Application::Run(&tempVar2);
	}
}
