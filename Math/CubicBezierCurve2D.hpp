#pragma once
#include "Engine/Math/Vec2.hpp"
// -----------------------------------------------------------------------------
class  CubicHermiteCurve2D;
// -----------------------------------------------------------------------------
class CubicBezierCurve2D
{
public:
	CubicBezierCurve2D();
	CubicBezierCurve2D(Vec2 posA, Vec2 posB, Vec2 posC, Vec2 posD);
	explicit CubicBezierCurve2D(CubicHermiteCurve2D const& fromHermite);
	Vec2 EvaluateAtParametric(float parametricZeroToOne) const;
	float GetApproximateLength(int numSubdivisions = 64) const;
	Vec2 EvaluateApproximateDistance(float distanceAlongCurve, int numSubdivisions = 64) const;
// -----------------------------------------------------------------------------
	Vec2 m_positionA = Vec2::ZERO;
	Vec2 m_positionB = Vec2::ZERO;
	Vec2 m_positionC = Vec2::ZERO;
	Vec2 m_positionD = Vec2::ZERO;
};