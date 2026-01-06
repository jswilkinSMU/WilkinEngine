#include "Engine/Core/TileHeatMap.hpp"
#include "Engine/Core/VertexUtils.h"
#include "Engine/Core/EngineCommon.h"
#include "Engine/Math/AABB2.h"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/MathUtils.h"

TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
	:m_dimensions(dimensions)
{
	m_values.resize(m_dimensions.x * m_dimensions.y, 0.0f);
}

void TileHeatMap::AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 totalBounds, FloatRange valueRange, Rgba8 lowColor, Rgba8 highColor, float specialValue, Rgba8 specialColor)
{
	Vec2 gridBottomLeft = totalBounds.m_mins;
	Vec2 gridTopRight = totalBounds.m_maxs;
	Vec2 tileSize = Vec2((gridTopRight.x - gridBottomLeft.x) / m_dimensions.x, (gridTopRight.y - gridBottomLeft.y) / m_dimensions.y);

	for (int tileY = 0; tileY < m_dimensions.y; ++tileY)
	{
		for (int tileX = 0; tileX < m_dimensions.x; ++tileX)
		{
			Vec2 tileBottomLeft = gridBottomLeft + Vec2(tileX * tileSize.x, tileY * tileSize.y);
			float heatValue = GetHeatValue(IntVec2(tileX, tileY));
			Rgba8 tileColor;

			if (heatValue == specialValue)
			{
				tileColor = specialColor;
			}
			else
			{
				float heatValueClamped = RangeMapClamped(heatValue, valueRange.m_min, valueRange.m_max, 0.f, 1.f);
				tileColor = lowColor.Rgba8Interpolate(lowColor, highColor, heatValueClamped);
			}

			AABB2 tileBounds(tileBottomLeft, tileBottomLeft + tileSize);
			Vec2 uvMins(0.f, 0.f);
			Vec2 uvMaxs(1.f, 1.f);
			AddVertsForAABB2D(verts, tileBounds, tileColor, uvMins, uvMaxs);
		}
	}
}

void TileHeatMap::SetAllValues(float value)
{
	for (int valueIndex = 0; valueIndex < static_cast<int>(m_values.size()); ++valueIndex)
	{
		m_values[valueIndex] = value;
	}
}

float TileHeatMap::GetHeatValue(IntVec2 const& tileCoordinates) const
{
	int heatMapIndex = (tileCoordinates.y * m_dimensions.x) + tileCoordinates.x;
	return m_values[heatMapIndex];
}

float TileHeatMap::GetHeatValue(int tileX, int tileY) const
{
	return GetHeatValue(IntVec2(tileX, tileY));
}

int TileHeatMap::GetNumTiles() const
{
	return m_dimensions.x * m_dimensions.y;
}

void TileHeatMap::SetHeatValue(IntVec2 const& tileCoordinates, float heatValue)
{
	int heatMapIndex = (tileCoordinates.y * m_dimensions.x) + tileCoordinates.x;
	SetHeatValueAtIndex(heatMapIndex, heatValue);
}

void TileHeatMap::SetHeatValue(int tileX, int tileY, float heatValue)
{
	int heatMapIndex = (tileY * m_dimensions.x) + tileX;
	SetHeatValueAtIndex(heatMapIndex, heatValue);
}

void TileHeatMap::SetHeatValueAtIndex(int tileIndex, float heatValue)
{
	m_values[tileIndex] = heatValue;
}

void TileHeatMap::AddHeatValue(IntVec2 const& tileCoordinates, float heatValue)
{
	int heatMapIndex = (tileCoordinates.y * m_dimensions.x) + tileCoordinates.x;
	m_values[heatMapIndex] += heatValue;
}

FloatRange TileHeatMap::GetRangeOfValuesExcludingSpecial(float specialValueToIgnore)
{
	FloatRange rangeOfNonSpecialValues(FLT_MAX, -FLT_MAX);
	int numTiles = GetNumTiles();
	for (int tileIndex = 0; tileIndex < numTiles; ++tileIndex)
	{
		float value = m_values[tileIndex];
		if (value != specialValueToIgnore)
		{
			rangeOfNonSpecialValues.StretchToIncludeValue(value);
		}
	}
	return rangeOfNonSpecialValues;
}


