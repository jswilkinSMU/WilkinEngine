#include "IntRange.hpp"

const IntRange IntRange::ZERO(0, 0);
const IntRange IntRange::ONE(1, 1);
const IntRange IntRange::ZERO_TO_ONE(0, 1);

IntRange::IntRange()
	:m_min(0),
	 m_max(0)
{
}

IntRange::IntRange(int minimum, int maximum)
	:m_min(minimum),
	 m_max(maximum)
{
}

bool IntRange::operator==(IntRange const& compare) const
{
	return m_min == compare.m_min && m_max == compare.m_max;
}

bool IntRange::operator!=(IntRange const& compare) const
{
	return m_min != compare.m_min or m_max != compare.m_max;
}

void IntRange::operator=(IntRange const& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

bool IntRange::isOnRange(int inRange) const
{
	return inRange >= m_min && inRange <= m_max;
}

bool IntRange::isOverLappingWith(IntRange const& overlappingRange) const
{
	return m_min <= overlappingRange.m_max && m_max >= overlappingRange.m_min;
}
