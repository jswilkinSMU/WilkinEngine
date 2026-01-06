#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/Vec3.h"

Vec4::Vec4(Vec4 const& copyFrom)
	: x(copyFrom.x),
	  y(copyFrom.y),
	  z(copyFrom.z),
	  w(copyFrom.w)
{
}

Vec4::Vec4(float initialX, float initialY, float initialZ, float initialW)
	:x(initialX),
	 y(initialY),
	 z(initialZ),
	 w(initialW)
{
}

Vec4::Vec4(Vec3 initialXYZ, float initialW)
{
	Vec3 positionXYZ = Vec3(x, y, z);
	positionXYZ = initialXYZ;
	w = initialW;
}

bool Vec4::operator==(Vec4 const& compare) const
{
	return x == compare.x and y == compare.y and z == compare.z and w == compare.w;
}

bool Vec4::operator!=(Vec4 const& compare) const
{
	return x != compare.x or y != compare.y or z != compare.z or w != compare.w;
}

Vec4 const Vec4::operator+(Vec4 const& vecToAdd) const
{
	return Vec4(vecToAdd.x + x, vecToAdd.y + y, vecToAdd.z + z, vecToAdd.w + w);
}

Vec4 const Vec4::operator-(Vec4 const& vecToSubtract) const
{
	return Vec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}

Vec4 const Vec4::operator-() const
{
	return Vec4(-(x), -(y), -(z), -(w));
}

Vec4 const Vec4::operator*(float uniformScale) const
{
	return Vec4(x * uniformScale, y * uniformScale, z * uniformScale, w * uniformScale);
}

Vec4 const Vec4::operator*(Vec4 const& vecToMultiply) const
{
	return Vec4(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z, w * vecToMultiply.w);
}

Vec4 const Vec4::operator/(float inverseScale) const
{
	return Vec4(x / inverseScale, y / inverseScale, z / inverseScale, w / inverseScale);
}

void Vec4::operator+=(Vec4 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}

void Vec4::operator-=(Vec4 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}

void Vec4::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}

void Vec4::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
	w /= uniformDivisor;
}

void Vec4::operator=(Vec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}
