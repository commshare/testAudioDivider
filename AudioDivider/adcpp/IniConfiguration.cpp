#include "IniConfiguration.h"
#include "StringExtensions.h"


namespace AudioDivider
{

	IniConfiguration::Property::Property(const std::wstring &name, const std::wstring &value)
	{
		this->name = name;
		this->value = value;
	}

	IniConfiguration::Section::Section(const std::wstring &name)
	{
		this->name = name;
	}

	Property *IniConfiguration::Section::TryGetPropery(const std::wstring &name)
	{
		for (auto property_Renamed : properties)
		{
			if (property_Renamed->name == name)
			{
				return property_Renamed;
			}
		}
		return nullptr;
	}

	IniConfiguration::IniConfiguration()
	{

	}

	IniConfiguration::Section *IniConfiguration::TryGetSection(const std::wstring &name)
	{
		for (auto section : sections)
		{
			if (section->name == name)
			{
				return section;
			}
		}
		return nullptr;
	}

	void IniConfiguration::SetValueString(const std::wstring &nameSection, const std::wstring &nameProperty, const std::wstring &value)
	{
		Section *section = TryGetSection(nameSection);
		if (section == nullptr)
		{
			section = new Section(nameSection);
			sections.push_back(section);
		}

		Property *property_Renamed = section->TryGetPropery(nameProperty);
		if (property_Renamed == nullptr)
		{
			property_Renamed = new Property(nameProperty, value);
			section->properties.push_back(property_Renamed);
		}
		else
		{
			property_Renamed->value = value;
		}
	}

	std::wstring IniConfiguration::GetValueString(const std::wstring &nameSection, const std::wstring &nameProperty)
	{
		Section *section = TryGetSection(nameSection);
		if (section == nullptr)
		{
			return L"";
		}

		Property *property_Renamed = section->TryGetPropery(nameProperty);
		if (property_Renamed == nullptr)
		{
			return L"";
		}

		return property_Renamed->value;
	}

	void IniConfiguration::SetValueInt(const std::wstring &nameSection, const std::wstring &nameProperty, int value)
	{
		SetValueString(nameSection, nameProperty, std::to_wstring(value));
	}

	int IniConfiguration::GetValueInt(const std::wstring &nameSection, const std::wstring &nameProperty, int defaultValue)
	{
		std::wstring value = GetValueString(nameSection, nameProperty);

		if (value == L"")
		{
			return defaultValue;
		}

		int valueInt;
		if (int::TryParse(value, valueInt))
		{
			return valueInt;
		}
		else
		{
			return defaultValue;
		}
	}

	void IniConfiguration::SetValueBool(const std::wstring &nameSection, const std::wstring &nameProperty, bool value)
	{
		SetValueString(nameSection, nameProperty, value ? L"true" : L"false");
	}

	bool IniConfiguration::GetValueBool(const std::wstring &nameSection, const std::wstring &nameProperty, bool defaultValue)
	{
		 std::wstring value = GetValueString(nameSection, nameProperty);
		 if (value == L"")
		 {
			 return defaultValue;
		 }

		 bool valueBool;
		 if (bool::TryParse(value, valueBool))
		 {
			 return valueBool;
		 }
		 else
		 {
			 return defaultValue;
		 }
	}

	void IniConfiguration::Save(Stream *stream)
	{
		StreamWriter *sw = new StreamWriter(stream, Encoding::UTF8);

		for (auto section : sections)
		{
			sw->WriteLine(L"[" + section->name + L"]");
			for (auto property_Renamed : section->properties)
			{
				sw->Write(StringExtensions::Escape(property_Renamed->name));
				sw->Write(L"=");
				sw->Write(StringExtensions::Escape(property_Renamed->value));
				sw->WriteLine();
			}
		}
		sw->Flush();
	}

	void IniConfiguration::Load(const std::wstring &path)
	{
//C# TO C++ CONVERTER NOTE: The following 'using' block is replaced by its C++ equivalent:
//ORIGINAL LINE: using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read))
		{
			FileStream *fs = new FileStream(path, FileMode::Open, FileAccess::Read);
			Load(fs);
			delete fs;
		}
	}

	void IniConfiguration::Save(const std::wstring &path)
	{
//C# TO C++ CONVERTER NOTE: The following 'using' block is replaced by its C++ equivalent:
//ORIGINAL LINE: using (FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write))
		{
			FileStream *fs = new FileStream(path, FileMode::Create, FileAccess::Write);
			Save(fs);
			delete fs;
		}
	}

	void IniConfiguration::Load(Stream *stream)
	{
		sections.clear();
		StreamReader *sr = new StreamReader(stream, Encoding::UTF8);

		Section *currentSection = nullptr;
		while (sr->Peek() >= 0)
		{
			std::wstring line = sr->ReadLine();
			if (boost::starts_with(line, L"["))
			{
				line = line.substr(1, line.length() - 2);
				currentSection = new Section(line);
				sections.push_back(currentSection);
			}
			else
			{
				bool found = false;
				int middle = 0;
				while (!found)
				{
					middle = (int)line.find(L'=', middle);
					if (line[middle - 1] != L'\\')
					{
						found = true;
					}
				}
				std::wstring propertyName = line.substr(0, middle);
				std::wstring propertyValue = line.substr(middle + 1, line.length() - (middle + 1));
				Property *property_Renamed = new Property(StringExtensions::Unescape(propertyName), StringExtensions::Unescape(propertyValue));
				currentSection->properties.push_back(property_Renamed);
			}
		}
	}
}
