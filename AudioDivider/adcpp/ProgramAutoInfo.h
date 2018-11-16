#pragma once

#include <string>


namespace AudioDivider
{
	class ProgramAutoInfo
	{
	public:
		enum class SelectBy
		{
			ProgramPath = 0,
			Windowname
		};

	public:
		SelectBy selectBy = static_cast<SelectBy>(0);
		std::wstring programPath;
		std::wstring windowName;
		std::wstring deviceId;
		bool instantHook = false;

		ProgramAutoInfo(SelectBy selectBy, const std::wstring &programPath, const std::wstring &windowName, const std::wstring &deviceId, bool instantHook);
	};
}
