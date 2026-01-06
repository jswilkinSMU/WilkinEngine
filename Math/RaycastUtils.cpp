#include "Engine/Math/RaycastUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Math/FloatRange.hpp"

RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius)
{
	RaycastResult2D rayCastResult;
	rayCastResult.m_rayStartPos = startPos;
	rayCastResult.m_rayFwdNormal = fwdNormal;
	rayCastResult.m_rayMaxLength = maxDist;

	Vec2 iNormalized = fwdNormal.GetNormalized();
	Vec2 jNormalized = iNormalized.GetRotated90Degrees();
	Vec2 startToCenter = discCenter - startPos;
	float startToCenterJProjection = DotProduct2D(startToCenter, jNormalized);

	if (startToCenterJProjection >= discRadius || startToCenterJProjection <= -discRadius)
	{
		return rayCastResult;
	}

	float discRadiusSquared = discRadius * discRadius;
	float startToCenterJProjectionSquared = startToCenterJProjection * startToCenterJProjection;
	float adjust = sqrtf(discRadiusSquared - startToCenterJProjectionSquared);
	float startToCenterIProjection = DotProduct2D(startToCenter, iNormalized);
	float impactDist = startToCenterIProjection - adjust;

	if (impactDist >= maxDist)
	{
		return rayCastResult;
	}

	if (IsPointInsideDisc2D(startPos, discCenter, discRadius))
	{
		Vec2 impactNormal = iNormalized.GetRotatedDegrees(180.f);
		rayCastResult.m_didImpact = true;
		rayCastResult.m_impactNormal = impactNormal;
		rayCastResult.m_impactPos = startPos;
		return rayCastResult;
	}

	if (startToCenterIProjection <= 0.f)
	{
		return rayCastResult;
	}

	rayCastResult.m_didImpact = true;
	rayCastResult.m_impactDist = impactDist;
	rayCastResult.m_impactPos = startPos + iNormalized * impactDist;

	Vec2 impactNormal = (rayCastResult.m_impactPos - discCenter).GetNormalized();
	rayCastResult.m_impactNormal = impactNormal;

	return rayCastResult;
}

RaycastResult2D RaycastVsLineSegment2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 lineStart, Vec2 lineEnd)
{
	RaycastResult2D rayCastResult;

	Vec2 iNormalized = fwdNormal.GetNormalized();
	Vec2 jNormalized = iNormalized.GetRotated90Degrees();

	Vec2 rayStartToLineStart = lineStart - startPos;
	Vec2 rayStartToLineEnd = lineEnd - startPos;
	float rayToStartJ = DotProduct2D(rayStartToLineStart, jNormalized);
	float rayToEndJ = DotProduct2D(rayStartToLineEnd, jNormalized);

	if ((rayToStartJ * rayToEndJ) >= 0)
	{
		// Miss! Failure to straddle
		return rayCastResult;
	}

	float rayToStartI = DotProduct2D(rayStartToLineStart, iNormalized);
	float rayToEndI = DotProduct2D(rayStartToLineEnd, iNormalized);

	if (rayToStartI >= maxDist && rayToEndI >= maxDist)
	{
		// Miss! After ray end
		return rayCastResult;
	}
	if (rayToStartI <= 0 && rayToEndI <= 0)
	{
		// Miss! Before ray start
		return rayCastResult;
	}

	float tImpact = rayToStartJ / (rayToStartJ - rayToEndJ);
	float impactDist = rayToStartI + tImpact * (rayToEndI - rayToStartI);

	if (impactDist <= 0 || impactDist >= maxDist)
	{
		// Miss!
		return rayCastResult;
	}

	Vec2 lineDirection = lineEnd - lineStart;
	Vec2 lineImpactNormal = lineDirection.GetRotated90Degrees().GetNormalized();
	if (rayToEndJ < 0.f)
	{
		lineImpactNormal *= -1.f;
	}

	rayCastResult.m_didImpact = true;
	rayCastResult.m_impactDist = impactDist;
	rayCastResult.m_impactPos = startPos + iNormalized * impactDist;
	rayCastResult.m_impactNormal = lineImpactNormal;

	return rayCastResult;
}

RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 const& bounds)
{
	RaycastResult2D raycastResult;

	// Is Point Inside Check
	if (IsPointInsideAABB2D(startPos, bounds))
	{
		raycastResult.m_impactNormal = -fwdNormal;
		raycastResult.m_didImpact = true;
		raycastResult.m_impactPos = startPos;
		return raycastResult;
	}

	// Raycast vs 4 line segments method
	RaycastResult2D resultBottom		 = RaycastVsLineSegment2D(startPos, fwdNormal, maxDist, bounds.m_mins, Vec2(bounds.m_maxs.x, bounds.m_mins.y));
	RaycastResult2D resultRight			 = RaycastVsLineSegment2D(startPos, fwdNormal, maxDist, Vec2(bounds.m_maxs.x, bounds.m_mins.y), bounds.m_maxs);
	RaycastResult2D resultTop			 = RaycastVsLineSegment2D(startPos, fwdNormal, maxDist, bounds.m_maxs, Vec2(bounds.m_mins.x, bounds.m_maxs.y));
	RaycastResult2D resultLeft			 = RaycastVsLineSegment2D(startPos, fwdNormal, maxDist, Vec2(bounds.m_mins.x, bounds.m_maxs.y), bounds.m_mins);
	RaycastResult2D raycastNearestImpact = GetNearestImpactOnBounds(resultBottom, resultRight, resultTop, resultLeft);

	return raycastNearestImpact;

	// Time Range Method
	/// Set X and Y float ranges
// 	FloatRange xRange = FloatRange(bounds.m_mins.x, bounds.m_maxs.x);
// 	FloatRange yRange = FloatRange(bounds.m_mins.y, bounds.m_maxs.y);
// 	float minX = (bounds.m_mins.x - startPos.x) / fwdNormal.x;
// 	float maxX = (bounds.m_maxs.x - startPos.x) / fwdNormal.x;
// 	float minY = (bounds.m_mins.y - startPos.y) / fwdNormal.y;
// 	float maxY = (bounds.m_maxs.y - startPos.y) / fwdNormal.y;
// 
// 	// Check for impacts on X range
// 	if (!IsImpactOnRange(minX, startPos, fwdNormal, yRange, false))
// 	{
// 		minX = maxDist + 1.f;
// 	}
// 	if (!IsImpactOnRange(maxX, startPos, fwdNormal, yRange, false))
// 	{
// 		maxX = maxDist + 1.f;
// 	}
// 	// Check for impacts on Y range
// 	if (!IsImpactOnRange(minY, startPos, fwdNormal, xRange, true))
// 	{
// 		minY = maxDist + 1.f;
// 	}
// 	if (!IsImpactOnRange(maxY, startPos, fwdNormal, xRange, true))
// 	{
// 		maxY = maxDist + 1.f;
// 	}
// 
// 	// Check if x and y ranges are not overlapping
// 	if (!xRange.isOverLappingWith(yRange))
// 	{
// 		raycastResult;
// 	}
// 
// 	// Impact Normal direction checks
// 	// We have an impact on the right
// 	Vec2 impactNormal = Vec2(-1.0f, 0.0f);
// 	if (maxX < minX)
// 	{
// 		// We have an impact on the left
// 		minX = maxX;
// 		impactNormal = Vec2(1.0f, 0.0f);
// 	}
// 	if (minY < minX)
// 	{
// 		// We have an impact on the bottom
// 		minX = minY;
// 		impactNormal = Vec2(0.0f, -1.0f);
// 	}
// 	if (maxY < minX)
// 	{
// 		// We have an impact on the top
// 		minX = maxY;
// 		impactNormal = Vec2(0.0f, 1.0f);
// 	}
// 
// 	if (minX < maxDist)
// 	{
// 		raycastResult.m_impactNormal = impactNormal;
// 		raycastResult.m_impactPos = startPos + fwdNormal * minX;
// 		raycastResult.m_didImpact = true;
// 		raycastResult.m_impactDist = minX;
// 		return raycastResult;
// 	}
// 
// 	return raycastResult;
}

RaycastResult3D RaycastVsSphere3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, Vec3 sphereCenter, float sphereRadius)
{
	RaycastResult3D rayCastResult;

	// Is Point Inside check
	if (IsPointInsideSphere3D(rayStart, sphereCenter, sphereRadius))
	{
		rayCastResult.m_didImpact = true;
		rayCastResult.m_impactNormal = -fwdNormal;
		rayCastResult.m_impactPos = rayStart;
		return rayCastResult;
	}

	Vec3 startToCenter = sphereCenter - rayStart;
	Vec3 iNormalized = fwdNormal; iNormalized.Normalize();
	float startToCenterIProjection = DotProduct3D(startToCenter, iNormalized);
	float sphereRadiusSquared = sphereRadius * sphereRadius;
	Vec3 iBasis = startToCenterIProjection * fwdNormal;


	// Checks for if ray projection is shorter or longer than dist to sphere
	if (startToCenterIProjection <= 0.f)
	{
		return rayCastResult;
	}
	if (startToCenterIProjection >= rayLength + sphereRadius)
	{
		return rayCastResult;
	}

	Vec3 startToCenterJProjection = startToCenter - iBasis;
	float startToCenterJProjectionSquared = startToCenterJProjection.GetLengthSquared();

	// Check for if J projection is greater than sphere radius
	if (startToCenterJProjectionSquared >= sphereRadiusSquared)
	{
		return rayCastResult;
	}

	float adjust = sqrtf(sphereRadiusSquared - startToCenterJProjectionSquared);
	float impactDist = startToCenterIProjection - adjust;

	if (impactDist >= rayLength)
	{
		return rayCastResult;
	}

	// If all checks passed, we have a hit
	rayCastResult.m_didImpact = true;
	rayCastResult.m_impactDist = impactDist;
	rayCastResult.m_impactPos = rayStart + iNormalized * impactDist;

	Vec3 impactNormal = (rayCastResult.m_impactPos - sphereCenter); 
	impactNormal.Normalize();
	rayCastResult.m_impactNormal = impactNormal;

	return rayCastResult;
}

RaycastResult3D RaycastVsAABB3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, AABB3 const& box)
{
	RaycastResult3D rayCastResult;
	float tXMin; float tXMax; 
	float tYMin; float tYMax; 
	float tZMin; float tZMax;

	// Is Point Inside Check
	if (IsPointInsideAABB3D(rayStart, box))
	{
		rayCastResult.m_didImpact = true;
		rayCastResult.m_impactNormal = -fwdNormal;
		rayCastResult.m_impactPos = rayStart;
		return rayCastResult;
	}

	AxisIntersection(box.m_mins.x, box.m_maxs.x, rayStart.x, fwdNormal.x, tXMin, tXMax);
	AxisIntersection(box.m_mins.y, box.m_maxs.y, rayStart.y, fwdNormal.y, tYMin, tYMax);
	AxisIntersection(box.m_mins.z, box.m_maxs.z, rayStart.z, fwdNormal.z, tZMin, tZMax);
	float tMin = std::max(tXMin, std::max(tYMin, tZMin));
	float tMax = std::min(tXMax, std::min(tYMax, tZMax));

	// Return if we aren't intersecting
	if (tMin > tMax)
	{
		return rayCastResult;
	}
	if (tMax < 0.f)
	{
		return rayCastResult;
	}
	if (tMin > rayLength)
	{
		return rayCastResult;
	}

	// Impact Normal Checks
	if (tMin == tXMin)
	{
		if (fwdNormal.x < 0.f)
		{
			rayCastResult.m_impactNormal = Vec3::XAXE;
		}
		else
		{
			rayCastResult.m_impactNormal = -Vec3::XAXE;
		}
	}
	else if (tMin == tYMin)
	{
		if (fwdNormal.y < 0.f)
		{
			rayCastResult.m_impactNormal = Vec3::YAXE;
		}
		else
		{
			rayCastResult.m_impactNormal = -Vec3::YAXE;
		}
	}
	else
	{
		if (fwdNormal.z < 0.f)
		{
			rayCastResult.m_impactNormal = Vec3::ZAXE;
		}
		else
		{
			rayCastResult.m_impactNormal = -Vec3::ZAXE;
		}
	}

	rayCastResult.m_impactDist = tMin;
	rayCastResult.m_impactPos = rayStart + fwdNormal * tMin;
	rayCastResult.m_didImpact = true;
	return rayCastResult;
}

RaycastResult3D RaycastVsCylinder3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, Vec3 cylinderStart, float cylinderRadius, float cylinderHeight)
{
	RaycastResult3D raycastResult;

	Vec2 rayStartXY = Vec2(rayStart.x, rayStart.y);
	Vec2 fwdNormalXY = Vec2(fwdNormal.x, fwdNormal.y);
	Vec2 cylinderStartXY = Vec2(cylinderStart.x, cylinderStart.y);
	float cylinderRadiusSquared = cylinderRadius * cylinderRadius;

	float fwdLengthXY = fwdNormalXY.NormalizeAndGetPreviousLength();
	// Check and exit if there is no raycast direction
	if (fwdLengthXY == 0) 
	{
		return raycastResult;
	}

	float rayLengthXY = fwdLengthXY * rayLength;
	RaycastResult2D impactedDisc = RaycastVsDisc2D(rayStartXY, fwdNormalXY, rayLengthXY, cylinderStartXY, cylinderRadius);

	// Is Point Inside Check
	if (IsPointInsideZCylinder3D(rayStart, cylinderStart, cylinderRadius, cylinderHeight))
	{
		raycastResult.m_didImpact = true;
		raycastResult.m_impactNormal = -fwdNormal;
		raycastResult.m_impactPos = rayStart;
		return raycastResult;
	}

	// Early check and exit if we have no impact on the disc
	if (impactedDisc.m_didImpact == false)
	{
		return raycastResult;
	}

	//--------------------------------BOTTOM-------------------------------------//
	float impactDistBottomDisc = (cylinderStart.z - rayStart.z) / fwdNormal.z;
	Vec3 impactPosBottomDisc = rayStart + impactDistBottomDisc * fwdNormal;
	float cylinderXYProjectedBottom = (cylinderStart - impactPosBottomDisc).GetLengthXYSquared();

	// Check bottom disc
	if (fwdNormal.z > 0.f && rayStart.z <= cylinderStart.z && cylinderXYProjectedBottom < cylinderRadiusSquared && impactDistBottomDisc < rayLength)
	{
		raycastResult.m_didImpact = true;
		raycastResult.m_impactPos = impactPosBottomDisc;
		raycastResult.m_impactNormal = -Vec3::ZAXE;
		raycastResult.m_impactDist = impactDistBottomDisc;
		return raycastResult;
	}
	//--------------------------------------------------------------------------//

	//--------------------------------BODY-------------------------------------//
	Vec3 impactedDiscPos = rayStart + fwdNormal * impactedDisc.m_impactDist * (1.f / fwdLengthXY);
	Vec2 impactedDiscPosXY = Vec2(impactedDiscPos.x, impactedDiscPos.y);

	// Check Cylinder Body
	if (impactedDiscPos.z > cylinderStart.z && impactedDiscPos.z < cylinderStart.z + cylinderHeight)
	{
		Vec2 impactNormalXY = impactedDiscPosXY - cylinderStartXY;
		impactNormalXY.Normalize();

		raycastResult.m_didImpact = true;
		raycastResult.m_impactPos = impactedDiscPos;
		raycastResult.m_impactNormal = Vec3(impactNormalXY.x, impactNormalXY.y, 0.f);
		raycastResult.m_impactDist = impactedDisc.m_impactDist * (1.f / fwdLengthXY);
		return raycastResult;
	}
	//-------------------------------------------------------------------------//

	//--------------------------------TOP-------------------------------------//
	float cylinderTop = cylinderStart.z + cylinderHeight;
	float impactDistTopDisc = (cylinderTop - rayStart.z) / fwdNormal.z;
	Vec3 impactPosTopDisc = rayStart + impactDistTopDisc * fwdNormal;
	float cylinderXYProjectedTop = (cylinderStart - impactPosTopDisc).GetLengthXYSquared();

	// Check top disc
	if (fwdNormal.z < 0.f && rayStart.z >= cylinderTop && cylinderXYProjectedTop < cylinderRadiusSquared && impactDistTopDisc < rayLength)
	{
		raycastResult.m_didImpact = true;
		raycastResult.m_impactPos = impactPosTopDisc;
		raycastResult.m_impactNormal = Vec3::ZAXE;
		raycastResult.m_impactDist = impactDistTopDisc;
		return raycastResult;
	}
	//------------------------------------------------------------------------//
	return raycastResult;
}

RaycastResult3D RaycastVsOBB3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, OBB3 const& box)
{
	RaycastResult3D raycastResult;

	// Get local to world transform of oriented box
	Mat44 transform = Mat44(box.m_iBasis, box.m_jBasis, box.m_kBasis, box.m_center);
	Mat44 localToWorldTransform = transform.GetOrthonormalInverse();
	AABB3 identityBox = AABB3(-box.m_halfDimensions, box.m_halfDimensions);

	// Transform to AABB space
	Vec3 localRayStart = localToWorldTransform.TransformPosition3D(rayStart);
	Vec3 localFwdNormal = localToWorldTransform.TransformVectorQuantity3D(fwdNormal);

	// After transform raycast vs AABB and solve with t
	RaycastResult3D raycastVsAABB3 = RaycastVsAABB3D(localRayStart, localFwdNormal, rayLength, identityBox);
	Vec3 rayImpactPos = transform.TransformPosition3D(raycastVsAABB3.m_impactPos);
	Vec3 rayImpactNormal = transform.TransformVectorQuantity3D(raycastVsAABB3.m_impactNormal);

	// Set ray results and return resulting raycast
	raycastResult.m_didImpact = raycastVsAABB3.m_didImpact;
	raycastResult.m_impactDist = raycastVsAABB3.m_impactDist;
	raycastResult.m_impactPos = rayImpactPos;
	raycastResult.m_impactNormal = rayImpactNormal;
	return raycastResult;
}

RaycastResult3D RaycastVsPlane3D(Vec3 rayStart, Vec3 fwdNormal, float rayLength, Plane3 const& plane)
{
	RaycastResult3D raycastResult;
	Vec3 planeCenter = plane.GetPlaneCenter();
	Vec3 planeNormalized = plane.m_normal.GetNormalized();
	float dotStartNormal = DotProduct3D(fwdNormal, planeNormalized);

	// Early out Parallel check
	if (fabsf(dotStartNormal) < 0.000001f)
	{
		return raycastResult;
	}

	float dotNormalCenter = DotProduct3D(planeNormalized, planeCenter);
	float t = (dotNormalCenter - DotProduct3D(planeNormalized, rayStart)) / dotStartNormal;

	// Check for not intersecting
	if (t < 0.f || t > rayLength)
	{
		return raycastResult;
	}

	raycastResult.m_didImpact = true;
	raycastResult.m_impactPos = rayStart + fwdNormal * t;
	if (dotStartNormal > 0.f)
	{
		raycastResult.m_impactNormal = -planeNormalized;
	}
	else
	{
		raycastResult.m_impactNormal = planeNormalized;
	}
	return raycastResult;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

RaycastResult2D GetNearestImpactOnBounds(RaycastResult2D const& raycastResult1, RaycastResult2D raycastResult2, RaycastResult2D raycastResult3, RaycastResult2D raycastResult4)
{
	float nearestImpactDist = 999999.f;
	RaycastResult2D nearestResult;

	if (raycastResult1.m_didImpact && raycastResult1.m_impactDist < nearestImpactDist)
	{
		nearestImpactDist = raycastResult1.m_impactDist;
		nearestResult = raycastResult1;
	}
	if (raycastResult2.m_didImpact && raycastResult2.m_impactDist < nearestImpactDist)
	{
		nearestImpactDist = raycastResult2.m_impactDist;
		nearestResult = raycastResult2;
	}
	if (raycastResult3.m_didImpact && raycastResult3.m_impactDist < nearestImpactDist)
	{
		nearestImpactDist = raycastResult3.m_impactDist;
		nearestResult = raycastResult3;
	}
	if (raycastResult4.m_didImpact && raycastResult4.m_impactDist < nearestImpactDist)
	{
		nearestImpactDist = raycastResult4.m_impactDist;
		nearestResult = raycastResult4;
	}

	return nearestResult;
}

void AxisIntersection(float boxMin, float boxMax, float rayStart, float rayDir, float& tMin, float& tMax)
{
	tMin = (boxMin - rayStart) / rayDir;
	tMax = (boxMax - rayStart) / rayDir;

	if (tMin > tMax)
	{
		std::swap(tMin, tMax);
	}
}

Ray2::Ray2(Vec2 const& startPos, Vec2 const& endPos)
	:m_startPos(startPos), m_endPos(endPos)
{
}

Ray2::Ray2(Vec2 const& startPos, Vec2 const& fwdNormal, float maxLength)
	:m_startPos(startPos), m_fwdNormal(fwdNormal), m_maxLength(maxLength)
{
}