#pragma once
// -----------------------------------------------------------------------------
struct IntVec3
{
	// static const declarations
	static const IntVec3 INVALID;
	static const IntVec3 ZERO;
	static const IntVec3 ONE;
	static const IntVec3 EAST;
	static const IntVec3 WEST;
	static const IntVec3 NORTH;
	static const IntVec3 SOUTH;
	static const IntVec3 TOP;
	static const IntVec3 BOTTOM;

public:
	int x = 0;
	int y = 0;
	int z = 0;

public:
	IntVec3() {}
	~IntVec3() {}
	IntVec3(IntVec3 const& copyFrom);
	explicit IntVec3(int initialX, int initialY, int initialZ);

	// Accessors
	float GetLength() const;
	int GetTaxicabLength() const;
	int GetLengthSquared() const;

	// Mutators
	void  SetFromText(char const* text);

	// Operators
	bool		operator==(IntVec3 const& compare) const;
	bool		operator!=(IntVec3 const& compare) const;
	IntVec3 const	operator+(IntVec3 const& vecToAdd) const;
	IntVec3 const	operator-(IntVec3 const& vecToSubtract) const;
	IntVec3 const	operator-() const;
	IntVec3 const	operator*(float uniformScale) const;
	IntVec3 const	operator*(IntVec3 const& vecToMultiply) const;
	IntVec3 const	operator/(float inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(IntVec3 const& vecToAdd);
	void		operator-=(IntVec3 const& vecToSubtract);
	void		operator*=(const float uniformScale);
	void		operator/=(const float uniformDivisor);
	void		operator=(IntVec3 const& copyFrom);
};