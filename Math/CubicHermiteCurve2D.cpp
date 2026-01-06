#include "Engine/Math/CubicHermiteCurve2D.hpp"
#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Math/MathUtils.h"

CubicHermiteCurve2D::CubicHermiteCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel)
	:m_startPos(startPos),
	 m_startVel(startVel),
	 m_endPos(endPos),
	 m_endVel(endVel)
{
}

CubicHermiteCurve2D::CubicHermiteCurve2D(CubicBezierCurve2D const& fromBezier)
{
	m_startPos = fromBezier.m_positionA;
	m_startVel = (fromBezier.m_positionB - fromBezier.m_positionA) * 3;
	m_endVel   = (fromBezier.m_positionD - fromBezier.m_positionC) * 3;
	m_endPos   = fromBezier.m_positionD;
}

Vec2 CubicHermiteCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	Vec2 r0 = Interpolate(m_startPos, m_startVel, parametricZeroToOne);
	Vec2 r1 = Interpolate(m_startVel, m_endVel, parametricZeroToOne);
	Vec2 r2 = Interpolate(m_endVel, m_endPos, parametricZeroToOne);

	Vec2 s0 = Interpolate(r0, r1, parametricZeroToOne);
	Vec2 s1 = Interpolate(r1, r2, parametricZeroToOne);

	Vec2 t0 = Interpolate(s0, s1, parametricZeroToOne);
	return t0;
}

float CubicHermiteCurve2D::GetApproximateLength(int numSubdivisions) const
{
	float approximateLength = 0.f;
	float u = 1.f / numSubdivisions;
	Vec2 startPos = m_startPos;
	for (int i = 0; i < numSubdivisions; ++i)
	{
		float t0 = u * ((i + 1));
		Vec2 endPos = EvaluateAtParametric(t0);
		approximateLength += GetDistance2D(startPos, endPos);
		startPos = endPos;
	}
	return approximateLength;
}

Vec2 CubicHermiteCurve2D::EvaluateApproximateDistance(float distanceAlongCurve, int numSubdivisions) const
{
	float u = 1.f / numSubdivisions;
	Vec2 startPosition = m_startPos;
	Vec2 currrentPosition = m_startPos;
	for (int i = 0; i < numSubdivisions; ++i)
	{
		float t0 = u * (i + 1);
		Vec2 endPosition = EvaluateAtParametric(t0);
		float approximateDistance = GetDistance2D(currrentPosition, endPosition);
		if (approximateDistance >= distanceAlongCurve)
		{
			Vec2 direction = endPosition - currrentPosition;
			direction.Normalize();
			return currrentPosition + direction * distanceAlongCurve;
		}
		distanceAlongCurve -= approximateDistance;
		currrentPosition = endPosition;
	}
	return currrentPosition;
}
