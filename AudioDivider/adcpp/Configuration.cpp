#include "Configuration.h"
#include "IniConfiguration.h"
#include "ProgramAutoInfo.h"


namespace AudioDivider
{

	Configuration::Configuration(const std::wstring &dataFolder)
	{
		this->dataFolder = dataFolder;
		iniConfiguration = new IniConfiguration();
	}

	std::wstring Configuration::getDataFolder() const
	{
		return dataFolder;
	}

	void Configuration::Save()
	{
		iniConfiguration->SetValueBool(L"AudioDivider", L"showOnlyActivePrograms", showOnlyActivePrograms);

		iniConfiguration->SetValueInt(L"AudioDivider", L"autoControl", autoControls.size());

		for (int i = 0; i < autoControls.size(); i++)
		{
			iniConfiguration->SetValueInt(L"AutoControl" + std::to_wstring(i), L"selectBy", static_cast<int>(autoControls[i]->selectBy));
			iniConfiguration->SetValueString(L"AutoControl" + std::to_wstring(i), L"windowName", autoControls[i]->windowName);
			iniConfiguration->SetValueString(L"AutoControl" + std::to_wstring(i), L"programPath", autoControls[i]->programPath);
			iniConfiguration->SetValueString(L"AutoControl" + std::to_wstring(i), L"deviceId", autoControls[i]->deviceId);
			iniConfiguration->SetValueBool(L"AutoControl" + std::to_wstring(i), L"instantHook", autoControls[i]->instantHook);
		}


		iniConfiguration->Save(dataFolder + L"\\config.ini");
	}

	void Configuration::Load()
	{
		if (!FileSystem::fileExists(dataFolder + L"\\config.ini"))
		{
			return;
		}

		iniConfiguration->Load(dataFolder + L"\\config.ini");
		showOnlyActivePrograms = iniConfiguration->GetValueBool(L"AudioDivider", L"showOnlyActivePrograms", false);

		autoControls.clear();

		int num = iniConfiguration->GetValueInt(L"AudioDivider", L"autoControl", 0);

		for (int i = 0; i < num; i++)
		{
			int selectBy = iniConfiguration->GetValueInt(L"AutoControl" + std::to_wstring(i), L"selectBy", 0);
			std::wstring windowName = iniConfiguration->GetValueString(L"AutoControl" + std::to_wstring(i), L"windowName");
			std::wstring programPath = iniConfiguration->GetValueString(L"AutoControl" + std::to_wstring(i), L"programPath");
			std::wstring deviceId = iniConfiguration->GetValueString(L"AutoControl" + std::to_wstring(i), L"deviceId");
			bool instantHook = iniConfiguration->GetValueBool(L"AutoControl" + std::to_wstring(i), L"instantHook", false);
			ProgramAutoInfo tempVar(static_cast<ProgramAutoInfo::SelectBy>(selectBy), programPath, windowName, deviceId, instantHook);
			autoControls.push_back(&tempVar);
		}
	}

	std::wstring &Configuration::operator [](const std::wstring &index)
	{
		return L"";
	}

//C# TO C++ CONVERTER TODO TASK: You cannot specify separate 'set' logic for indexers in native C++:
//	void Configuration::setdefault(const std::wstring &index, const std::wstring &value)
//	{
//
//	}

	bool Configuration::getShowOnlyActivePrograms() const
	{
		return showOnlyActivePrograms;
	}

	void Configuration::setShowOnlyActivePrograms(const bool &value)
	{
		showOnlyActivePrograms = value;
		Save();
	}

	bool Configuration::AutoControlExists(const std::wstring &programPath, const std::wstring &windowName)
	{
		return std::find_if(autoControls.begin(), autoControls.end(), [&] (void *autoControl)
		{
			return autoControl->programPath == programPath && autoControl->windowName == windowName;
		}) != autoControls.end();
	}

	void Configuration::AutoControlAdd(ProgramAutoInfo *autoControl)
	{
		autoControls.push_back(autoControl);
		Save();
	}

	void Configuration::AutoControlRemove(const std::wstring &programPath, const std::wstring &windowName)
	{
		autoControls.RemoveAll([&] (void *autoControl)
		{
			return autoControl->programPath == programPath && autoControl->windowName == windowName;
		});
		Save();
	}

	std::vector<ProgramAutoInfo*> Configuration::getGetAutoControls() const
	{
		return autoControls;
	}
}
