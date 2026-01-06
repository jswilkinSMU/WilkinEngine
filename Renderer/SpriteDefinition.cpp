#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

SpriteDefinition::SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs)
	:m_spriteSheet(spriteSheet), m_spriteIndex(spriteIndex), m_uvAtMins(uvAtMins), m_uvAtMaxs(uvAtMaxs)
{
}

void SpriteDefinition::GetUVs(Vec2& out_uvAtMins, Vec2& uv_outAtMaxs) const
{
	out_uvAtMins = m_uvAtMins;
	uv_outAtMaxs = m_uvAtMaxs;
}

AABB2 SpriteDefinition::GetUVs() const
{
	return AABB2(m_uvAtMins, m_uvAtMaxs);
}

SpriteSheet const& SpriteDefinition::GetSpriteSheet() const
{
	return m_spriteSheet;
}

Texture& SpriteDefinition::GetTexture() const
{
	return m_spriteSheet.GetTexture();
}

float SpriteDefinition::GetAspect() const
{
	Vec2 spriteSheetDimensions = GetUVs().GetDimensions();
	return spriteSheetDimensions.x / spriteSheetDimensions.y;
}
