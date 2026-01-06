#include "Engine/Math/MathUtils.h"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/EngineCommon.h"
#include <cmath>
// -----------------------------------------------------------------------------

float GetClampedZeroToOne(float value)
{
	return GetClamped(value, 0.0f, 1.0f);
}

float SoftClamp(float value, float minValue, float maxValue, float stiffness)
{
	if (value < minValue)
	{
		value = Interpolate(value, minValue, stiffness);
	}
	else if (value > maxValue)
	{
		value = Interpolate(value, minValue, stiffness);
	}
	return value;
}

float Interpolate(float start, float end, float fractionTowardEnd)
{
	float lerp = start + (end - start) * fractionTowardEnd;
	return lerp;
}

Vec2 Interpolate(Vec2 startPos, Vec2 endPos, float fractionTowardEnd)
{
	return startPos + (endPos - startPos) * fractionTowardEnd;
}

Vec3 Interpolate(Vec3 startPos, Vec3 endPos, float fractionTowardEnd)
{
	return startPos + (endPos - startPos) * fractionTowardEnd;
}

Vec4 Interpolate(Vec4 startPos, Vec4 endPos, float fractionTowardEnd)
{
	return startPos + (endPos - startPos) * fractionTowardEnd;
}

Quat Interpolate(Quat startPos, Quat endPos, float fractionTowardEnd)
{
	return startPos + (endPos - startPos) * fractionTowardEnd;
}

Quat SLerp(Quat startQuat, Quat endQuat, float fractionTowardEnd)
{
	float dotQuat = startQuat.DotQuat(endQuat);

	// Check for if our dotted quat is less than 1
	// If it is, negate end quaternion
	if (dotQuat < 0.f)
	{
		dotQuat = -dotQuat;
		endQuat = endQuat.NegateQuat();
	}

	// Check if our dotted quat is close to one
	// If it is use lerp since they're similar
	if (dotQuat > 0.999f)
	{
		return Interpolate(startQuat, endQuat, fractionTowardEnd).GetNormalized();
	}

	float t0 = acosf(dotQuat);
	float t1 = t0 * fractionTowardEnd;
	float s0 = sinf(t0);
	float s1 = sinf(t1);

	float q0 = cosf(t1) - dotQuat * s1 / s0;
	float q1 = s1 / s0;
	Quat  end = (startQuat * q0 + endQuat * q1).GetNormalized();
	return end;
}

Vec3 SLerp(Vec3 startDir, Vec3 endDir, float fractionTowardEnd)
{
	float directionDot = DotProduct3D(startDir.GetNormalized(), endDir.GetNormalized());
	directionDot = GetClamped(directionDot, -1.f, 1.f);

	float theta = acosf(directionDot) * fractionTowardEnd;
	Vec3 relativeVector = (endDir - startDir * directionDot).GetNormalized();

	return ((startDir * cosf(theta)) + (relativeVector * sinf(theta))).GetNormalized();
}

float NormalizeByte(unsigned char byteValue)
{
	return static_cast<float>(byteValue) / 255.f;
}

unsigned char DenormalizeByte(float zeroToOne)
{
	float evenScale = zeroToOne * 256.f;
	if (evenScale >= 256.f)
	{
		return static_cast<unsigned char>(255.f);
	}
	return static_cast<unsigned char>(evenScale);
}

float GetFractionWithinRange(float value, float rangeStart, float rangeEnd)
{
	float t = (value - rangeStart) / (rangeEnd - rangeStart);
	return Interpolate(rangeStart, rangeEnd, t);
}

float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float t = (inValue - inStart) / (inEnd - inStart);
	return Interpolate(outStart, outEnd, t);
}

float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float t = (inValue - inStart) / (inEnd - inStart);
	t = GetClampedZeroToOne(t);
	return Interpolate(outStart, outEnd, t);
}

int RoundDownToInt(float value)
{
	return static_cast<int>((floor(value)));
}

float GetMin(float value, float min)
{
	if (value < min)
	{
		return value;
	}
	return min;
}

float GetMax(float value, float max)
{
	if (value > max)
	{
		return value;
	}
	return max;
}

float DiminishingAdd(float componentA, float componentB)
{
	return 1.f - (1.f - componentA) * (1.f - componentB);
}

float ComputeCubicBezier1D(float a, float b, float c, float d, float t)
{
	// De Casteljau Algorithm
	float r0 = Interpolate(a, b, t);
	float r1 = Interpolate(b, c, t);
	float r2 = Interpolate(c, d, t);

	float s0 = Interpolate(r0, r1, t);
	float s1 = Interpolate(r1, r2, t);

	float t0 = Interpolate(s0, s1, t);
	return t0;
}

float ComputeQuinticBezier1D(float a, float b, float c, float d, float e, float f, float t)
{
	// De Casteljau Algorithm
	float p0 = Interpolate(a, b, t);
	float p1 = Interpolate(b, c, t);
	float p2 = Interpolate(c, d, t);
	float p3 = Interpolate(d, e, t);
	float p4 = Interpolate(e, f, t);

	float n0 = Interpolate(p0, p1, t);
	float n1 = Interpolate(p1, p2, t);
	float n2 = Interpolate(p2, p3, t);
	float n3 = Interpolate(p3, p4, t);

	float r0 = Interpolate(n0, n1, t);
	float r1 = Interpolate(n1, n2, t);
	float r2 = Interpolate(n2, n3, t);

	float s0 = Interpolate(r0, r1, t);
	float s1 = Interpolate(r1, r2, t);

	float t0 = Interpolate(s0, s1, t);
	return t0;
}

float SmoothStart2(float easeAmount)
{
	float t = easeAmount;
	return (t * t);
}

float SmoothStart3(float easeAmount)
{
	float t = easeAmount;
	return (t * t * t);
}

float SmoothStart4(float easeAmount)
{
	float t = easeAmount;
	return (t * t * t * t);
}

float SmoothStart5(float easeAmount)
{
	float t = easeAmount;
	return (t * t * t * t * t);
}

float SmoothStart6(float easeAmount)
{
	float t = easeAmount;
	return (t * t * t * t * t * t);
}

float SmoothStop2(float easeAmount)
{
	float t = easeAmount;
	float s = 1 - t;
	return (1 - (s * s));
}

float SmoothStop3(float easeAmount)
{
	float t = easeAmount;
	float s = 1 - t;
	return (1 - (s * s * s));
}

float SmoothStop4(float easeAmount)
{
	float t = easeAmount;
	float s = 1 - t;
	return (1 - (s * s * s * s));
}

float SmoothStop5(float easeAmount)
{
	float t = easeAmount;
	float s = 1 - t;
	return (1 - (s * s * s * s * s));
}

float SmoothStop6(float easeAmount)
{
	float t = easeAmount;
	float s = 1 - t;
	return (1 - (s * s * s * s * s * s));
}

float SmoothStep3(float easeAmount)
{
	float t = easeAmount;
	return Interpolate(SmoothStart2(t), SmoothStop2(t), t);
}

float SmoothStep5(float easeAmount)
{
	float t = easeAmount;
	return ComputeQuinticBezier1D(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, t);
}

float Hesitate3(float easeAmount)
{
	float t = easeAmount;
	return ComputeCubicBezier1D(0.f, 1.f, 0.f, 1.f, t);
}

float Hesitate5(float easeAmount)
{
	float t = easeAmount;
	return ComputeQuinticBezier1D(0.f, 1.f, 0.f, 1.f, 0.f, 1.f, t);
}

// Method for converting degrees to radians
float ConvertDegreesToRadians(float degrees)
{
	return degrees * (PI / 180.f);
}

// Method for converting radians to degrees
float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.f / PI);
}

float CosDegrees(float degrees)
{
	return cosf(degrees * (PI / 180.f));
}

float ACosDegrees(float degrees)
{
	return acosf(degrees * (PI / 180.f));
}

float SinDegrees(float degrees)
{
	return sinf(degrees * (PI / 180.f));
}

float ASinDegrees(float degrees)
{
	return asinf(degrees * (PI / 180.f));
}

float Atan2Degrees(float y, float x)
{
	return atan2f(y, x) * (180/PI);
}

float CopySignDegrees(float magnitude, float sign)
{
	return copysignf(magnitude, sign);
}

float GetShortestAngularDispDegrees(float startDegrees, float endDegrees)
{
	float displacement = endDegrees - startDegrees;
	while (displacement > 180.f)
	{
		displacement -= 360.f;
	}
	while (displacement < -180.f)
	{
		displacement += 360.f;
	}
	return displacement;
}

float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees)
{
	float angDispDeg = GetShortestAngularDispDegrees(currentDegrees, goalDegrees);
	if (fabsf(angDispDeg) < maxDeltaDegrees)
	{
		return goalDegrees;
	}
	if (angDispDeg > 0.f)
	{
		return currentDegrees + maxDeltaDegrees;
	}
	else
	{
		return currentDegrees - maxDeltaDegrees;
	}
}

float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b)
{
	float cosOfAngle = DotProduct2D(a.GetNormalized(), b.GetNormalized());
	cosOfAngle = GetClamped(cosOfAngle, -1.f, 1.f);
	float angleRadians = acosf(cosOfAngle);
	return ConvertRadiansToDegrees(angleRadians);
}

float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float CrossProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.y) - (a.y * b.x);
}

Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b)
{
	return Vec3((a.y * b.z - a.z * b.y), (a.z * b.x - a.x * b.z), (a.x * b.y - a.y * b.x));
}


// Method for getting a 2D distance between two 2D positions
float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float dx = positionA.x - positionB.x;
	float dy = positionA.y - positionB.y;
	return sqrtf((dx * dx) + (dy * dy));
}


// Method for getting a 2D distance squared between two 2D positions
// Faster method to get distance
float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float dx = positionA.x - positionB.x;
	float dy = positionA.y - positionB.y;
	return ((dx * dx) + (dy * dy));
}


// Method for getting a 3D distance between two 3D positions
float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float dx = positionA.x - positionB.x;
	float dy = positionA.y - positionB.y;
	float dz = positionA.z - positionB.z;
	return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}


// Method for getting a 3D distance squared between two 3D positions
float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float dx = positionA.x - positionB.x;
	float dy = positionA.y - positionB.y;
	float dz = positionA.z - positionB.z;
	return ((dx * dx) + (dy * dy) + (dz * dz));
}


// Method for getting a 2D distance between two 3D positions
float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float dx = positionA.x - positionB.x;
	float dy = positionA.y - positionB.y;
	return sqrtf((dx * dx) + (dy * dy));
}


// Method for getting a 2D distance squared between two 3D positions
float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float dx = positionA.x - positionB.x;
	float dy = positionA.y - positionB.y;
	return ((dx * dx) + (dy * dy));
}

int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB)
{
	int dx = abs(pointA.x - pointB.x);
	int dy = abs(pointA.y - pointB.y);
	return dx + dy;
}

float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 vectorToProjectOntoNormalized = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct2D(vectorToProjectOntoNormalized, vectorToProject);
	return projectedLength;
}

float GetProjectedLength3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto)
{
	Vec3 vectorToProjectOntoNormalized = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct3D(vectorToProjectOntoNormalized, vectorToProject);
	return projectedLength;
}

Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 vectorToProjectOntoNormalized = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct2D(vectorToProjectOntoNormalized, vectorToProject);
	return projectedLength * vectorToProjectOntoNormalized;
}

Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto)
{
	Vec3 vectorToProjectOntoNormalized = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct3D(vectorToProjectOntoNormalized, vectorToProject);
	return projectedLength * vectorToProjectOntoNormalized;
}


// Method asking if two discs or circles are overlapping
bool DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB)
{
	float dx = centerA.x - centerB.x;
	float dy = centerA.y - centerB.y;
	float distSquared2D = ((dx * dx) + (dy * dy));

	float radiusSum = radiusA + radiusB;
	float radiusSquared = radiusSum * radiusSum;

	return distSquared2D <= radiusSquared;
}

bool DoDiscsOverlapAABB2D(Vec2 const& discCenter, float discRadius, AABB2 const& box)
{
	float nearestX = GetClamped(discCenter.x, box.m_mins.x, box.m_maxs.x);
	float nearestY = GetClamped(discCenter.y, box.m_mins.y, box.m_maxs.y);
	Vec2  nearestPoint = Vec2(nearestX, nearestY);

	float distSquared = GetDistanceSquared2D(discCenter, nearestPoint);
	float radiusSquared = (discRadius * discRadius);

	return distSquared < radiusSquared;
}

bool DoSpheresOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	float dx = centerA.x - centerB.x;
	float dy = centerA.y - centerB.y;
	float dz = centerA.z - centerB.z;
	float distSquared3D = ((dx * dx) + (dy * dy) + (dz * dz));

	float radiusSum = radiusA + radiusB;
	float radiusSquared = radiusSum * radiusSum;

	return distSquared3D <= radiusSquared;
}

bool DoSpheresAndAABBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, AABB3 const& box)
{
	Vec3 nearestPoint = GetNearestPointOnAABB3D(sphereCenter, box);

	if (IsPointInsideSphere3D(nearestPoint, sphereCenter, sphereRadius))
	{
		return true;
	}
	return false;
}

bool DoAABB2sOverlap(AABB2 const& boxA, AABB2 const& boxB)
{
	// Left and right check
	if (boxA.m_maxs.x < boxB.m_mins.x || boxA.m_mins.x > boxB.m_maxs.x)
	{
		return false;
	}

	// Top and bottom check
	if (boxA.m_mins.y > boxB.m_maxs.y || boxA.m_maxs.y < boxB.m_mins.y)
	{
		return false;
	}

	// If none of the checks are fulfilled, we are overlapping
	return true;
}

bool DoAABB3sOverlap(AABB3 const& boxA, AABB3 const& boxB)
{
	// Forward and Backward check
	if (boxA.m_maxs.x < boxB.m_mins.x || boxA.m_mins.x > boxB.m_maxs.x)
	{
		return false;
	}

	// Left and Right check
	if (boxA.m_mins.y > boxB.m_maxs.y || boxA.m_maxs.y < boxB.m_mins.y)
	{
		return false;
	}

	// Top and Bottom check
	if (boxA.m_mins.z > boxB.m_maxs.z || boxA.m_maxs.z < boxB.m_mins.z)
	{
		return false;
	}

	// If none of the checks are fulfilled, we are overlapping
	return true;
}

bool DoZCylindersOverlap3D(Vec2 cylinderACenterXY, float cylinderARadius, FloatRange cylinderAMinMaxZ, Vec2 cylinderBCenterXY, float cylinderBRadius, FloatRange cylinderBMinMaxZ)
{
	float distSquared	   = (cylinderACenterXY - cylinderBCenterXY).GetLengthSquared();
	float radiusSum		   = (cylinderARadius + cylinderBRadius);
	float radiusSumSquared = (radiusSum * radiusSum);

	if (distSquared > radiusSumSquared)
	{
		return false;
	}

	if (cylinderAMinMaxZ.m_max < cylinderBMinMaxZ.m_min)
	{
		return false;
	}

	if (cylinderAMinMaxZ.m_min > cylinderBMinMaxZ.m_max)
	{
		return false;
	}

	return true;
}

bool DoZCylinderAndAABB3Overlap3D(Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, AABB3 const& box)
{
	AABB2 boxXY = AABB2(box.m_mins.x, box.m_mins.y, box.m_maxs.x, box.m_maxs.y);
	Vec2 nearestPoint = GetNearestPointOnAABB2D(cylinderCenterXY, boxXY);
	float distSquared = (nearestPoint - cylinderCenterXY).GetLengthSquared();
	float cylinderRadiusSquared = cylinderRadius * cylinderRadius;

	if (distSquared > cylinderRadiusSquared)
	{
		return false;
	}

	if (cylinderMinMaxZ.m_max < box.m_mins.z || cylinderMinMaxZ.m_min > box.m_maxs.z)
	{
		return false;
	}

	return true;
}

bool DoZCylindersOverlap3D(Vec3 const& cylinderAStart, float cylinderARadius, float cylinderAHeight, Vec3 const& cylinderBStart, float cylinderBRadius, float cylinderBHeight)
{
	Vec2 aDisc = Vec2(cylinderAStart.x, cylinderAStart.y);
	Vec2 bDisc = Vec2(cylinderBStart.x, cylinderBStart.y);
	if (cylinderAStart.z < cylinderBStart.z + cylinderBHeight && cylinderBStart.z < cylinderAHeight + cylinderAStart.z)
	{
		if (DoDiscsOverlap(aDisc, cylinderARadius, bDisc, cylinderBRadius))
		{
			return true;
		}
	}
	return false;
}

bool DoZCylinderAndAABB3Overlap3D(Vec3 const& cylinderStart, float cylinderRadius, float cylinderHeight, AABB3 const& box)
{
	Vec2 cylinderDisc = Vec2(cylinderStart.x, cylinderStart.y);
	AABB2 cylinderBox = AABB2(box.m_mins.x, box.m_mins.y, box.m_maxs.x, box.m_maxs.y);
	if (box.m_mins.z < cylinderStart.z + cylinderHeight && cylinderStart.z < box.m_maxs.z)
	{
		Vec2 nearestPoint = GetNearestPointOnAABB2D(cylinderDisc, cylinderBox);
		if (IsPointInsideDisc2D(nearestPoint, cylinderDisc, cylinderRadius))
		{
			return true;
		}
	}
	return false;
}

bool DoZCylinderAndSphereOverlap3D(Vec3 const& cylinderStart, float cylinderRadius, float cylinderHeight, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 nearestPoint = GetNearestPointOnCylinderZ3D(sphereCenter, cylinderStart, cylinderRadius, cylinderHeight);
	return IsPointInsideSphere3D(nearestPoint, sphereCenter, sphereRadius);
}

bool DoZCylinderAndOBB3sOverlap3D(Vec3 const& cylinderStart, float cylinderRadius, float cylinderHeight, OBB3 const& orientedBox)
{
	Vec3 localPos = orientedBox.GetLocalPosForWorldPos(cylinderStart);
	Vec3 nearestPointLocal = Vec3::ZERO;

	nearestPointLocal.x = GetClamped(localPos.x, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	nearestPointLocal.y = GetClamped(localPos.y, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);
	nearestPointLocal.z = GetClamped(localPos.z, -orientedBox.m_halfDimensions.z, orientedBox.m_halfDimensions.z);

	Vec3 centerToPoint = localPos - nearestPointLocal;
	float distBetween = (centerToPoint.x * centerToPoint.x) + (centerToPoint.y * centerToPoint.y);
	float radiusSquared = cylinderRadius * cylinderRadius;

	if (distBetween > radiusSquared)
	{
		return false;
	}

	float cylinderBaseZ = cylinderStart.z;
	float cylinderEndZ = (cylinderStart.z + cylinderHeight);
	float obbMinZ = orientedBox.m_center.z - orientedBox.m_halfDimensions.z;
	float obbMaxZ = orientedBox.m_center.z + orientedBox.m_halfDimensions.z;

	if (cylinderEndZ < obbMinZ || cylinderBaseZ > obbMaxZ)
	{
		return false;
	}

	return true;
}

bool DoOBB3sAndSpheresOverlap3D(OBB3 const& box, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 nearestPoint = GetNearestPointOnOBB3D(sphereCenter, box);
	return IsPointInsideSphere3D(nearestPoint, sphereCenter, sphereRadius);
}

bool DoOBB3sAndPlanesOverlap3D(OBB3 const& box, Plane3 const& plane)
{
	Vec3 obbCornerPoints[8];
	box.GetCornerPoints(obbCornerPoints);
	bool anyInFront = false;
	bool anyInBack = false;

	for (int i = 0; i < 8; ++i) 
	{
		if (plane.IsPointInFront(obbCornerPoints[i])) 
		{
			anyInFront = true;
		}
		else if (plane.IsPointInBack(obbCornerPoints[i])) 
		{
			anyInBack = true;
		}

		if (anyInFront && anyInBack) 
		{
			return true;
		}
	}
	return false;
}

bool DoPlanesAndSpheresOverlap3D(Plane3 const& plane, Vec3 const& sphereCenter, float sphereRadius)
{
	float signedDistance = DotProduct3D(plane.m_normal, sphereCenter) - plane.m_distance;
	return fabsf(signedDistance) <= sphereRadius;
}

bool DoPlanesAndAABB3sOverlap3D(Plane3 const& plane, AABB3 const& box)
{
	Vec3 boxCenter = box.GetCenter();
	Vec3 halfDims = (box.m_maxs - box.m_mins) * 0.5f;

	float projectionRadius = (halfDims.x * fabsf(plane.m_normal.x)) + (halfDims.y * fabsf(plane.m_normal.y)) + (halfDims.z * fabsf(plane.m_normal.z));
	float distance = DotProduct3D(plane.m_normal, boxCenter) - plane.m_distance;

	return fabsf(distance) <= projectionRadius;
}

bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius)
{
	float dx = point.x - discCenter.x;
	float dy = point.y - discCenter.y;
	return (dx * dx + dy * dy) <= (discRadius * discRadius);
}

bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box)
{
	return box.IsPointInside(point);
}

bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& box)
{
	return box.IsPointInside(point);
}

bool IsPointInsideCapsule(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 lineSegment = boneEnd - boneStart;
	Vec2 pointToStart = point - boneStart;
	float lineSegmentSquared = lineSegment.GetLengthSquared();

	float normalDistance = DotProduct2D(pointToStart, lineSegment) / lineSegmentSquared;
	normalDistance = GetClamped(normalDistance, 0.0f, 1.0f);

	Vec2 nearestPoint = boneStart + normalDistance * lineSegment;
	Vec2 pointToNearest = point - nearestPoint;
	float distance = pointToNearest.GetLength();
	return distance <= radius;
}

bool IsPointInsideTriangle2D(Vec2 const& point, Vec2 const& triCCW0, Vec2 const& triCCW1, Vec2 const& triCCW2)
{
	Vec2 aToB = triCCW1 - triCCW0;
	Vec2 bToC = triCCW2 - triCCW1;
	Vec2 cToA = triCCW0 - triCCW2;

	Vec2 aToPoint = point - triCCW0;
	Vec2 bToPoint = point - triCCW1;
	Vec2 cToPoint = point - triCCW2;

	float aToBPointCheck = aToB.x * aToPoint.y - aToB.y * aToPoint.x;
	float bToCPointCheck = bToC.x * bToPoint.y - bToC.y * bToPoint.x;
	float cToAPointCheck = cToA.x * cToPoint.y - cToA.y * cToPoint.x;
	return (aToBPointCheck >= 0 && bToCPointCheck >= 0 && cToAPointCheck >= 0) || (aToBPointCheck <= 0 && bToCPointCheck <= 0 && cToAPointCheck <= 0);
}

bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	float forwardRadians = ConvertDegreesToRadians(sectorForwardDegrees);
	Vec2 sectorTipToPoint = point - sectorTip;
	float distanceSquared = sectorTipToPoint.GetLengthSquared();

	if (distanceSquared > sectorRadius * sectorRadius)
	{
		return false;
	}

	Vec2 sectorForwardAngle = Vec2::MakeFromPolarRadians(forwardRadians);
	float angleToPoint = GetAngleDegreesBetweenVectors2D(sectorForwardAngle, sectorTipToPoint);
	float halfApertureDegrees = (sectorApertureDegrees) / 2.0f;
	return angleToPoint <= halfApertureDegrees;
}

bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	Vec2 sectorTipToPoint = point - sectorTip;
	float distanceSquared = sectorTipToPoint.GetLengthSquared();
	if (distanceSquared > sectorRadius * sectorRadius)
	{
		return false;
	}
	float angleToPoint = GetAngleDegreesBetweenVectors2D(sectorForwardNormal, sectorTipToPoint);
	return angleToPoint <= (sectorApertureDegrees / 2.0f);
}

bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 distBetween = point - sphereCenter;
	float distSquared = distBetween.GetLengthSquared();
	float radiusSquared = sphereRadius * sphereRadius;

	if (distSquared <= radiusSquared)
	{
		return true;
	}

	return false;
}

bool IsPointInsideAABB3D(Vec3 const& point, AABB3 const& box)
{
	return box.IsPointInside(point);
}

bool IsPointInsideOBB3D(Vec3 const& point, OBB3 const& box)
{
	return box.IsPointInside(point);
}

bool IsPointInsideZCylinder3D(Vec3 const& point, Vec3 const& start, float discRadius, float cylinderHeight)
{
	Vec2 startXY = Vec2(start.x, start.y);
	Vec2 pointXY = Vec2(point.x, point.y);

	if (IsPointInsideDisc2D(pointXY, startXY, discRadius) &&
		point.z >= start.z && point.z <= start.z + cylinderHeight)
	{
		return true;
	}

	return false;
}

Vec2 GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius)
{
	Vec2 distance = referencePosition - discCenter;
	float distanceSquared = distance.GetLengthSquared();
	float radiusSquared = discRadius * discRadius;

	if (distanceSquared <= radiusSquared)
	{
		return referencePosition;
	}
	else
	{
		Vec2 nearestPoint = discCenter + distance.GetNormalized() * discRadius;
		return nearestPoint;
	}
}

Vec2 GetNearestPointOnAABB2D(Vec2 const& referencePos, AABB2 const& alignedBox)
{
	return alignedBox.GetNearestPoint(referencePos);
}

Vec2 GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox)
{
	Vec2 distanceBetween = referencePos - orientedBox.m_center;
	Vec2 iBasis = orientedBox.m_iBasisNormal.GetNormalized();
	float distanceBetweenIBasis = DotProduct2D(distanceBetween, orientedBox.m_iBasisNormal);
	Vec2 jNormalized = Vec2(-orientedBox.m_iBasisNormal.y, orientedBox.m_iBasisNormal.x);
	float distanceBetweenJBasis = DotProduct2D(distanceBetween, jNormalized);

	float halfWidth = orientedBox.m_halfDimensions.x;
	float halfHeight = orientedBox.m_halfDimensions.y;

	float nearestIBasis = GetClamped(distanceBetweenIBasis, -halfWidth, halfWidth);
	float nearestJBasis = GetClamped(distanceBetweenJBasis, -halfHeight, halfHeight);

	Vec2 nearestPoint = orientedBox.m_center + nearestIBasis * iBasis + nearestJBasis * jNormalized;
	return nearestPoint;
}

Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine)
{
	// Region 1
	Vec2 startToEnd = anotherPointOnLine - pointOnLine;
	Vec2 startToPoint = referencePos - pointOnLine;
	if (DotProduct2D(startToEnd, startToPoint) <= 0)
	{
		return pointOnLine;
	}

	// Region 2
	Vec2 endToPoint = referencePos - anotherPointOnLine;
	if (DotProduct2D(startToEnd, endToPoint) >= 0)
	{
		return anotherPointOnLine;
	}

	// Region 3
	Vec2 startToNearest = GetProjectedOnto2D(startToPoint, startToEnd);
	Vec2 nearestPoint = pointOnLine + startToNearest;
	return nearestPoint;
}

Vec2 GetNearestPointOnLineSegment2D(Vec2 const& referencePos, Vec2 const& start, Vec2 const& end)
{
	// Region 1
	Vec2 startToEnd = end - start;
	Vec2 startToPoint = referencePos - start;
	if (DotProduct2D(startToEnd, startToPoint) <= 0)
	{
		return start;
	}

	// Region 2
	Vec2 endToPoint = referencePos - end;
	if (DotProduct2D(startToEnd, endToPoint) >= 0)
	{
		return end;
	}

	// Region 3
	Vec2 startToNearest = GetProjectedOnto2D(startToPoint, startToEnd);
	Vec2 nearestPoint = start + startToNearest;
	return nearestPoint;
}

Vec2 GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 lineSegment = boneEnd - boneStart;
	Vec2 pointToStart = referencePos - boneStart;
	float lineSegmentSquared = lineSegment.GetLengthSquared();

	float normalDistance = DotProduct2D(pointToStart, lineSegment) / lineSegmentSquared;
	normalDistance = GetClamped(normalDistance, 0.0f, 1.0f);

	Vec2 nearestPointOnLine = boneStart + normalDistance * lineSegment;
	Vec2 pointToNearest = referencePos - nearestPointOnLine;
	float distanceToNearest = pointToNearest.GetLength();

	if (distanceToNearest <= radius)
	{
		return nearestPointOnLine;
	}
	else
	{
		Vec2 direction = pointToNearest.GetNormalized();
		return nearestPointOnLine + direction * radius;
	}
}

Vec2 GetNearestPointOnTriangle2D(Vec2 const& referencePos, Vec2 const& triCCW0, Vec2 const& triCCW1, Vec2 const& triCCW2)
{
	Vec2 nearestA = GetNearestPointOnLineSegment2D(referencePos, triCCW0, triCCW1);
	Vec2 nearestB = GetNearestPointOnLineSegment2D(referencePos, triCCW1, triCCW2);
	Vec2 nearestC = GetNearestPointOnLineSegment2D(referencePos, triCCW2, triCCW0);

	if (IsPointInsideTriangle2D(referencePos, triCCW0, triCCW1, triCCW2))
	{
		return referencePos;
	}

	float minimumDistanceSquared = (nearestA - referencePos).GetLengthSquared();
	Vec2 closestPoint = nearestA;

	float distanceB = (nearestB - referencePos).GetLengthSquared();
	if (distanceB < minimumDistanceSquared) {
		minimumDistanceSquared = distanceB;
		closestPoint = nearestB;
	}

	float distanceC = (nearestC - referencePos).GetLengthSquared();
	if (distanceC < minimumDistanceSquared) {
		closestPoint = nearestC;
	}
	return closestPoint;
}

Vec3 const GetNearestPointOnSphere3D(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 distBetween = referencePos - sphereCenter;
	distBetween.Normalize();
	return sphereCenter + (distBetween * sphereRadius);
}

Vec3 const GetNearestPointOnAABB3D(Vec3 const& referencePos, AABB3 const& alignedBox)
{
	return alignedBox.GetNearestPoint(referencePos);
}

Vec3 const GetNearestPointOnOBB3D(Vec3 const& referencePos, OBB3 const& orientedBox)
{
	Vec3 localPos = orientedBox.GetLocalPosForWorldPos(referencePos);

	float clampedX = GetClamped(localPos.x, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	float clampedY = GetClamped(localPos.y, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);
	float clampedZ = GetClamped(localPos.z, -orientedBox.m_halfDimensions.z, orientedBox.m_halfDimensions.z);
	Vec3  clampedLocalPos = Vec3(clampedX, clampedY, clampedZ);

	return orientedBox.GetWorldPosFromLocalPos(clampedLocalPos);
}

Vec3 const GetNearestPointOnCylinderZ3D(Vec3 const& referencePos, Vec3 const& start, float discRadius, float cylinderHeight)
{
	Vec2 startXY = Vec2(start.x, start.y);
	Vec2 refPosXY = Vec2(referencePos.x, referencePos.y);
	Vec2 cylinderDisc = GetNearestPointOnDisc2D(refPosXY, startXY, discRadius);
	
	float refPosZValue = referencePos.z;
	refPosZValue = GetClamped(refPosZValue, start.z, start.z + cylinderHeight);
	
	Vec3 nearestPointOnCylinder = Vec3(cylinderDisc.x, cylinderDisc.y, refPosZValue);
	return nearestPointOnCylinder;
}

Vec3 const GetNearestPointOnPlane3D(Vec3 const& referencePos, Plane3 const& plane)
{
	Vec3 planeCenter = plane.GetPlaneCenter();
	Vec3 pointToPlaneDisp = referencePos - planeCenter;
	Vec3 planeNormalDisp = GetProjectedOnto3D(pointToPlaneDisp, plane.m_normal);
	Vec3 nearestPoint = planeCenter + (pointToPlaneDisp - planeNormalDisp);
	return nearestPoint;
}

bool PushDiscOutOfPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint)
{
	Vec2 distance = mobileDiscCenter - fixedPoint;
	float distanceSquared = distance.GetLengthSquared();
	float radiusSquared = discRadius * discRadius;
	float overlap = discRadius - distance.GetLength();

	if (distanceSquared >= radiusSquared)
	{
		return false;
	}
	if (overlap > 0)
	{
		distance.Normalize();
	}
	mobileDiscCenter += distance * overlap;
	return true;
}

bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	Vec2 distBetweenDiscCenters = mobileDiscCenter - fixedDiscCenter;
	float radiusSumSquared = (fixedDiscRadius + mobileDiscRadius) * (fixedDiscRadius + mobileDiscRadius);
	
	if (distBetweenDiscCenters.GetLengthSquared() < radiusSumSquared)
	{
		distBetweenDiscCenters.SetLength(fixedDiscRadius + mobileDiscRadius);
		mobileDiscCenter = fixedDiscCenter + distBetweenDiscCenters;

		return true;
	}
	return false;
}

bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius)
{
	Vec2 distBetweenDiscCenters = aCenter - bCenter;
	Vec2 normalizedDirection = distBetweenDiscCenters.GetNormalized();

	float radiusSum = aRadius + bRadius;
	float distance = distBetweenDiscCenters.GetLength();
	float overlap = radiusSum - distance;

	if (distance < radiusSum)
	{
		if (overlap > 0)
		{
			aCenter += normalizedDirection * (overlap / 2.0f);
			bCenter -= normalizedDirection * (overlap / 2.0f);
			return true;
		}
	}
	return false;
}

bool PushDiscOutOfAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox)
{
	Vec2 nearestPointOnAABB2 = GetNearestPointOnAABB2D(mobileDiscCenter, fixedBox);
	return PushDiscOutOfPoint2D(mobileDiscCenter, discRadius, nearestPointOnAABB2);
}

bool PushZCylinderOutOfFixedAABB3D(Vec3& start, float radius, float height, AABB3 const& fixedBox)
{
	float cylinderBottom = start.z - height * 0.5f;
	float cylinderTop = start.z + height * 0.5f;
	float boxBottom = fixedBox.m_mins.z;
	float boxTop = fixedBox.m_maxs.z;

	FloatRange cylinderZRange(cylinderBottom, cylinderTop);
	FloatRange boxZRange(boxBottom, boxTop);

	// Early check to see if our z ranges are overlapping
	if (!cylinderZRange.isOverLappingWith(boxZRange))
	{
		return false;
	}

	Vec2 cylinderStartXY = Vec2(start.x, start.y);
	AABB2 boxXY = AABB2(fixedBox.m_mins.GetXY(), fixedBox.m_maxs.GetXY());
	
	// Check X and Y Overlap
	if (!DoDiscsOverlapAABB2D(cylinderStartXY, radius, boxXY))
	{
		return false;
	}

	// Push out of Z
	float zOverlapTop = boxTop - cylinderBottom;
	float zOverlapBottom = cylinderTop - boxBottom;
	bool  nudgeZ = zOverlapTop < 0.1f || zOverlapBottom < 0.1f;

	if (nudgeZ)
	{
		if (zOverlapTop < zOverlapBottom)
		{
			start.z += zOverlapTop;
		}
		else
		{
			start.z -= zOverlapBottom;
		}
	}
	else
	{
		if (PushDiscOutOfAABB2D(cylinderStartXY, radius, boxXY))
		{
			start.x = cylinderStartXY.x;
			start.y = cylinderStartXY.y;
		}
	}

	return true;
}

bool BounceDiscsOffEachOther2D(Vec2& discACenter, Vec2& discBCenter, float discARadius, float discBRadius, Vec2& discAVelocity, Vec2& discBVelocity, float elasticityA, float elasticityB)
{
	if (!DoDiscsOverlap(discACenter, discARadius, discBCenter, discBRadius))
	{
		return false;
	}
	
	Vec2 centerDispAtoB = discBCenter - discACenter;
	Vec2 centerDispAtoBNormalized = centerDispAtoB.GetNormalized();

	float discAdotDisp = DotProduct2D(discAVelocity, centerDispAtoBNormalized);
	float discBdotDisp = DotProduct2D(discBVelocity, centerDispAtoBNormalized);

	Vec2 discANormalVelocity = centerDispAtoBNormalized * discAdotDisp;
	Vec2 discBNormalVelocity = centerDispAtoBNormalized * discBdotDisp;
	Vec2 discATangentVelocity = discAVelocity - discANormalVelocity;
	Vec2 discBTangentVelocity = discBVelocity - discBNormalVelocity;

	PushDiscsOutOfEachOther2D(discACenter, discARadius, discBCenter, discBRadius);

	float combinedElasiticty = elasticityA * elasticityB;
	Vec2 newDiscANormalVelocity = discBNormalVelocity * combinedElasiticty;
	Vec2 newDiscBNormalVelocity = discANormalVelocity * combinedElasiticty;

	if (discBdotDisp - discAdotDisp < 0)
	{
		discAVelocity = discATangentVelocity + newDiscANormalVelocity;
		discBVelocity = discBTangentVelocity + newDiscBNormalVelocity;
		return true;
	}

	return false;
}

bool BounceDiscOffFixedPoint(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Vec2 const& fixedPoint, float pointElasticity)
{
	float distanceSquared = GetDistanceSquared2D(discCenter, fixedPoint);
	if (distanceSquared >= discRadius * discRadius)
	{
		return false;
	}

	PushDiscOutOfPoint2D(discCenter, discRadius, fixedPoint);

	Vec2 direction = discCenter - fixedPoint;
	Vec2 normal = direction.GetNormalized();
	float velocityNormal = DotProduct2D(discVelocity, normal);

	if (velocityNormal > 0)
	{
		return false;
	}

	Vec2 velocityVector = velocityNormal * normal;
	Vec2 velocityTangent = discVelocity - velocityVector;

	float combinedElasticity = discElasticity * pointElasticity;
	discVelocity = velocityTangent - (velocityVector * combinedElasticity);
	return true;
}

bool BounceDiscOffFixedDisc2D(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Vec2 const& fixedDiscCenter, float fixedDiscRadius, float fixedDiscElasticity)
{
	if (!DoDiscsOverlap(discCenter, discRadius, fixedDiscCenter, fixedDiscRadius))
	{
		return false;
	}

	Vec2 direction = (fixedDiscCenter - discCenter).GetNormalized();
	float discdotDisp = DotProduct2D(discVelocity, direction);
	float fixedDisp   = DotProduct2D(Vec2::ZERO, direction);

	Vec2 discNormalVelocity = direction * discdotDisp;
	Vec2 discTangentVelocity = discVelocity - discNormalVelocity;
	float combinedElasticity = discElasticity * fixedDiscElasticity;

	PushDiscOutOfDisc2D(discCenter, discRadius, fixedDiscCenter, fixedDiscRadius);

	if (fixedDisp - discdotDisp < 0)
	{
		discNormalVelocity = -discNormalVelocity * combinedElasticity;
		discVelocity = discTangentVelocity + discNormalVelocity;
		return true;
	}

	return false;
}

void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	posToTransform.x *= uniformScale;
	posToTransform.y *= uniformScale;

	float x = posToTransform.x * CosDegrees(rotationDegrees) - posToTransform.y * SinDegrees(rotationDegrees);
	float y = posToTransform.x * SinDegrees(rotationDegrees) + posToTransform.y * CosDegrees(rotationDegrees);
	posToTransform.x = x;
	posToTransform.y = y;

	posToTransform.x += translation.x;
	posToTransform.y += translation.y;

}

void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	posToTransform = translation + (posToTransform.x * iBasis) + (posToTransform.y * jBasis);
}

void TransformPositionXY3D(Vec3& posToTransform, float uniformScale, float zRotationDegrees, Vec2 const& translationXY)
{
	posToTransform.x *= uniformScale;
	posToTransform.y *= uniformScale;

	float x = posToTransform.x * CosDegrees(zRotationDegrees) - posToTransform.y * SinDegrees(zRotationDegrees);
	float y = posToTransform.x * SinDegrees(zRotationDegrees) + posToTransform.y * CosDegrees(zRotationDegrees);
	posToTransform.x = x;
	posToTransform.y = y;

	posToTransform.x += translationXY.x;
	posToTransform.y += translationXY.y;
}

void TransformPositionXY3D(Vec3& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translationXY)
{
	float transformedX = (posToTransform.x * iBasis.x) + (posToTransform.y * jBasis.x);
	float transformedY = (posToTransform.x * iBasis.y) + (posToTransform.y * jBasis.y);

	posToTransform.x = transformedX;
	posToTransform.y = transformedY;

	posToTransform.x += translationXY.x;
	posToTransform.y += translationXY.y;
}

Mat44 GetBillboardMatrix(BillboardType billboardType, Mat44 const& targetMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale)
{
	Vec3 iBasis;
	Vec3 jBasis;
	Vec3 kBasis;

	switch (billboardType)
	{
		case BillboardType::NONE:
		{
			break;
		}
		case BillboardType::FULL_FACING:
		{
			iBasis = targetMatrix.GetTranslation3D() - billboardPosition;
			iBasis.Normalize();

			if (fabs(DotProduct3D(iBasis, Vec3::ZAXE)) != 1.f)
			{
				jBasis = CrossProduct3D(Vec3::ZAXE, iBasis);
				kBasis = CrossProduct3D(iBasis, jBasis);
			}
			else
			{
				kBasis = CrossProduct3D(Vec3::YAXE, iBasis);
				jBasis = CrossProduct3D(iBasis, jBasis);
			}
			return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
		}
		case BillboardType::FULL_OPPOSING:
		{
			iBasis = -targetMatrix.GetIBasis3D();
			iBasis.Normalize();

			jBasis = -targetMatrix.GetJBasis3D();
			jBasis.Normalize();

			kBasis = targetMatrix.GetKBasis3D();
			kBasis.Normalize();

			return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
		}
		case BillboardType::WORLD_UP_FACING:
		{
			iBasis = targetMatrix.GetTranslation3D() - billboardPosition;
			iBasis.Normalize();
			iBasis.z = 0.f;

			jBasis = Vec3(-iBasis.y, iBasis.x, 0.f);
			kBasis = Vec3::ZAXE;

			return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
		}
		case BillboardType::WORLD_UP_OPPOSING:
		{
			iBasis = -targetMatrix.GetIBasis3D();
			iBasis.Normalize();
			iBasis.z = 0.f;

			jBasis = CrossProduct3D(iBasis, jBasis);
			kBasis = Vec3::ZAXE;

			return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
		}
	}
	return Mat44();
}

Mat44 GetLookAtMatrix(Vec3 const& start, Vec3 const& end)
{
	Vec3 iBasis = end - start;
	iBasis.Normalize();

	Vec3 jBasis = CrossProduct3D(iBasis, Vec3::ZAXE);
	if (jBasis.GetLengthSquared() < 0.01f)
	{
		jBasis = CrossProduct3D(iBasis, Vec3::YAXE);
	}
	jBasis.Normalize();

	Vec3 kBasis = CrossProduct3D(iBasis, jBasis);
	kBasis.Normalize();

	Mat44 lookAtMatrix;
	lookAtMatrix.SetIJKT3D(jBasis, kBasis, -iBasis, start);
	return lookAtMatrix;
}

