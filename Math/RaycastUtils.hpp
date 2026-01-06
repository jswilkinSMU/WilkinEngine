#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/AABB2.h"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/Plane3.hpp"
// -----------------------------------------------------------------------------
struct RaycastResult2D
{
	// Basic Raycast result information
	bool m_didImpact = false;
	float m_impactDist = 0.0f;
	Vec2 m_impactPos;
	Vec2 m_impactNormal;

	// Original Raycast information
	Vec2 m_rayFwdNormal;
	Vec2 m_rayStartPos;
	float m_rayMaxLength = 1.f;
};
// -----------------------------------------------------------------------------
struct RaycastResult3D
{
	bool m_didImpact = false;
	Vec3 m_rayStartPosition;
	Vec3 m_rayFwdNormal;
	Vec3 m_rayDirection;
	float m_rayLength = 1.0f;
	float m_impactDist = 0.0f;
	Vec3 m_impactPos;
	Vec3 m_impactNormal;
};
// -----------------------------------------------------------------------------
struct Ray2
{
	Ray2() = default;
	Ray2(Vec2 const& startPos, Vec2 const& endPos);
	Ray2(Vec2 const& startPos, Vec2 const& fwdNormal, float maxLength);

	Vec2 m_startPos;
	Vec2 m_endPos;
	Vec2 m_fwdNormal;
	float m_maxLength = 1.f;
};
// -----------------------------------------------------------------------------
// Raycasting methods 2D
RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius);
RaycastResult2D RaycastVsLineSegment2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 lineStart, Vec2 lineEnd);
RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 const& bounds);
// -----------------------------------------------------------------------------
// Raycasting methods 3D
RaycastResult3D RaycastVsSphere3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, Vec3 sphereCenter, float sphereRadius);
RaycastResult3D RaycastVsAABB3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, AABB3 const& box);
RaycastResult3D RaycastVsCylinder3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, Vec3 cylinderStart, float cylinderRadius, float cylinderHeight);
RaycastResult3D RaycastVsOBB3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, OBB3 const& box);
RaycastResult3D RaycastVsPlane3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, Plane3 const& plane);
// -----------------------------------------------------------------------------
// Raycast helpers
RaycastResult2D GetNearestImpactOnBounds(RaycastResult2D const& raycastResult1, RaycastResult2D raycastResult2, RaycastResult2D raycastResult3, RaycastResult2D raycastResult4);
void			AxisIntersection(float boxMin, float boxMax, float rayStart, float rayDir, float& tMin, float& tMax);
