#pragma once

#include <string>
#include <vector>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Logger; }


namespace AudioDivider
{
	class Security
	{
	private:
		Logger *logger;
	public:
		virtual ~Security()
		{
			delete logger;
		}

		Security();

		// Enable the privilege to change security parameters
		void EnableSeSecurityPrivilege();

		// Sets the rights for a handle (e.g pipe) to lowest, so even low-integrity programs like browsers are allowed to access it
		void SetLowIntegrity(void* handle);
	};
}
