#include "Engine/Core/Rgba8Gradient.hpp"
#include <algorithm>
// -----------------------------------------------------------------------------
GradientStop::GradientStop(float position, Rgba8 const& color)
	:m_position(position),
	 m_color(color)
{
}
// -----------------------------------------------------------------------------
void Rgba8Gradient::AddStop(float position, Rgba8 const& color)
{
	GradientStop newGradientStop(position, color);

	auto foundStop = std::lower_bound(m_stops.begin(), m_stops.end(), newGradientStop,
	[](GradientStop const& stopA, GradientStop const& stopB)
	{
		return stopA.m_position < stopB.m_position;
	});

	m_stops.insert(foundStop, newGradientStop);
}
// -----------------------------------------------------------------------------
Rgba8 Rgba8Gradient::Evaluate(float t) const
{
	// First check if we have any stops
	if (m_stops.empty())
	{
		return Rgba8::WHITE;
	}

	// Clamping to the edges
	if (t <= m_stops.front().m_position)
	{
		return m_stops.front().m_color;
	}
	if (t >= m_stops.back().m_position)
	{
		return m_stops.back().m_color;
	}

	// Finding the surrounding stops
	for (int stopIndex = 0; stopIndex < static_cast<int>(m_stops.size() - 1); ++stopIndex)
	{
		GradientStop const& stopA = m_stops[stopIndex];
		GradientStop const& stopB = m_stops[stopIndex + 1];

		if (t >= stopA.m_position && t <= stopB.m_position)
		{
			float localT = (t - stopA.m_position) / (stopB.m_position - stopA.m_position);
			return Rgba8::Rgba8Interpolate(stopA.m_color, stopB.m_color, localT);
		}
	}

	return Rgba8::WHITE;
}
// -----------------------------------------------------------------------------