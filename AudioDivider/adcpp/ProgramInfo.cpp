#include "ProgramInfo.h"


namespace AudioDivider
{

	ProgramInfo::ProgramInfo(const std::wstring &name, int pid, const std::wstring &deviceID)
	{
		this->name = name;
		this->pid = pid;
		this->deviceID = deviceID;
		//根据进程id获取进程
		process = Process::GetProcessById(pid);
	}

	bool ProgramInfo::IsAlive()
	{
		try
		{
			return !process->HasExited;
		}
		catch (const std::exception &e1)
		{
			return true;
		}
	}

	std::wstring ProgramInfo::getPath() const
	{
		return process->Modules[0]->FileName;
	}
}
