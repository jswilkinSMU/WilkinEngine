#include "Vertex_PCU.h"

Vertex_PCU::Vertex_PCU()
    : m_position(Vec3(0.f, 0.f, 0.f)), m_color(Rgba8(0, 0, 0, 0)), m_uvTexCoords(Vec2(0.f, 0.f)) {}


Vertex_PCU::Vertex_PCU(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords)
    :m_position(position), m_color(color), m_uvTexCoords(uvTexCoords)
{
}

Vertex_PCU::Vertex_PCU(Vec3 const& position, Rgba8 const& color)
    :m_position(position), m_color(color)
{
}

Vertex_PCU::Vertex_PCU(Vec2 const& position, Rgba8 const& color)
    :m_position(Vec3(position.x, position.y, 0.f)), m_color(color)
{
}

Vertex_PCU::Vertex_PCU(Vec2 const& position, Rgba8 const& color, Vec2 const& uvTexCoords)
    :m_position(Vec3(position.x, position.y, 0.f)), m_color(color), m_uvTexCoords(uvTexCoords)
{
}
