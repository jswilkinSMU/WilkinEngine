#include "Engine/Math/IntVec3.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <cmath>

IntVec3 const IntVec3::INVALID(INT_MAX, INT_MAX, INT_MAX);
IntVec3 const IntVec3::ZERO(0, 0, 0);
IntVec3 const IntVec3::ONE(1, 1, 1);
IntVec3 const IntVec3::EAST(1, 0, 0);
IntVec3 const IntVec3::WEST(-1, 0, 0);
IntVec3 const IntVec3::NORTH(0, 1, 0);
IntVec3 const IntVec3::SOUTH(0, -1, 0);
IntVec3 const IntVec3::TOP(0, 0, 1);
IntVec3 const IntVec3::BOTTOM(0, 0, -1);

IntVec3::IntVec3(IntVec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

IntVec3::IntVec3(int initialX, int initialY, int initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}

float IntVec3::GetLength() const
{
	return (sqrtf(static_cast<float>(x * x) + static_cast<float>(y * y) + static_cast<float>(z * z)));
}

int IntVec3::GetTaxicabLength() const
{
	int dx = abs(x);
	int dy = abs(y);
	int dz = abs(z);
	return dx + dy + dz;
}

int IntVec3::GetLengthSquared() const
{
	return static_cast<int>(x * x) + static_cast<int>(y * y) + static_cast<int>(z * z);
}

void IntVec3::SetFromText(char const* text)
{
	Strings commaSplit = SplitStringOnDelimiter(text, ',');
	x = static_cast<int>(atoi(commaSplit[0].c_str()));
	y = static_cast<int>(atoi(commaSplit[1].c_str()));
	z = static_cast<int>(atoi(commaSplit[2].c_str()));
}

bool IntVec3::operator==(IntVec3 const& compare) const
{
	return x == compare.x && y == compare.y && z == compare.z;
}

bool IntVec3::operator!=(IntVec3 const& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z;
}

IntVec3 const IntVec3::operator+(IntVec3 const& vecToAdd) const
{
	return IntVec3(vecToAdd.x + x, vecToAdd.y + y, vecToAdd.z + z);
}

IntVec3 const IntVec3::operator-(IntVec3 const& vecToSubtract) const
{
	return IntVec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

IntVec3 const IntVec3::operator-() const
{
	return IntVec3(-(x), -(y), -(z));
}

IntVec3 const IntVec3::operator*(float uniformScale) const
{
	return IntVec3(x * static_cast<int>(uniformScale), y * static_cast<int>(uniformScale), z * static_cast<int>(uniformScale));
}

IntVec3 const IntVec3::operator*(IntVec3 const& vecToMultiply) const
{
	return IntVec3(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z);
}

IntVec3 const IntVec3::operator/(float inverseScale) const
{
	return IntVec3(x / static_cast<int>(inverseScale), y / static_cast<int>(inverseScale), z / static_cast<int>(inverseScale));
}

void IntVec3::operator+=(IntVec3 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

void IntVec3::operator-=(IntVec3 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

void IntVec3::operator*=(const float uniformScale)
{
	x *= static_cast<int>(uniformScale);
	y *= static_cast<int>(uniformScale);
	z *= static_cast<int>(uniformScale);
}

void IntVec3::operator/=(const float uniformDivisor)
{
	x /= static_cast<int>(uniformDivisor);
	y /= static_cast<int>(uniformDivisor);
	z /= static_cast<int>(uniformDivisor);
}

void IntVec3::operator=(IntVec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

