#pragma once
#include "Engine/Math/Vec2.hpp"
#include <vector>
// -----------------------------------------------------------------------------
class Spline
{
public:
	Spline() = default;
	Spline(std::vector<Vec2> positions);
	Spline(std::vector<Vec2> positions, std::vector<Vec2> velocities);
	Vec2 EvaluateAtParametric(float parametricZeroToOne) const;
	Vec2 CatmullRomSpline(Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, Vec2 const& p3, float t) const;
// -----------------------------------------------------------------------------
	std::vector<Vec2> m_positions;
	std::vector<Vec2> m_velocites;
};