#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/StringUtils.hpp"
#include <math.h>

const Vec2 Vec2::ZERO(0.f, 0.f);
const Vec2 Vec2::ONE(1.f, 1.f);
const Vec2 Vec2::ONEHALF(0.5f, 0.5f);
const Vec2 Vec2::ZERO_TO_ONE(0.f, 1.f);
const Vec2 Vec2::ONE_TO_ZERO(1.f, 0.f);

//-----------------------------------------------------------------------------------------------
Vec2::Vec2( Vec2 const& copy )
	: x(copy.x)
	, y(copy.y)
{
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( float initialX, float initialY )
	: x(initialX)
	, y(initialY)
{
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator + ( Vec2 const& vecToAdd ) const
{
	return Vec2(vecToAdd.x + x,vecToAdd.y + y);
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator-( Vec2 const& vecToSubtract ) const
{
	return Vec2(x - vecToSubtract.x, y - vecToSubtract.y);
}


//------------------------------------------------------------------------------------------------
Vec2 const Vec2::operator-() const
{
	return Vec2(-(x), -(y));
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator*( float uniformScale ) const
{
	return Vec2(x * uniformScale, y * uniformScale);
}


//------------------------------------------------------------------------------------------------
Vec2 const Vec2::operator*( Vec2 const& vecToMultiply ) const
{
	return Vec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator/( float inverseScale ) const
{
	return Vec2(x / inverseScale, y / inverseScale);
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=( Vec2 const& vecToAdd )
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=( Vec2 const& vecToSubtract )
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=( const float uniformScale )
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=( const float uniformDivisor )
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=( Vec2 const& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
}

Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float distance)
{
	float tempx = distance * cosf(orientationRadians);
	float tempy = distance * sinf(orientationRadians);
	return Vec2(tempx, tempy);
}

Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float distance)
{
	float tempx = distance * CosDegrees(orientationDegrees);
	float tempy = distance * SinDegrees(orientationDegrees);
	return Vec2(tempx, tempy);
}


//-----------------------------------------------------------------------------------------------
Vec2 const operator*( float uniformScale, Vec2 const& vecToScale )
{
	return Vec2(uniformScale * vecToScale.x, uniformScale * vecToScale.y);
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==( Vec2 const& compare ) const
{
	return x == compare.x and y == compare.y;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=(Vec2 const& compare) const
{
	return x != compare.x or y != compare.y;
}

float Vec2::GetLength() const
{
	return sqrtf((x*x)+(y*y));
}

float Vec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}

float Vec2::GetOrientationRadians() const
{
	return atan2f(y,x);
}

float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y, x);
}

Vec2 const Vec2::GetRotated90Degrees() const
{
	return Vec2(-y, x);
}

Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2(y, -x);
}

Vec2 const Vec2::GetRotatedRadians(float deltaRadians) const
{
	float tempx = x * cosf(deltaRadians) - y * sinf(deltaRadians);
	float tempy = x * sinf(deltaRadians) + y * cosf(deltaRadians);
	return Vec2(tempx, tempy);
}

Vec2 const Vec2::GetRotatedDegrees(float deltaDegrees) const
{
	float tempx = x * CosDegrees(deltaDegrees) - y * SinDegrees(deltaDegrees);
	float tempy = x * SinDegrees(deltaDegrees) + y * CosDegrees(deltaDegrees);
	return Vec2(tempx, tempy);
}

void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float length = sqrtf((x * x) + (y * y));
	x = length * cosf(newOrientationRadians);
	y = length * sinf(newOrientationRadians);
}

void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float length = sqrtf((x * x) + (y * y));
	x = length * CosDegrees(newOrientationDegrees);
	y = length * SinDegrees(newOrientationDegrees);
}

void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	x = newLength * cosf(newOrientationRadians);
	y = newLength * sinf(newOrientationRadians);
}

void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	x = newLength * CosDegrees(newOrientationDegrees);
	y = newLength * SinDegrees(newOrientationDegrees);
}

void Vec2::Rotate90Degrees()
{
	float oldx = x;
	x = -y;
	y = oldx;
}

void Vec2::RotateMinus90Degrees()
{
	float oldy = y;
	y = -x;
	x = oldy;
}

void Vec2::RotateRadians(float deltaRadians)
{
	float tempx = x * cosf(deltaRadians) - y * sinf(deltaRadians);
	float tempy = x * sinf(deltaRadians) + y * cosf(deltaRadians);
	x = tempx;
	y = tempy;
}

void Vec2::RotateDegrees(float deltaDegrees)
{
	float tempx = x * CosDegrees(deltaDegrees) - y * SinDegrees(deltaDegrees);
	float tempy = x * SinDegrees(deltaDegrees) + y * CosDegrees(deltaDegrees);
	x = tempx;
	y = tempy;
}

Vec2 const Vec2::GetClamped(float maxLength) const
{
	if (GetLength() >= maxLength) {
		float scaleDown = maxLength / GetLength();
		return Vec2(x * scaleDown, y * scaleDown);
	}
	return Vec2(x, y);
}

Vec2 const Vec2::GetNormalized() const
{
	if (GetLength() > 0) {
		return Vec2(x / GetLength(), y / GetLength());
	}
	return Vec2(0, 0);
}

Vec2 const Vec2::GetReflected(Vec2 const& normalOfSurfaceToReflectOffOf) const
{
	Vec2 normalizedSurface = normalOfSurfaceToReflectOffOf.GetNormalized();

	float reflection = DotProduct2D(Vec2(x, y), normalOfSurfaceToReflectOffOf);
	Vec2 reflected = Vec2(x,y) - normalizedSurface * (2.0f * reflection);
	return reflected;
}

Vec3 const Vec2::GetAsVec3(float z) const
{
	return Vec3(x, y, z);
}

void Vec2::SetLength(float newlength)
{
	Normalize();
	x *= newlength;
	y *= newlength;
}

void Vec2::SetFromText(char const* text)
{
	// *Ask in office hours Tuesday about parsing whitespace*
	Strings commaSplit = SplitStringOnDelimiter(text, ',');
	x = static_cast<float>(atof(commaSplit[0].c_str()));
	y = static_cast<float>(atof(commaSplit[1].c_str()));
}

void Vec2::ClampLength(float maxLength)
{
	if (GetLength() >= maxLength) {
		float scaleDown = maxLength / GetLength();
		x *= scaleDown;
		y *= scaleDown;
	}
}

void Vec2::Normalize()
{
	float length = GetLength();

	if (length > 0) {
		x /= length;
		y /= length;
	}
}

void Vec2::Reflect(Vec2 const& normalOfSurfaceToReflectOffOf)
{
	Vec2 normalizedSurface = normalOfSurfaceToReflectOffOf.GetNormalized();

	float reflection = DotProduct2D(Vec2(x, y), normalOfSurfaceToReflectOffOf);
	*this = Vec2(x, y) - normalizedSurface * (2.0f * reflection);
}

float Vec2::NormalizeAndGetPreviousLength()
{
	float length = GetLength();

	if (length > 0) {
		x /= length;
		y /= length;
	}
	return length;
}

bool Vec2::IsNearlyZero(float nearlyZeroValue) const
{
	float nearlyZeroValSquared = nearlyZeroValue * nearlyZeroValue;
	return GetLengthSquared() < nearlyZeroValSquared;
}


