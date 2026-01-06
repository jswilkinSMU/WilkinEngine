#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/MathUtils.h"

Plane3::Plane3(Vec3 const& normal, float distance)
	:m_normal(normal),
	 m_distance(distance)
{
}

bool Plane3::IsPointInFront(Vec3 const& referencePos) const
{
	float dotRefToNormal = DotProduct3D(referencePos, m_normal);
	if (dotRefToNormal >= m_distance)
	{
		return true;
	}
	return false;
}

bool Plane3::IsPointInBack(Vec3 const& referencePos) const
{
	float dotRefToNormal = DotProduct3D(referencePos, m_normal);
	if (dotRefToNormal <= m_distance)
	{
		return true;
	}
	return false;
}

Vec3 const Plane3::GetPlaneCenter() const
{
	return m_normal * m_distance;
}

