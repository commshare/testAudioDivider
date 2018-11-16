#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>


namespace AudioDivider
{
	class IniConfiguration
	{
	private:
		class Property
		{
		public:
			std::wstring name;
			std::wstring value;
			Property(const std::wstring &name, const std::wstring &value);
		};

	private:
		class Section
		{
		public:
			std::wstring name;
			std::vector<Property*> properties;
			Section(const std::wstring &name);

			Property *TryGetPropery(const std::wstring &name);

		};

	private:
		std::vector<Section*> sections;

	public:
		IniConfiguration();

	private:
		Section *TryGetSection(const std::wstring &name);

	public:
		void SetValueString(const std::wstring &nameSection, const std::wstring &nameProperty, const std::wstring &value);

		std::wstring GetValueString(const std::wstring &nameSection, const std::wstring &nameProperty);

		void SetValueInt(const std::wstring &nameSection, const std::wstring &nameProperty, int value);

		int GetValueInt(const std::wstring &nameSection, const std::wstring &nameProperty, int defaultValue);

		void SetValueBool(const std::wstring &nameSection, const std::wstring &nameProperty, bool value);

		bool GetValueBool(const std::wstring &nameSection, const std::wstring &nameProperty, bool defaultValue);

		void Save(Stream *stream);

		void Load(const std::wstring &path);

		void Save(const std::wstring &path);

		void Load(Stream *stream);
	};
}
