#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/MathUtils.h"
#include <corecrt_math.h>

AABB3::AABB3(AABB3 const& copyFrom)
{
	m_mins = copyFrom.m_mins;
	m_maxs = copyFrom.m_maxs;
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	m_mins.x = minX;
	m_mins.y = minY;
	m_mins.z = minZ;
	
	m_maxs.x = maxX;
	m_maxs.y = maxY;
	m_maxs.z = maxZ;
}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}

bool AABB3::IsPointInside(Vec3 const& point) const
{
	if (point.x >= m_mins.x && point.x <= m_maxs.x &&
		point.y >= m_mins.y && point.y <= m_maxs.y &&
		point.z >= m_mins.z && point.z <= m_maxs.z)
	{
		return true;
	}
	return false;
}

bool AABB3::Intersects(AABB3 const& box) const
{
	// Forward and Backward check
	if (m_maxs.x < box.m_mins.x || m_mins.x > box.m_maxs.x)
	{
		return false;
	}

	// Left and Right check
	if (m_mins.y > box.m_maxs.y || m_maxs.y < box.m_mins.y)
	{
		return false;
	}

	// Top and Bottom check
	if (m_mins.z > box.m_maxs.z || m_maxs.z < box.m_mins.z)
	{
		return false;
	}

	// If none of the checks are fulfilled, we are overlapping
	return true;
}

Vec3 const AABB3::GetCenter() const
{
	float x = (m_mins.x + m_maxs.x) * 0.5f;
	float y = (m_maxs.y + m_mins.y) * 0.5f;
	float z = (m_mins.z + m_maxs.z) * 0.5f;
	return Vec3(x, y, z);
}

Vec3 const AABB3::GetDimensions() const
{
	float x = m_maxs.x - m_mins.x;
	float y = m_maxs.y - m_mins.y;
	float z = m_maxs.z - m_mins.z;
	return Vec3(x, y, z);
}

Vec3 const AABB3::GetNearestPoint(Vec3 const& referencePosition) const
{
	float x = GetClamped(referencePosition.x, m_mins.x, m_maxs.x);
	float y = GetClamped(referencePosition.y, m_mins.y, m_maxs.y);
	float z = GetClamped(referencePosition.z, m_mins.z, m_maxs.z);
	return Vec3(x, y, z);
}

void AABB3::Translate(Vec3 const& translationToApply)
{
	m_mins += translationToApply;
	m_maxs += translationToApply;
}

void AABB3::SetCenter(Vec3 const& newCenter)
{
	Vec3 dimensions = GetDimensions();
	m_mins = newCenter - dimensions * 0.5f;
	m_maxs = newCenter + dimensions * 0.5f;
}

void AABB3::SetDimensions(Vec3 const& newDimensions)
{
	Vec3 center = GetCenter();
	m_mins = center - newDimensions * 0.5f;
	m_maxs = center + newDimensions * 0.5f;
}

void AABB3::StretchToIncludePoint(Vec3 const& point)
{
	// X Check
	if (point.x < m_mins.x)
	{
		m_mins.x = point.x;
	}
	if (point.x > m_maxs.x)
	{
		m_maxs.x = point.x;
	}

	// Y Check
	if (point.y < m_mins.y)
	{
		m_mins.y = point.y;
	}
	if (point.y > m_maxs.y)
	{
		m_maxs.y = point.y;
	}

	// Z Check
	if (point.z < m_mins.z)
	{
		m_mins.z = point.z;
	}
	if (point.z > m_maxs.z)
	{
		m_maxs.z = point.z;
	}
}
