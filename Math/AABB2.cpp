#include "Engine/Math/AABB2.h"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
const AABB2 AABB2::ZERO_TO_ONE(0.f, 0.f, 1.f, 1.f);
// -----------------------------------------------------------------------------
AABB2::AABB2(AABB2 const& copyFrom)
{
	m_mins = copyFrom.m_mins;
	m_maxs = copyFrom.m_maxs;
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	m_mins.x = minX;
	m_mins.y = minY;
	m_maxs.x = maxX;
	m_maxs.y = maxY;
}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}

bool AABB2::IsPointInside(Vec2 const& point) const
{
	if (point.x >= m_mins.x && point.x <= m_maxs.x &&
		point.y >= m_mins.y && point.y <= m_maxs.y)
	{
		return true;
	}

	return false;
}

Vec2 const AABB2::GetCenter() const
{
	float x = (m_mins.x + m_maxs.x) * 0.5f;
	float y = (m_maxs.y + m_mins.y) * 0.5f;
	return Vec2(x, y);
}

Vec2 const AABB2::GetDimensions() const
{
	float x = m_maxs.x - m_mins.x;
	float y = m_maxs.y - m_mins.y;
	return Vec2(x, y);
}

Vec2 const AABB2::GetNearestPoint(Vec2 const& referencePosition) const
{
	float x = GetClamped(referencePosition.x, m_mins.x, m_maxs.x);
	float y = GetClamped(referencePosition.y, m_mins.y, m_maxs.y);
	return Vec2(x, y);
}

Vec2 const AABB2::GetPointAtUV(Vec2 const& uv) const
{
	float x = Interpolate(m_mins.x, m_maxs.x, uv.x);
	float y = Interpolate(m_mins.y, m_maxs.y, uv.y);
	return Vec2(x, y);
}

Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{
	float x = (point.x - m_mins.x) / GetDimensions().x;
	float y = (point.y - m_mins.y) / GetDimensions().y;
	return Vec2(x, y);
}

AABB2 const AABB2::GetBoxAtUVs(Vec2 const& uvMins, Vec2 const& uvMaxs)
{
	return AABB2(GetPointAtUV(uvMins), GetPointAtUV(uvMaxs));
}

void AABB2::Translate(Vec2 const& translationToApply)
{
	m_mins += translationToApply;
	m_maxs += translationToApply;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 dimensions = GetDimensions();
	m_mins = newCenter - dimensions * 0.5f;
	m_maxs = newCenter + dimensions * 0.5f;
}

void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	Vec2 center = GetCenter();
	m_mins = center - newDimensions * 0.5f;
	m_maxs = center + newDimensions * 0.5f;
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (point.x < m_mins.x)
	{
		m_mins.x = point.x;
	}
	if (point.x > m_maxs.x)
	{
		m_maxs.x = point.x;
	}
	if (point.y < m_mins.y)
	{
		m_mins.y = point.y;
	}
	if (point.y > m_maxs.y)
	{
		m_maxs.y = point.y;
	}
}

void AABB2::AddPadding(float xToAddOnBothSides, float yToAddToTopAndBottom)
{
	// Padding mins
	m_mins.x -= xToAddOnBothSides;
	m_mins.y -= yToAddToTopAndBottom;

	// Padding maxs
	m_maxs.x += xToAddOnBothSides;
	m_maxs.y += yToAddToTopAndBottom;
}

void AABB2::ReduceToNewAspect(float newAspect)
{
	Vec2 currentDimensions = GetDimensions();
	Vec2 currentCenter = GetCenter();
	float currentAspect = (currentDimensions.x / currentDimensions.y);

	if (newAspect < currentAspect)
	{
		float newAspectWidth = currentDimensions.y * newAspect;
		SetCenter(currentCenter);
		SetDimensions(Vec2(newAspectWidth, currentDimensions.y));
	}
	else
	{
		float newAspectHeight = currentDimensions.x / newAspect;
		SetCenter(currentCenter);
		SetDimensions(Vec2(currentDimensions.x, newAspectHeight));
	}
}

void AABB2::EnlargeToNewAspect(float newAspect)
{
	Vec2 currentDimensions = GetDimensions();
	Vec2 currentCenter = GetCenter();
	float currentAspect = (currentDimensions.x / currentDimensions.y);

	if (newAspect < currentAspect)
	{
		float newAspectWidth = currentDimensions.y / newAspect;
		SetCenter(currentCenter);
		SetDimensions(Vec2(newAspectWidth, currentDimensions.y));
	}
	else
	{
		float newAspectHeight = currentDimensions.x * newAspect;
		SetCenter(currentCenter);
		SetDimensions(Vec2(currentDimensions.x, newAspectHeight));
	}
}

AABB2 AABB2::ChopOffTop(float percentOfOriginalToChop, float extraHeightOfOriginalToChop)
{
	float chopY = (m_maxs.y - m_mins.y);
	float chopHeight = chopY * percentOfOriginalToChop + extraHeightOfOriginalToChop;
	m_maxs.y -= chopHeight;
	return *this;
}

AABB2 AABB2::ChopOffBottom(float percentOfOriginalToChop, float extraHeightOfOriginalToChop)
{
	float chopY = (m_maxs.y - m_mins.y);
	float chopHeight = chopY * percentOfOriginalToChop + extraHeightOfOriginalToChop;
	m_mins.y += chopHeight;
	return *this;
}

AABB2 AABB2::ChopOffLeft(float percentOfOriginalToChop, float extraHeightOfOriginalToChop)
{
	float chopX = (m_maxs.x - m_mins.x);
	float chopWidth = chopX * percentOfOriginalToChop + extraHeightOfOriginalToChop;
	m_mins.x += chopWidth;
	return *this;
}

AABB2 AABB2::ChopOffRight(float percentOfOriginalToChop, float extraHeightOfOriginalToChop)
{
	float chopX = (m_maxs.x - m_mins.x);
	float chopWidth = chopX * percentOfOriginalToChop + extraHeightOfOriginalToChop;
	m_maxs.x -= chopWidth;
	return *this;
}
