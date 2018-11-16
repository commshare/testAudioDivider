#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "tangible_filesystem.h"

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { class IniConfiguration; }
namespace AudioDivider { class ProgramAutoInfo; }


namespace AudioDivider
{
	class Configuration
	{
	private:
		std::wstring dataFolder;
		bool showOnlyActivePrograms = false;
		IniConfiguration *iniConfiguration;

	public:
		virtual ~Configuration()
		{
			delete iniConfiguration;
		}

		Configuration(const std::wstring &dataFolder);

		std::wstring getDataFolder() const;

		void Save();

		void Load();

		std::wstring &operator [](const std::wstring &index);

		bool getShowOnlyActivePrograms() const;
		void setShowOnlyActivePrograms(const bool &value);

	private:
		std::vector<ProgramAutoInfo*> autoControls;

	public:
		bool AutoControlExists(const std::wstring &programPath, const std::wstring &windowName);

		void AutoControlAdd(ProgramAutoInfo *autoControl);

		void AutoControlRemove(const std::wstring &programPath, const std::wstring &windowName);

		std::vector<ProgramAutoInfo*> getGetAutoControls() const;
	};
}
