#pragma once

struct FloatRange
{
	// static const declarations
	static const FloatRange ZERO;
	static const FloatRange ONE;
	static const FloatRange ZERO_TO_ONE;

public:
	float m_min = 0.0f;
	float m_max = 0.0f;

public:
	FloatRange();
	explicit FloatRange(float minimum, float maximum);

public:
	// Basic Methods for FloatRange
	bool		operator==(FloatRange const& compare) const;
	bool		operator!=(FloatRange const& compare) const;
	void		operator=(FloatRange const& copyFrom);
	
	bool isOnRange(float inRange) const;
	bool isOverLappingWith(FloatRange const& overlappingRange) const;
	void StretchToIncludeValue(float value);
	void SetFromText(char const* text);
};