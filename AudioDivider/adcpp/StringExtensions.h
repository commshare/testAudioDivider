#pragma once

#include <string>
#include "exceptionhelper.h"
#include "stringbuilder.h"


namespace AudioDivider
{
	class StringExtensions final
	{
	public:
		static std::wstring Escape(const std::wstring &str);

		static std::wstring Unescape(const std::wstring &str);
	};
}
