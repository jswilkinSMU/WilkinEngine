#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/EngineCommon.h"

OBB2::OBB2(OBB2 const& copyFrom)
{
	m_center = copyFrom.m_center;
	m_iBasisNormal = copyFrom.m_iBasisNormal;
	m_halfDimensions = copyFrom.m_halfDimensions;
}

OBB2::OBB2(Vec2 const& center, Vec2 const& iBasisNormal, Vec2 const& halfDimensions)
{
	m_center = center;
	m_iBasisNormal = iBasisNormal;
	m_halfDimensions = halfDimensions;
}

void OBB2::GetCornerPoints(Vec2* out_fourCornerWorldPositions) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();

	float halfWidth = m_halfDimensions.x;
	float halfHeight = m_halfDimensions.y;

	out_fourCornerWorldPositions[0] = m_center + (m_iBasisNormal * halfWidth) + (jBasisNormal * halfHeight);
	out_fourCornerWorldPositions[1] = m_center + (m_iBasisNormal * halfWidth) - (jBasisNormal * halfHeight); 
	out_fourCornerWorldPositions[2] = m_center - (m_iBasisNormal * halfWidth) - (jBasisNormal * halfHeight); 
	out_fourCornerWorldPositions[3] = m_center - (m_iBasisNormal * halfWidth) + (jBasisNormal * halfHeight);
}

Vec2 const OBB2::GetLocalPosForWorldPos(Vec2 const& worldPos) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();
	Vec2 centerToWorld = worldPos - m_center;

	float iDotCenter = DotProduct2D(centerToWorld, m_iBasisNormal);
	float jDotCenter = DotProduct2D(centerToWorld, jBasisNormal);
	Vec2  localPosition = Vec2(iDotCenter, jDotCenter);

	return localPosition;
}

Vec2 const OBB2::GetWorldPosForLocalPos(Vec2 const& localPos) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();
	Vec2 worldX = m_iBasisNormal * localPos.x;
	Vec2 worldY = jBasisNormal * localPos.y;

	Vec2 worldPosition = m_center + worldX + worldY;
	return worldPosition;
}

bool OBB2::IsPointInside(Vec2 const& point) const
{
	Vec2 pointLocalPos = GetLocalPosForWorldPos(point);
	return (pointLocalPos.x >= -m_halfDimensions.x && pointLocalPos.x <= m_halfDimensions.x) 
		&& (pointLocalPos.y >= -m_halfDimensions.y && pointLocalPos.y <= m_halfDimensions.y);
}

void OBB2::RotateAboutCenter(float rotationDeltaDegrees)
{
	m_iBasisNormal.RotateDegrees(rotationDeltaDegrees);
}
