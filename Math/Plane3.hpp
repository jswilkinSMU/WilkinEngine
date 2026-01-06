#pragma once
#include "Engine/Math/Vec3.h"
// -----------------------------------------------------------------------------
struct Plane3
{
public:
	Vec3 m_normal = Vec3::ZAXE;
	float m_distance = 0.0f;

public:
	Plane3() = default;
	~Plane3() = default;
	Plane3(Plane3 const& copyFrom) = default;
	explicit Plane3(Vec3 const& normal, float distance);
// -----------------------------------------------------------------------------
	bool IsPointInFront(Vec3 const& referencePos) const;
	bool IsPointInBack(Vec3 const& referencePos) const;
	Vec3 const GetPlaneCenter() const;
};