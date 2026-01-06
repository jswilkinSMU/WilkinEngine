#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

SpriteSheet::SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout)
	:m_texture(texture)
{
	m_simpleGridLayout = simpleGridLayout;

	IntVec2 textureDimensions = texture.GetDimensions();
	int textureWidth = textureDimensions.x;
	int textureHeight = textureDimensions.y;

	float uNudge = 1.f / (static_cast<float>(textureWidth) * 128.f);
	float vNudge = 1.f / (static_cast<float>(textureHeight) * 128.f);

	int numSprites = simpleGridLayout.x * simpleGridLayout.y;
	for (int spriteIndex = 0; spriteIndex < numSprites; ++spriteIndex)
	{
		IntVec2 spriteCoords(spriteIndex % simpleGridLayout.x, simpleGridLayout.y - 1 - (spriteIndex / simpleGridLayout.x));
		Vec2 uvAtMins(static_cast<float>(spriteCoords.x) / static_cast<float>(simpleGridLayout.x), static_cast<float>(spriteCoords.y) / static_cast<float>(simpleGridLayout.y));
		Vec2 uvAtMaxs(static_cast<float>(spriteCoords.x + 1) / static_cast<float>(simpleGridLayout.x), static_cast<float>(spriteCoords.y + 1) / static_cast<float>(simpleGridLayout.y));
		
		uvAtMins.x += uNudge;
		uvAtMins.y += vNudge;
		uvAtMaxs.x -= uNudge;
		uvAtMaxs.y -= vNudge;

		SpriteDefinition spriteDefintion(*this, spriteIndex, uvAtMins, uvAtMaxs);
		m_sprites.push_back(spriteDefintion);
	}
}

Texture& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprites() const
{
	return static_cast<int>(m_sprites.size());
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	return m_sprites[spriteIndex];
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	SpriteDefinition const& spriteDefinition = GetSpriteDef(spriteIndex);
	spriteDefinition.GetUVs(out_uvAtMins, out_uvAtMaxs);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	SpriteDefinition const& spriteDefinition = GetSpriteDef(spriteIndex);
	return spriteDefinition.GetUVs();
}

int SpriteSheet::GetSpriteUVCoords(IntVec2 const& spriteUVCoords) const
{
	int spriteIndex = spriteUVCoords.y * m_simpleGridLayout.x + spriteUVCoords.x;
	return spriteIndex;
}

AABB2 SpriteSheet::GetSpriteUVCoords(IntVec2 const& spriteUVCoords)
{
	int spriteIndex = spriteUVCoords.y * m_simpleGridLayout.x + spriteUVCoords.x;
	return m_sprites[spriteIndex].GetUVs();
}
