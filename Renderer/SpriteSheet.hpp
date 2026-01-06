#pragma once
#include "Engine/Math/IntVec2.h"
#include "Engine/Math/AABB2.h"
#include "Engine/Core/VertexUtils.h"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include <vector>

// ------------------------------------
class SpriteDefinition;
class Texture;
// ------------------------------------
class SpriteSheet
{
public:
	explicit SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout);

	Texture& GetTexture() const;
	int GetNumSprites() const;
	SpriteDefinition const& GetSpriteDef(int spriteIndex) const;
	void GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	AABB2 GetSpriteUVs(int spriteIndex) const;
	int GetSpriteUVCoords(IntVec2 const& spriteUVCoords) const;
	AABB2 GetSpriteUVCoords(IntVec2 const& spriteUVCoords);

protected:
	Texture& m_texture;
	IntVec2  m_simpleGridLayout = IntVec2::ZERO;
	std::vector<SpriteDefinition> m_sprites;
};