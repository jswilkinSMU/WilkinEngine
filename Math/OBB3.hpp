#pragma once
#include "Engine/Math/Vec3.h"
// -----------------------------------------------------------------------------
struct OBB3
{
public:
	Vec3 m_center = Vec3::ZERO;
	Vec3 m_iBasis = Vec3::ZERO;
	Vec3 m_jBasis = Vec3::ZERO;
	Vec3 m_kBasis = Vec3::ZERO;
	Vec3 m_halfDimensions = Vec3::ZERO;

public:
	OBB3() = default;
	~OBB3() = default;
	OBB3(OBB3 const& copyFrom) = default;
	explicit OBB3(Vec3 const& center, Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis, Vec3 const& halfDimensions);

public:
	void GetCornerPoints(Vec3* out_fourCornerWorldPositions) const;
	Vec3 GetLocalPosForWorldPos(Vec3 const& worldPos) const;
	Vec3 GetWorldPosFromLocalPos(Vec3 const& localPos) const;
	Vec3 GetCenter() const;
	bool IsPointInside(Vec3 const& point) const;
};