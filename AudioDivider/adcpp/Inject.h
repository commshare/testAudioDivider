#pragma once

#include <string>
#include "tangible_filesystem.h"

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Logger; }
namespace AudioDivider { class Security; }


namespace AudioDivider
{
	class Injector
	{
	private:
		Logger *logger;
		Security *security;
	public:
		virtual ~Injector()
		{
			delete logger;
			delete security;
		}

		Injector();

	private:
		int LoadLibrary32Address = 0; // Address of LoadLibrary in 32Bit modules, we get this using a 32Bit-helper program. We need this because the address of LoadLibrary is different in 64Bit and 32Bit processes.
		std::wstring WorkingDirectory;

		void Initialize();

		// Inject the DLL setting the hook into a process
	public:
		void Inject(int pid);

	};
}
