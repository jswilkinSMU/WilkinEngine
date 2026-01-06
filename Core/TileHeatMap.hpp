#pragma once
#include "Engine/Math/IntVec2.h"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/Vertex_PCU.h"
#include <vector>

struct AABB2;

// -----------------------------------------------------------------------------
class TileHeatMap
{
public:
	TileHeatMap() = default;
	explicit TileHeatMap(IntVec2 const& dimensions);
	~TileHeatMap() = default;
	void  AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 totalBounds, FloatRange valueRange = FloatRange(0.f, 1.f), Rgba8 lowColor = Rgba8(0, 0, 0, 100), Rgba8 highColor = Rgba8(255, 255, 255, 100), float specialValue = 999999.f, Rgba8 specialColor = Rgba8(255, 0, 255));
	void  AddHeatValue(IntVec2 const& tileCoordinates, float heatValue);
	FloatRange GetRangeOfValuesExcludingSpecial(float specialValueToIgnore);

public:
	float GetHeatValue(IntVec2 const& tileCoordinates) const;
	float GetHeatValue(int tileX, int tileY) const;
	int	  GetNumTiles() const;

	void  SetAllValues(float value);
	void  SetHeatValue(IntVec2 const& tileCoordinates, float heatValue);
	void  SetHeatValue(int tileX, int tileY, float heatValue);
	void  SetHeatValueAtIndex(int tileIndex, float heatValue);

private:
	IntVec2 m_dimensions;
	std::vector<float> m_values;
};