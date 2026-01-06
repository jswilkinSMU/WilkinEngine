#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/StringUtils.hpp"

const FloatRange FloatRange::ZERO(0.0f, 0.0f);
const FloatRange FloatRange::ONE(1.0f, 1.0f);
const FloatRange FloatRange::ZERO_TO_ONE(0.0f, 1.0f);

FloatRange::FloatRange()
	: m_min(0.0f),
	  m_max(0.0f)
{
}

FloatRange::FloatRange(float minimum, float maximum)
	: m_min(minimum),
	  m_max(maximum)
{
}

bool FloatRange::operator==(FloatRange const& compare) const
{
	return m_min == compare.m_min && m_max == compare.m_max;
}

bool FloatRange::operator!=(FloatRange const& compare) const
{
	return m_min != compare.m_min or m_max != compare.m_max;
}

void FloatRange::operator=(FloatRange const& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

bool FloatRange::isOnRange(float inRange) const
{
	return inRange >= m_min && inRange <= m_max;
}

bool FloatRange::isOverLappingWith(FloatRange const& overlappingRange) const
{
	return m_min <= overlappingRange.m_max && m_max >= overlappingRange.m_min;
}

void FloatRange::StretchToIncludeValue(float value)
{
	if (value < m_min)
	{
		m_min = value;
	}
	if (value > m_max)
	{
		m_max = value;
	}
}

void FloatRange::SetFromText(char const* text)
{
	Strings tildeSplit = SplitStringOnDelimiter(text, '~');
	m_min = static_cast<float>(atof(tildeSplit[0].c_str()));
	m_max = static_cast<float>(atof(tildeSplit[1].c_str()));
}
