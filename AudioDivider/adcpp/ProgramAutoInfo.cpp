#include "ProgramAutoInfo.h"


namespace AudioDivider
{

	ProgramAutoInfo::ProgramAutoInfo(SelectBy selectBy, const std::wstring &programPath, const std::wstring &windowName, const std::wstring &deviceId, bool instantHook)
	{
		this->selectBy = selectBy;
		this->programPath = programPath;
		this->windowName = windowName;
		this->deviceId = deviceId;
		this->instantHook = instantHook;
	}
}
