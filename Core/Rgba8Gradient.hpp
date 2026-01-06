#pragma once
#include "Engine/Core/Rgba8.h"
#include <vector>
// -----------------------------------------------------------------------------
struct GradientStop
{
	GradientStop(float position, Rgba8 const& color);

	float m_position;
	Rgba8 m_color;
};
// -----------------------------------------------------------------------------
class Rgba8Gradient
{
public:
	void  AddStop(float position, Rgba8 const& color);
	Rgba8 Evaluate(float t) const;

private:
	std::vector<GradientStop> m_stops;
};
// -----------------------------------------------------------------------------