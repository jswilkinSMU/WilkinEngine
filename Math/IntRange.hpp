#pragma once

struct IntRange
{
	// static const declarations
	static const IntRange ZERO;
	static const IntRange ONE;
	static const IntRange ZERO_TO_ONE;

public:
	int m_min = 0;
	int m_max = 0;

public:
	IntRange();
	explicit IntRange(int minimum, int maximum);

public:
	// Basic Methods for IntRange
	bool		operator==(IntRange const& compare) const;
	bool		operator!=(IntRange const& compare) const;
	void		operator=(IntRange const& copyFrom);

	bool isOnRange(int inRange) const;
	bool isOverLappingWith(IntRange const& overlappingRange) const;
};