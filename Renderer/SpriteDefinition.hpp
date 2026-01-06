#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.h"

// ------------------------------------
class Texture;
class SpriteSheet;
// ------------------------------------
class SpriteDefinition
{
public:
	explicit SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs);
	void GetUVs(Vec2& out_uvAtMins, Vec2& uv_outAtMaxs) const;
	AABB2 GetUVs() const;
	SpriteSheet const& GetSpriteSheet() const;
	Texture& GetTexture() const;
	float GetAspect() const;

protected:
	SpriteSheet const& m_spriteSheet;
	int m_spriteIndex = -1;
	Vec2 m_uvAtMins = Vec2::ZERO;
	Vec2 m_uvAtMaxs = Vec2::ONE;
};