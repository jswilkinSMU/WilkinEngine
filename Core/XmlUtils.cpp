#include "Engine/Core/XmlUtils.hpp"

int ParseXmlAttribute(XmlElement const& element, char const* attributeName, int defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	return atoi(attributeValue);
}

char ParseXmlAttribute(XmlElement const& element, char const* attributeName, char defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	return *attributeValue;
}

bool ParseXmlAttribute(XmlElement const& element, char const* attributeName, bool defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	std::string stringName(attributeValue);

	if (stringName == "true")
	{
		return true;
	}
	if (stringName == "false")
	{
		return false;
	}
	return defaultValue;
}

float ParseXmlAttribute(XmlElement const& element, char const* attributeName, float defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	return static_cast<float>(atof(attributeValue));
}

Rgba8 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Rgba8 const& defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	Rgba8 colorValue;
	colorValue.SetFromText(attributeValue);
	return colorValue;
}

Vec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec2 const& defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	Vec2 valueXY;
	valueXY.SetFromText(attributeValue);
	return valueXY;
}

Vec3 ParseXmlAttribute(XmlElement const& element, char const* attributeName, Vec3 const& defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	Vec3 valueXYZ;
	valueXYZ.SetFromText(attributeValue);
	return valueXYZ;
}

EulerAngles ParseXmlAttribute(XmlElement const& element, char const* attributeName, EulerAngles const& defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	EulerAngles valueYPR;
	valueYPR.SetFromText(attributeValue);
	return valueYPR;
}

FloatRange ParseXmlAttribute(XmlElement const& element, const char* attributeName, FloatRange const& defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	FloatRange valueMinMax;
	valueMinMax.SetFromText(attributeValue);
	return valueMinMax;
}

IntVec2 ParseXmlAttribute(XmlElement const& element, char const* attributeName, IntVec2 const& defaultValue)
{
	char const* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	IntVec2 valueXY;
	valueXY.SetFromText(attributeValue);
	return valueXY;
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, std::string const& defaultValue)
{
	const char* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValue;
	}
	return std::string(attributeValue);
}

Strings ParseXmlAttribute(XmlElement const& element, char const* attributeName, Strings const& defaultValues)
{
	const char* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return defaultValues;
	}
	return SplitStringOnDelimiter(attributeValue, ' ');
}

std::string ParseXmlAttribute(XmlElement const& element, char const* attributeName, char const* defaultValue)
{
	const char* attributeValue = element.Attribute(attributeName);
	if (attributeValue == nullptr)
	{
		return std::string(defaultValue);
	}
	return std::string(attributeValue);
}
