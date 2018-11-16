#include "Logging.h"
#include "Configuration.h"


namespace AudioDivider
{

Logger *Logger::logger;

	Logger *Logger::getLogger()
	{
		return logger;
	}

	void Logger::setLogger(Logger *newLogger)
	{
		logger = newLogger;
	}

	Logger::Logger(Configuration *configuration)
	{
		this->configuration = configuration;
	}

	void Logger::Error(const std::wstring &text)
	{
		Log(L"Error: " + text);
	}

	void Logger::Log(const std::wstring &text)
	{
		try
		{
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
			File::AppendAllText(configuration->getDataFolder() + L"AudioDivider.log", DateTime::Now.ToString(L"'['hh':'mm':'ss'] '") + L"(Server): " + text + L"\n");
		}
		catch (const std::exception &e1)
		{
			try
			{
				delay(100); // To work against race conditions when injecting the Dll
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
				File::AppendAllText(configuration->getDataFolder() + L"AudioDivider.log", DateTime::Now.ToString(L"'['hh':'mm':'ss'] '") + L"(Server): " + text + L"\n");
			}
			catch (const std::exception &e2)
			{

			}
		}
	}

	void Logger::Log(const std::wstring &text, int data)
	{
		try
		{
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
			File::AppendAllText(configuration->getDataFolder() + L"AudioDivider.log", DateTime::Now.ToString(L"'['hh':'mm':'ss'] '") + L"(Server): " + text + std::to_wstring(data) + L"\n");
		}
		catch (const std::exception &e1)
		{
			try
			{
				delay(100); // To work against race conditions when injecting the Dll
//C# TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'ToString':
				File::AppendAllText(configuration->getDataFolder() + L"AudioDivider.log", DateTime::Now.ToString(L"'['hh':'mm':'ss'] '") + L"(Server): " + text + std::to_wstring(data) + L"\n");
			}
			catch (const std::exception &e2)
			{

			}
		}

	}
}
