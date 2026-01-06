#include "NamedStrings.hpp"

void NamedStrings::PopulateFromXmlElementAttributes(XmlElement const& element)
{
	XmlAttribute const* attribute = element.FirstAttribute();
	while (attribute)
	{
		SetValue(attribute->Name(), attribute->Value());
		attribute = attribute->Next();
	}
}

void NamedStrings::SetValue(std::string const& keyName, std::string const& newValue)
{
	m_keyValuePairs[keyName] = newValue;
}

std::string NamedStrings::GetValue(std::string const& keyName, std::string const& defaultValue) const
{
	// May change to auto later once I can tell for certain this is a const_iterator
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	return found->second;
}

bool NamedStrings::GetValue(std::string const& keyName, bool defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	std::string const& stringValue = found->second;

	if (stringValue == "true")
	{
		return true;
	}
	if (stringValue == "false")
	{
		return false;
	}
	return defaultValue;
}

int NamedStrings::GetValue(std::string const& keyName, int defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	return atoi(found->second.c_str());
}

float NamedStrings::GetValue(std::string const& keyName, float defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	return static_cast<float>(atof(found->second.c_str()));
}

std::string NamedStrings::GetValue(std::string const& keyName, char const* defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	return std::string(found->second);
}

Rgba8 NamedStrings::GetValue(std::string const& keyName, Rgba8 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	Rgba8 colorValue;
	colorValue.SetFromText(found->second.c_str());
	return colorValue;
}

Vec2 NamedStrings::GetValue(std::string const& keyName, Vec2 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	Vec2 valueXY;
	valueXY.SetFromText(found->second.c_str());
	return valueXY;
}

IntVec2 NamedStrings::GetValue(std::string const& keyName, IntVec2 const& defaultValue) const
{
	std::map<std::string, std::string>::const_iterator found = m_keyValuePairs.find(keyName);
	if (found == m_keyValuePairs.end())
	{
		return defaultValue;
	}
	IntVec2 valueXY;
	valueXY.SetFromText(found->second.c_str());
	return valueXY;
}

std::map<std::string, std::string> NamedStrings::GetKeyValuePairs() const
{
	return m_keyValuePairs;
}
