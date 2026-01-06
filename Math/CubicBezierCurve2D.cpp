#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Math/CubicHermiteCurve2D.hpp"
#include "Engine/Math/MathUtils.h"

CubicBezierCurve2D::CubicBezierCurve2D()
{
}

CubicBezierCurve2D::CubicBezierCurve2D(Vec2 posA, Vec2 posB, Vec2 posC, Vec2 posD)
	:m_positionA(posA),
	 m_positionB(posB),
	 m_positionC(posC),
	 m_positionD(posD)
{
}

CubicBezierCurve2D::CubicBezierCurve2D(CubicHermiteCurve2D const& fromHermite)
{
	m_positionA = fromHermite.m_startPos;
	m_positionB = fromHermite.m_startPos + fromHermite.m_startVel * 0.33f;
	m_positionC = fromHermite.m_endPos - fromHermite.m_endVel * 0.33f;
	m_positionD = fromHermite.m_endPos;
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	Vec2 r0 = Interpolate(m_positionA, m_positionB, parametricZeroToOne);
	Vec2 r1 = Interpolate(m_positionB, m_positionC, parametricZeroToOne);
	Vec2 r2 = Interpolate(m_positionC, m_positionD, parametricZeroToOne);

	Vec2 s0 = Interpolate(r0, r1, parametricZeroToOne);
	Vec2 s1 = Interpolate(r1, r2, parametricZeroToOne);

	Vec2 t0 = Interpolate(s0, s1, parametricZeroToOne);
	return t0;
}

float CubicBezierCurve2D::GetApproximateLength(int numSubdivisions) const
{
	float approximateLength = 0.f;
	float u = 1.f / numSubdivisions;
	Vec2 startPos = m_positionA;
	for (int paraIndex = 0; paraIndex < numSubdivisions; ++paraIndex)
	{
		float t0 = u * (paraIndex + 1);
		Vec2 endPos = EvaluateAtParametric(t0);
		approximateLength += GetDistance2D(startPos, endPos);
		startPos = endPos;
	}
	return approximateLength;
}

Vec2 CubicBezierCurve2D::EvaluateApproximateDistance(float distanceAlongCurve, int numSubdivisions) const
{
	float u = 1.f / numSubdivisions;
	Vec2 startPosition = m_positionA;
	Vec2 currrentPosition = m_positionA;
	for (int paraIndex = 0; paraIndex < numSubdivisions; ++paraIndex)
	{
		float t0 = u * (paraIndex + 1);
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
