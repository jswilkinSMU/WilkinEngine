#pragma once

struct IntVec2
{
	// static const declarations
	static const IntVec2 INVALID;
	static const IntVec2 ZERO;
	static const IntVec2 ONE;
	static const IntVec2 ZERO_TO_ONE;
	static const IntVec2 EAST;
	static const IntVec2 WEST;
	static const IntVec2 NORTH;
	static const IntVec2 SOUTH;
	static const IntVec2 GRID2X2;
	static const IntVec2 GRID3X3;
	static const IntVec2 GRID4X4;
	static const IntVec2 GRID5X5;
	static const IntVec2 GRID6X6;
	static const IntVec2 GRID7X7;
	static const IntVec2 GRID8X8;

public:
	int x = 0;
	int y = 0;

public:
	IntVec2() {}
	~IntVec2() {}
	IntVec2(const IntVec2& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	// Accessors
	float GetLength() const;
	int GetTaxicabLength() const;
	int GetLengthSquared() const;
	float GetOrientationRadians() const;
	float GetOrientationDegrees() const;
	IntVec2 const GetRotated90Degrees() const;
	IntVec2 const GetRotatedMinus90Degrees() const;

	// Mutators
	void  SetFromText(char const* text);
	void Rotate90Degrees();
	void RotateMinus90Degrees();

	// Operators
	bool		operator==(IntVec2 const& compare) const;	
	bool		operator!=(IntVec2 const& compare) const;		
	IntVec2 const	operator+(IntVec2 const& vecToAdd) const;		
	IntVec2 const	operator-(IntVec2 const& vecToSubtract) const;
	IntVec2 const	operator-() const;								
	IntVec2 const	operator*(float uniformScale) const;			
	IntVec2 const	operator*(IntVec2 const& vecToMultiply) const;	
	IntVec2 const	operator/(float inverseScale) const;		

	// Operators (self-mutating / non-const)
	void		operator+=(IntVec2 const& vecToAdd);				
	void		operator-=(IntVec2 const& vecToSubtract);		
	void		operator*=(const float uniformScale);			
	void		operator/=(const float uniformDivisor);		
	void		operator=(IntVec2 const& copyFrom);				
};
