#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Rgba8.h"
#include "Engine/Math/Vec3.h"

struct Vertex_PCU
{
public:
	Vec3 m_position;
	Rgba8 m_color;
	Vec2 m_uvTexCoords;

	Vertex_PCU();

	explicit Vertex_PCU(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords);
	explicit Vertex_PCU(Vec3 const& position, Rgba8 const& color);
	explicit Vertex_PCU(Vec2 const& position, Rgba8 const& color);
	explicit Vertex_PCU(Vec2 const& position, Rgba8 const& color, Vec2 const& uvTexCoords);
};