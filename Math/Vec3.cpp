#include "Engine/Math/Vec3.h"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/StringUtils.hpp"
#include <math.h>

const Vec3 Vec3::ZERO(0.f, 0.f, 0.f);
const Vec3 Vec3::ONE(1.f, 1.f, 1.f);
const Vec3 Vec3::ONE_HALF(0.5f, 0.5f, 0.5f);
const Vec3 Vec3::XAXE(1.f, 0.f, 0.f);
const Vec3 Vec3::YAXE(0.f, 1.f, 0.f);
const Vec3 Vec3::ZAXE(0.f, 0.f, 1.f);
//-----------------------------------------------------------------------------------------------
Vec3::Vec3(Vec3 const& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
{
}

Vec3::Vec3(float initialX, float initialY, float initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}

//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator + (Vec3 const& vecToAdd) const
{
	return Vec3(vecToAdd.x + x, vecToAdd.y + y, vecToAdd.z + z);
}


//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator-(Vec3 const& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}


//------------------------------------------------------------------------------------------------
Vec3 const Vec3::operator-() const
{
	return Vec3(-(x), -(y), -(z));
}


//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator*(float uniformScale) const
{
	return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
}


//------------------------------------------------------------------------------------------------
Vec3 const Vec3::operator*(Vec3 const& vecToMultiply) const
{
	return Vec3(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z);
}


//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator/(float inverseScale) const
{
	return Vec3(x / inverseScale, y / inverseScale, z / inverseScale);
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator+=(Vec3 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator-=(Vec3 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator=(Vec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

Vec3 const Vec3::MakeFromPolarRadians(float pitchRadians, float yawRadians, float length)
{
	float tempX = length * cosf(pitchRadians) * cosf(yawRadians);
	float tempY = length * cosf(pitchRadians) * sinf(yawRadians);
	float tempZ = length * -sinf(pitchRadians);
	return Vec3(tempX, tempY, tempZ);
}

Vec3 const Vec3::MakeFromPolarDegrees(float pitchDegrees, float yawDegrees, float length)
{
	float tempX = length * CosDegrees(pitchDegrees) * CosDegrees(yawDegrees);
	float tempY = length * CosDegrees(pitchDegrees) * SinDegrees(yawDegrees);
	float tempZ = length * -SinDegrees(pitchDegrees);
	return Vec3(tempX, tempY, tempZ);
}

float Vec3::GetLength() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vec3::GetLengthXY() const
{
	return sqrtf((x * x) + (y * y));
}

float Vec3::GetLengthSquared() const
{
	return ((x * x) + (y * y) + (z * z));
}

float Vec3::GetLengthXYSquared() const
{
	return ((x * x) + (y * y));
}

float Vec3::GetAngleAboutZRadians() const
{
	return atan2f(y, x);
}

float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}

bool Vec3::IsNearlyZero(float nearlyZeroValue) const
{
	float nearlyZeroValSquared = nearlyZeroValue * nearlyZeroValue;
	return GetLengthSquared() < nearlyZeroValSquared;
}

Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{
	float tempx = x * cosf(deltaRadians) - y * sinf(deltaRadians);
	float tempy = x * sinf(deltaRadians) + y * cosf(deltaRadians);
	float tempz = z;
	return Vec3(tempx, tempy, tempz);
}

Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{
	float tempx = x * CosDegrees(deltaDegrees) - y * SinDegrees(deltaDegrees);
	float tempy = x * SinDegrees(deltaDegrees) + y * CosDegrees(deltaDegrees);
	float tempz = z;
	return Vec3(tempx, tempy, tempz);
}

Vec3 const Vec3::GetNormalized() const
{
	float length = GetLength();

	if (length > 0)
	{
		Vec3 normalizedVector = Vec3(x / length, y / length, z / length);
		return normalizedVector;
	}
	return Vec3::ZERO;
}

Vec2 const Vec3::GetXY() const
{
	return Vec2(x, y);
}

Vec3 Vec3::GetAbs() const
{
	return Vec3(fabsf(x), fabsf(y), fabsf(z));
}

void Vec3::Normalize()
{
	float length = GetLength();

	if (length > 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}
}

void Vec3::SetFromText(char const* text)
{
	Strings commaSplit = SplitStringOnDelimiter(text, ',');
	x = static_cast<float>(atof(commaSplit[0].c_str()));
	y = static_cast<float>(atof(commaSplit[1].c_str()));
	z = static_cast<float>(atof(commaSplit[2].c_str()));
}


//-----------------------------------------------------------------------------------------------
Vec3 const operator*(float uniformScale, Vec3 const& vecToScale)
{
	return Vec3(uniformScale * vecToScale.x, uniformScale * vecToScale.y, uniformScale * vecToScale.z);
}


//-----------------------------------------------------------------------------------------------
bool Vec3::operator==(Vec3 const& compare) const
{
	return x == compare.x and y == compare.y and z == compare.z;
}


//-----------------------------------------------------------------------------------------------
bool Vec3::operator!=(Vec3 const& compare) const
{
	return x != compare.x or y != compare.y or z != compare.z;
}