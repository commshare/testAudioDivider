#include "StringExtensions.h"


namespace AudioDivider
{

	std::wstring StringExtensions::Escape(const std::wstring &str)
	{
		StringBuilder *sb = new StringBuilder();
		for (auto c : str)
		{
			switch (c)
			{
				case L'\n':
					sb->append(L"\\n");
					break;
				case L'\0':
					sb->append(L"\\0");
					break;
				case L'\r':
					sb->append(L"\\r");
					break;
				case L'\t':
					sb->append(L"\\t");
					break;
				case L'\\':
					sb->append(L"\\\\");
					break;
				case L';':
				case L'=':
				case L':':
				case L'#':
					sb->append(L'\\');
					sb->append(c);
					break;
				default:
					sb->append(c);
					break;
			}
		}
		return sb->toString();
	}

	std::wstring StringExtensions::Unescape(const std::wstring &str)
	{
		StringBuilder *sb = new StringBuilder();
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] == L'\\')
			{
				i++;
				if (i >= str.length())
				{
					throw InvalidOperationException();
				}
				switch (str[i])
				{
					case L'n':
						sb->append(L'\n');
						break;
					case L'0':
						sb->append(L'\0');
						break;
					case L'r':
						sb->append(L'\r');
						break;
					case L't':
						sb->append(L'\t');
						break;
					case L'\\':
						sb->append(L'\\');
						break;
					case L';':
					case L'=':
					case L':':
					case L'#':
						sb->append(str[i]);
						break;
					default:
						throw InvalidOperationException();
				}
			}
			else
			{
				sb->append(str[i]);
			}
		}

		return sb->toString();
	}
}
