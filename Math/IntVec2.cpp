#include "IntVec2.h"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/StringUtils.hpp"
#include <math.h>

/* COORDS */
// -----------------------------------------------------------------------------
const IntVec2 IntVec2::INVALID(INT_MAX, INT_MAX);
const IntVec2 IntVec2::ZERO(0, 0);
const IntVec2 IntVec2::ONE(1, 1);
const IntVec2 IntVec2::ZERO_TO_ONE(0, 1);
const IntVec2 IntVec2::NORTH(0, 1);
const IntVec2 IntVec2::SOUTH(0, -1);
const IntVec2 IntVec2::EAST(1, 0);
const IntVec2 IntVec2::WEST(-1, 0);
// -----------------------------------------------------------------------------
/* SHEETS/GRIDS */
const IntVec2 IntVec2::GRID2X2(2, 2);
const IntVec2 IntVec2::GRID3X3(3, 3);
const IntVec2 IntVec2::GRID4X4(4, 4);
const IntVec2 IntVec2::GRID5X5(5, 5);
const IntVec2 IntVec2::GRID6X6(6, 6);
const IntVec2 IntVec2::GRID7X7(7, 7);
const IntVec2 IntVec2::GRID8X8(8, 8);
// -----------------------------------------------------------------------------

IntVec2::IntVec2(const IntVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

IntVec2::IntVec2(int initialX, int initialY)
	: x(initialX)
	, y(initialY)
{
}

float IntVec2::GetLength() const
{
	return (sqrtf(static_cast<float>(x * x) + static_cast<float>(y * y)));
}

int IntVec2::GetTaxicabLength() const
{
	int dx = abs(x);
	int dy = abs(y);
	return dx + dy;
}

int IntVec2::GetLengthSquared() const
{
	return static_cast<int>(x * x) + static_cast<int>(y * y);
}

float IntVec2::GetOrientationRadians() const
{
	return atan2f(static_cast<float>(y), static_cast<float>(x));
}

float IntVec2::GetOrientationDegrees() const
{
	return Atan2Degrees(static_cast<float>(y), static_cast<float>(x));
}

IntVec2 const IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y, x);
}

IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}

void IntVec2::SetFromText(char const* text)
{
	Strings commaSplit = SplitStringOnDelimiter(text, ',');
	x = static_cast<int>(atoi(commaSplit[0].c_str()));
	y = static_cast<int>(atoi(commaSplit[1].c_str()));
}

void IntVec2::Rotate90Degrees()
{
	int oldx = x;
	x = -y;
	y = oldx;
}

void IntVec2::RotateMinus90Degrees()
{
	int oldy = y;
	y = -x;
	x = oldy;
}

//-----------------------------------------------------------------------------------------------
IntVec2 const IntVec2::operator + (IntVec2 const& vecToAdd) const
{
	return IntVec2(vecToAdd.x + x, vecToAdd.y + y);
}


//-----------------------------------------------------------------------------------------------
IntVec2 const IntVec2::operator-(IntVec2 const& vecToSubtract) const
{
	return IntVec2(x - vecToSubtract.x, y - vecToSubtract.y);
}


//------------------------------------------------------------------------------------------------
IntVec2 const IntVec2::operator-() const
{
	return IntVec2(-(x), -(y));
}


//-----------------------------------------------------------------------------------------------
IntVec2 const IntVec2::operator*(float uniformScale) const
{
	return IntVec2(x * static_cast<int>(uniformScale), y * static_cast<int>(uniformScale));
}


//------------------------------------------------------------------------------------------------
IntVec2 const IntVec2::operator*(IntVec2 const& vecToMultiply) const
{
	return IntVec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
IntVec2 const IntVec2::operator/(float inverseScale) const
{
	return IntVec2(x / static_cast<int>(inverseScale), y / static_cast<int>(inverseScale));
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator+=(IntVec2 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator-=(IntVec2 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator*=(const float uniformScale)
{
	x *= static_cast<int>(uniformScale);
	y *= static_cast<int>(uniformScale);
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator/=(const float uniformDivisor)
{
	x /= static_cast<int>(uniformDivisor);
	y /= static_cast<int>(uniformDivisor);
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator=(IntVec2 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

//-----------------------------------------------------------------------------------------------
IntVec2 const operator*(float uniformScale, IntVec2 const& vecToScale)
{
	return IntVec2(static_cast<int>(uniformScale) * vecToScale.x, static_cast<int>(uniformScale) * vecToScale.y);
}


//-----------------------------------------------------------------------------------------------
bool IntVec2::operator==(IntVec2 const& compare) const
{
	return x == compare.x and y == compare.y;
}

//-----------------------------------------------------------------------------------------------
bool IntVec2::operator!=(IntVec2 const& compare) const
{
	return x != compare.x or y != compare.y;
}