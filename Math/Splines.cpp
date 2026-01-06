#include "Engine/Math/Splines.hpp"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Math/MathUtils.h"

Spline::Spline(std::vector<Vec2> positions)
    :m_positions(positions)
{
    m_velocites.resize(m_positions.size());

    // Set start velocity to zero
    m_velocites[0] = Vec2::ZERO;

    // Set end velocity to zero
    size_t endIndex = m_velocites.size() - 1;
    m_velocites[endIndex] = Vec2::ZERO;

    // Implement CatMull-Rom algorithm: velocity[i] = (position[i+1] – position[i-1]) / 2
	for (int i = 1; i < static_cast<int>(m_positions.size()) - 1; ++i)
	{
		m_velocites[i] = (m_positions[i+1] - m_positions[i-1]) * 0.5f;
	}
}

Spline::Spline(std::vector<Vec2> positions, std::vector<Vec2> velocities)
    :m_positions(positions), m_velocites(velocities)
{
    m_velocites.resize(m_positions.size());

	// Set start velocity to zero
	m_velocites[0] = Vec2::ZERO;

	// Set end velocity to zero
	size_t endIndex = m_velocites.size() - 1;
	m_velocites[endIndex] = Vec2::ZERO;

	// Implement CatMull-Rom algorithm: velocity[i] = (position[i+1] – position[i-1]) / 2
	for (int i = 1; i < static_cast<int>(m_positions.size()) - 1; ++i)
	{
		m_velocites[i] = (m_positions[i+1] - m_positions[i-1]) * 0.5f;
	}
}

Vec2 Spline::EvaluateAtParametric(float parametricZeroToOne) const
{
	int numSplinePoints = static_cast<int>(m_positions.size());
	float segmentLength = 1.0f / (numSplinePoints - 1);

	int segmentIndex = static_cast<int>(parametricZeroToOne / segmentLength);
	segmentIndex = GetClamped(segmentIndex, 0, numSplinePoints - 2);
	float t0 = (parametricZeroToOne - segmentIndex * segmentLength) / segmentLength;

	Vec2 p0 = m_positions[std::max(0, segmentIndex - 1)];
	Vec2 p1 = m_positions[segmentIndex];
	Vec2 p2 = m_positions[std::min(segmentIndex + 1, numSplinePoints - 1)];
	Vec2 p3 = m_positions[std::min(segmentIndex + 2, numSplinePoints - 1)];

	return CatmullRomSpline(p0, p1, p2, p3, t0);
}

Vec2 Spline::CatmullRomSpline(Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, Vec2 const& p3, float t) const
{
	float t2 = t * t;
	float t3 = t * t * t;

	Vec2 n0 = (2.f * p1);
	Vec2 n1 = (-p0 + p2) * t;
	Vec2 n2 = ((2 * p0) - (5 * p1) + (4 * p2) - p3) * t2;
	Vec2 n3 = ((-p0 + (3 * p1) - (3 * p2) + p3)) * t3;

	Vec2 qT = 0.5f * (n0 + n1 + n2 + n3);
	return qT;
}
