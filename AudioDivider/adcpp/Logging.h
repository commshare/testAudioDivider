#pragma once

#include <string>
#include <stdexcept>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Configuration; }


namespace AudioDivider
{
	class Logger
	{
	private:
		static Logger *logger;

	public:
		virtual ~Logger()
		{
			delete configuration;
		}

		static Logger *getLogger();
		static void setLogger(Logger *newLogger);


	private:
		Configuration *configuration;

	public:
		Logger(Configuration *configuration);

		void Error(const std::wstring &text);

		void Log(const std::wstring &text);
		void Log(const std::wstring &text, int data);
	};
}
