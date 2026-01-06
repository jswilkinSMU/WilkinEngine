#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/MathUtils.h"

OBB3::OBB3(Vec3 const& center, Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& halfDimensions)
	:m_center(center),
	 m_iBasis(iBasis),
	 m_jBasis(jBasis),
	 m_kBasis(kBasis),
	 m_halfDimensions(halfDimensions)
{
}

void OBB3::GetCornerPoints(Vec3* out_fourCornerWorldPositions) const
{
	float halfFwd = m_halfDimensions.x;
	float halfLeft = m_halfDimensions.y;
	float halfUp = m_halfDimensions.z;

	out_fourCornerWorldPositions[0] = m_center - (m_iBasis * halfFwd) - (m_jBasis * halfLeft) - (m_kBasis * halfUp); // bottomRightBack
	out_fourCornerWorldPositions[1] = m_center - (m_iBasis * halfFwd) - (m_jBasis * halfLeft) + (m_kBasis * halfUp); // bottomLeftBack
	out_fourCornerWorldPositions[2] = m_center - (m_iBasis * halfFwd) + (m_jBasis * halfLeft) - (m_kBasis * halfUp); // bottomRightFront
	out_fourCornerWorldPositions[3] = m_center - (m_iBasis * halfFwd) + (m_jBasis * halfLeft) + (m_kBasis * halfUp); // bottomLeftFront
	out_fourCornerWorldPositions[4] = m_center + (m_iBasis * halfFwd) - (m_jBasis * halfLeft) - (m_kBasis * halfUp); // topRightBack
	out_fourCornerWorldPositions[5] = m_center + (m_iBasis * halfFwd) - (m_jBasis * halfLeft) + (m_kBasis * halfUp); // topLeftBack
	out_fourCornerWorldPositions[6] = m_center + (m_iBasis * halfFwd) + (m_jBasis * halfLeft) - (m_kBasis * halfUp); // topRightFront
	out_fourCornerWorldPositions[7] = m_center + (m_iBasis * halfFwd) + (m_jBasis * halfLeft) + (m_kBasis * halfUp); // topLeftFront
}

Vec3 OBB3::GetLocalPosForWorldPos(Vec3 const& worldPos) const
{
	Vec3 localPosition = worldPos - m_center;
	float localXPos = DotProduct3D(localPosition, m_iBasis);
	float localYPos = DotProduct3D(localPosition, m_jBasis);
	float localZPos = DotProduct3D(localPosition, m_kBasis);
	return Vec3(localXPos, localYPos, localZPos);
}

Vec3 OBB3::GetWorldPosFromLocalPos(Vec3 const& localPos) const
{
	Vec3 iBasisNormalized = m_iBasis.GetNormalized();
	Vec3 jBasisNormalized = m_jBasis.GetNormalized();
	Vec3 kBasisNormalized = m_kBasis.GetNormalized();

	Vec3 worldPos = m_center + (localPos.x * iBasisNormalized) + (localPos.y * jBasisNormalized) + (localPos.z * kBasisNormalized);
	return worldPos;
}

Vec3 OBB3::GetCenter() const
{
	return m_center;
}

bool OBB3::IsPointInside(Vec3 const& point) const
{
	Vec3 pointLocalPos = GetLocalPosForWorldPos(point);
	return (pointLocalPos.x >= -m_halfDimensions.x && pointLocalPos.x <= m_halfDimensions.x)
		&& (pointLocalPos.y >= -m_halfDimensions.y && pointLocalPos.y <= m_halfDimensions.y)
		&& (pointLocalPos.z >= -m_halfDimensions.z && pointLocalPos.z <= m_halfDimensions.z);
}