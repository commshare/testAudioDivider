#pragma once

#include <string>
#include <stdexcept>


namespace AudioDivider
{
	class ProgramInfo
	{
	private:
		Process *process;
	public:
		std::wstring deviceID;
		std::wstring name;
		int pid = 0;
		virtual ~ProgramInfo()
		{
			delete process;
		}

		ProgramInfo(const std::wstring &name, int pid, const std::wstring &deviceID);

		bool IsAlive();

		std::wstring getPath() const;
	};
}
