#include "Security.h"
#include "Logging.h"
#include "Native.h"


namespace AudioDivider
{

	Security::Security()
	{
		logger = Logger::getLogger();
	}

	void Security::EnableSeSecurityPrivilege()
	{
		int error;

		Native::LUID luid = Native::LUID();
		error = Native::LookupPrivilegeValue(nullptr, L"SeSecurityPrivilege", luid);
		if (error == 0)
		{
			logger->Error(L"'LookupPrivilegeValue' failed.");
		}

		void* hToken;
		error = Native::OpenProcessToken(Native::GetCurrentProcess(), Native::TOKEN_QUERY | Native::TOKEN_ADJUST_PRIVILEGES, hToken);
		if (error == 0)
		{
			logger->Error(L"'OpenProcessToken' failed.");
		}

		Native::TOKEN_PRIVILEGES tokenPrivileges;
		tokenPrivileges.PrivilegeCount = 1;
		tokenPrivileges.Privileges = std::vector<Native::LUID_AND_ATTRIBUTES*>(1);
		tokenPrivileges.Privileges[0].Luid = luid;
		tokenPrivileges.Privileges[0].Attributes = static_cast<int>(Native::SE_PRIVILEGE_ENABLED);
		error = Native::AdjustTokenPrivileges(hToken, 0, tokenPrivileges, 0, void*::Zero, void*::Zero);
		if (error == 0)
		{
			logger->Error(L"'AdjustTokenPrivileges' failed.");
		}
	}

	void Security::SetLowIntegrity(void* handle)
	{
		int error;

		std::wstring descriptor = L"S:(ML;;NW;;;LW)";
		void* securityDescriptor = new void*();
		unsigned int securityDescriptorSize;
		error = Native::ConvertStringSecurityDescriptorToSecurityDescriptor(descriptor, Native::SDDL_REVISION_1, securityDescriptor, securityDescriptorSize);
		if (error == 0)
		{
			logger->Error(L"'ConvertStringSecurityDescriptorToSecurityDescriptor' failed.");
		}

		int saclPresent;
		void* sacl = new void*();
		int lpbSaclDefaulted;
		error = Native::GetSecurityDescriptorSacl(securityDescriptor, saclPresent, sacl, lpbSaclDefaulted);
		if (error == 0)
		{
			logger->Error(L"'GetSecurityDescriptorSacl' failed.");
		}

		error = Native::SetSecurityInfo(handle, Native::SE_OBJECT_TYPE::SE_KERNEL_OBJECT, Native::LABEL_SECURITY_INFORMATION, void*::Zero, void*::Zero, void*::Zero, sacl);
		if (error != 0)
		{
			logger->Error(L"'SetSecurityInfo' failed.");
		}
	}
}
