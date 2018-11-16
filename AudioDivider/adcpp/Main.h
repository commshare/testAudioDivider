#pragma once

#include <string>
#include "tangible_filesystem.h"

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class Configuration; }

using namespace Microsoft::Win32;

namespace AudioDivider
{
	class Program final
	{
	private:
		static Configuration *configuration;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [STAThread] static void Main()
		static void Main();
	};
}
