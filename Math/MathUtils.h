#pragma once
#include "Engine/Math/IntVec2.h"
#include "Engine/Math/AABB2.h"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/Quat.hpp"
// -----------------------------------------------------------------------------
constexpr float PI = 3.14159265359f;
// -----------------------------------------------------------------------------
struct Vec2;
struct Vec3;
struct Vec4;
// -----------------------------------------------------------------------------
enum class BillboardType
{
	NONE = -1,
	WORLD_UP_FACING,
	WORLD_UP_OPPOSING,
	FULL_FACING,
	FULL_OPPOSING,
	COUNT
};
// -----------------------------------------------------------------------------
// Clamp and Lerp
template <typename T>
T GetClamped(T value, T minValue, T maxValue)
{
	if (value < minValue)
	{
		return minValue;
	}
	if (value > maxValue)
	{
		return maxValue;
	}
	return value;
}
float GetClampedZeroToOne(float value);
float SoftClamp(float value, float minValue, float maxValue, float stiffness = 0.1f);
float Interpolate(float start, float end, float fractionTowardEnd);
Vec2  Interpolate(Vec2 startPos, Vec2 endPos, float fractionTowardEnd);
Vec3  Interpolate(Vec3 startPos, Vec3 endPos, float fractionTowardEnd);
Vec4  Interpolate(Vec4 startPos, Vec4 endPos, float fractionTowardEnd);
Quat  Interpolate(Quat startPos, Quat endPos, float fractionTowardEnd);
Quat  SLerp(Quat startQuat, Quat endQuat, float fractionTowardEnd);
Vec3  SLerp(Vec3 startDir, Vec3 endDir, float fractionTowardEnd);
float NormalizeByte(unsigned char byteValue);
unsigned char DenormalizeByte(float normalizedValue);
float GetFractionWithinRange(float value, float rangeStart, float rangeEnd);
float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd);
float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd);
int   RoundDownToInt(float value);
float GetMin(float value, float min);
float GetMax(float value, float max);
float DiminishingAdd(float componentA, float componentB);

// 1D Bezier Standalone functions
float ComputeCubicBezier1D(float a, float b, float c, float d, float t);
float ComputeQuinticBezier1D(float a, float b, float c, float d, float e, float f, float t);

// Easing Utilities
float SmoothStart2(float easeAmount);
float SmoothStart3(float easeAmount);
float SmoothStart4(float easeAmount);
float SmoothStart5(float easeAmount);
float SmoothStart6(float easeAmount);
float SmoothStop2(float easeAmount);
float SmoothStop3(float easeAmount);
float SmoothStop4(float easeAmount);
float SmoothStop5(float easeAmount);
float SmoothStop6(float easeAmount);
float SmoothStep3(float easeAmount);
float SmoothStep5(float easeAmount);
float Hesitate3(float easeAmount);
float Hesitate5(float easeAmount);

// Angle Utilities
float ConvertDegreesToRadians(float degrees);
float ConvertRadiansToDegrees(float radians);
float CosDegrees(float degrees);
float ACosDegrees(float degrees);
float SinDegrees(float degrees);
float ASinDegrees(float degrees);
float Atan2Degrees(float y, float x);
float CopySignDegrees(float magnitude, float sign);
float GetShortestAngularDispDegrees(float startDegrees, float endDegrees);
float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees);
float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b);

// Dot and Cross
float DotProduct2D(Vec2 const& a, Vec2 const& b);
float DotProduct3D(Vec3 const& a, Vec3 const& b);
float DotProduct4D(Vec4 const& a, Vec4 const& b);
float CrossProduct2D(Vec2 const& a, Vec2 const& b);
Vec3  CrossProduct3D(Vec3 const& a, Vec3 const& b);

// 2D and 3D utilities
float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB); // Distance apart of two 2D positions 
float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB); // Distance apart of two 2D positions squared
float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB); // Distance apart of two 3D positions
float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB); // Distance apart of two 3D positions squared
float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB); // 2D Distance apart of two 3D positions
float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB); // 2D Distance apart of two 3D positions squared
int   GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB);
float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto); // Works if Vecs not normalized
float GetProjectedLength3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto);
Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto); // Works if Vecs not normalized
Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto);


// Overlapping utilities
bool DoDiscsOverlap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB);
bool DoDiscsOverlapAABB2D(Vec2 const& discCenter, float discRadius, AABB2 const& box);
bool DoSpheresOverlap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);
bool DoSpheresAndAABBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, AABB3 const& box);
bool DoAABB2sOverlap(AABB2 const& boxA, AABB2 const& boxB);
bool DoAABB3sOverlap(AABB3 const& boxA, AABB3 const& boxB);
bool DoZCylindersOverlap3D(Vec2 cylinderACenterXY, float cylinderARadius, FloatRange cylinderAMinMaxZ, Vec2 cylinderBCenterXY, float cylinderBRadius, FloatRange cylinderBMinMaxZ);
bool DoZCylinderAndAABB3Overlap3D(Vec2 cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, AABB3 const& box);
bool DoZCylindersOverlap3D(Vec3 const& cylinderAStart, float cylinderARadius, float cylinderAHeight, Vec3 const& cylinderBStart, float cylinderBRadius, float cylinderBHeight);
bool DoZCylinderAndAABB3Overlap3D(Vec3 const& cylinderStart, float cylinderRadius, float cylinderHeight, AABB3 const& box);
bool DoZCylinderAndSphereOverlap3D(Vec3 const& cylinderStart, float cylinderRadius, float cylinderHeight, Vec3 const& sphereCenter, float sphereRadius);
bool DoZCylinderAndOBB3sOverlap3D(Vec3 const& cylinderStart, float cylinderRadius, float cylinderHeight, OBB3 const& orientedBox);
bool DoOBB3sAndSpheresOverlap3D(OBB3 const& box, Vec3 const& sphereCenter, float sphereRadius);
bool DoOBB3sAndPlanesOverlap3D(OBB3 const& box, Plane3 const& plane);
bool DoPlanesAndSpheresOverlap3D(Plane3 const& plane, Vec3 const& sphereCenter, float sphereRadius);
bool DoPlanesAndAABB3sOverlap3D(Plane3 const& plane, AABB3 const& box);

// Pushing Out Utilities
bool PushDiscOutOfPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint);
bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius);
bool PushDiscOutOfAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox);
bool PushZCylinderOutOfFixedAABB3D(Vec3& start, float radius, float height, AABB3 const& fixedBox);

// Bouncing utilities
bool BounceDiscsOffEachOther2D(Vec2& discACenter, Vec2& discBCenter, float discARadius, float discBRadius, Vec2& discAVelocity, Vec2& discBVelocity, float elasticityA, float elasticityB);
bool BounceDiscOffFixedPoint(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Vec2 const& fixedPoint, float pointElasticity);
bool BounceDiscOffFixedDisc2D(Vec2& discCenter, float discRadius, Vec2& discVelocity, float discElasticity, Vec2 const& fixedDiscCenter, float fixedDiscRadius, float fixedDiscElasticity);

// Is Point Inside
bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius);
bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box);
bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& box);
bool IsPointInsideCapsule(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
bool IsPointInsideTriangle2D(Vec2 const& point, Vec2 const& triCCW0, Vec2 const& triCCW1, Vec2 const& triCCW2);
bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float sphereRadius);
bool IsPointInsideAABB3D(Vec3 const& point, AABB3 const& box);
bool IsPointInsideOBB3D(Vec3 const& point, OBB3 const& box);
bool IsPointInsideZCylinder3D(Vec3 const& point, Vec3 const& start, float discRadius, float cylinderHeight);

// Get Nearest Point
Vec2 GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius);
Vec2 GetNearestPointOnAABB2D(Vec2 const& referencePos, AABB2 const& alignedBox);
Vec2 GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox);
Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine);
Vec2 GetNearestPointOnLineSegment2D(Vec2 const& referencePos, Vec2 const& start, Vec2 const& end);
Vec2 GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
Vec2 GetNearestPointOnTriangle2D(Vec2 const& referencePos, Vec2 const& triCCW0, Vec2 const& triCCW1, Vec2 const& triCCW2);
Vec3 const GetNearestPointOnSphere3D(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRadius);
Vec3 const GetNearestPointOnAABB3D(Vec3 const& referencePos, AABB3 const& alignedBox);
Vec3 const GetNearestPointOnOBB3D(Vec3 const& referencePos, OBB3 const& orientedBox);
Vec3 const GetNearestPointOnCylinderZ3D(Vec3 const& referencePos, Vec3 const& start, float discRadius, float cylinderHeight);
Vec3 const GetNearestPointOnPlane3D(Vec3 const& referencePos, Plane3 const& plane);

// Transform utilities
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation);
void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void TransformPositionXY3D(Vec3& posToTransform, float uniformScale, float zRotationDegrees, Vec2 const& translationXY);
void TransformPositionXY3D(Vec3& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translationXY);
// -----------------------------------------------------------------------------
Mat44 GetBillboardMatrix(BillboardType billboardType, Mat44 const& targetMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale = Vec2::ONE);
Mat44 GetLookAtMatrix(Vec3 const& start, Vec3 const& end);