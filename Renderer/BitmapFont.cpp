#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.h"
#include "Engine/Core/EngineCommon.h"

BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture)
	:m_fontFilePathNameWithNoExtension(fontFilePathNameWithNoExtension),
	 m_fontGlyphsSpriteSheet(fontTexture, IntVec2(16, 16))
{
}

Texture& BitmapFont::GetTexture()
{
    return m_fontGlyphsSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspectScale)
{
	Vec2 startPosition = textMins;

	for (int textIndex = 0; textIndex < (int)text.size(); ++textIndex)
	{
		int glyphIndex = text[textIndex];
		float glyphAspect = GetGlyphAspect(glyphIndex);
		float glyphWidth = cellHeight * glyphAspect * cellAspectScale;
		float glyphHeight = cellHeight;

		AABB2 glyphCoords = m_fontGlyphsSpriteSheet.GetSpriteUVs(glyphIndex);
		AABB2 glyphAlignedBox(startPosition, startPosition + Vec2(glyphWidth, glyphHeight));

		AddVertsForAABB2D(vertexArray, glyphAlignedBox, tint, glyphCoords.m_mins, glyphCoords.m_maxs);
		startPosition.x += glyphWidth;
	}
}

void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, std::string const& text, AABB2 const& box, float cellHeight, Rgba8 const& tint, float cellAspectScale, Vec2 const& alignment, TextBoxMode mode, int maxGlyphsToDraw)
{
	Strings textLines = SplitStringOnDelimiter(text, '\n');

	float adjustedCellHeight = cellHeight;

	if (mode == TextBoxMode::SHRINK_TO_FIT)
	{
		float paragraphHeight = static_cast<float>(textLines.size()) * cellHeight;
		float paragraphWidth = 0.f;
		for (int lineIndex = 0; lineIndex < (int)textLines.size(); ++lineIndex)
		{
			float lineWidth = GetTextWidth(cellHeight, textLines[lineIndex], cellAspectScale);
			if (lineWidth > paragraphWidth)
			{
				paragraphWidth = lineWidth;
			}
		}
		if (paragraphWidth > box.GetDimensions().x || paragraphHeight > box.GetDimensions().y)
		{
			float widthProportion = box.GetDimensions().x / paragraphWidth; // Possibly needs a name change though I think proportion is understandable 
			float heightProportion = box.GetDimensions().y / paragraphHeight;
			float minimumBoxProportion = 0.f;

			if (widthProportion < heightProportion)
			{
				minimumBoxProportion = widthProportion;
			}
			else
			{
				minimumBoxProportion = heightProportion;
			}
			adjustedCellHeight *= minimumBoxProportion;
		}
	}

	std::vector<AABB2> textBoxes;
	textBoxes.reserve(textLines.size());
	for (int lineIndex = 0; lineIndex < (int)textLines.size(); ++lineIndex)
	{
		float lineWidth = GetTextWidth(cellHeight, textLines[lineIndex], cellAspectScale);
		textBoxes.push_back(AABB2(Vec2::ZERO, Vec2(lineWidth, adjustedCellHeight)));
	}

	// Alignment configuration
	float boxCenterX = Interpolate(box.m_mins.x, box.m_maxs.x, alignment.x);
	float boxCenterY = Interpolate(box.m_mins.y, box.m_maxs.y, alignment.y);
	float paragraphHeight = static_cast<float>(textLines.size() * adjustedCellHeight);
	for (int textBoxIndex = 0; textBoxIndex < (int)textBoxes.size(); ++textBoxIndex)
	{
		AABB2& textBox = textBoxes[textBoxIndex];
		float textBoxCenterX = Interpolate(textBox.m_mins.x, textBox.m_maxs.x, alignment.x);
		Vec2 translationAcrossBox;
		translationAcrossBox.x = boxCenterX - textBoxCenterX;
		translationAcrossBox.y = boxCenterY - adjustedCellHeight * static_cast<float>(textBoxIndex + 1) + (1.f - alignment.y) + paragraphHeight;
		textBox.Translate(translationAcrossBox);
	}

    // Add verts within maxGlyphsToDraw
	int glyphsDrawn = 0;
	for (int textBoxIndex = 0; textBoxIndex < (int)textBoxes.size(); ++textBoxIndex)
	{
		AABB2& textBox = textBoxes[textBoxIndex];
		Vec2 startPosition = textBox.m_mins;
		std::string const& textLine = textLines[textBoxIndex];
		for (int textIndex = 0; textIndex < (int)textLine.size(); ++textIndex)
		{
			int glyphIndex = static_cast<int>(textLine[textIndex]);
			float glyphAspect = GetGlyphAspect(glyphIndex);
			float glyphWidth = adjustedCellHeight * glyphAspect * cellAspectScale;
			float glyphHeight = adjustedCellHeight;

			AABB2 glyphCoords = m_fontGlyphsSpriteSheet.GetSpriteUVs(glyphIndex);
			AABB2 glyphAlignedBox(startPosition, startPosition + Vec2(glyphWidth, glyphHeight));
			AddVertsForText2D(vertexArray, startPosition, adjustedCellHeight, std::string(1, textLine[textIndex]), tint, cellAspectScale);

			startPosition.x += glyphWidth;
			glyphsDrawn += 1;

			// Check for maxglyphs
			if (glyphsDrawn >= maxGlyphsToDraw)
			{
				return;
			}
		}
	}
}

void BitmapFont::AddVertsForText3DAtOriginXForward(std::vector<Vertex_PCU>& verts, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, int maxGlyphsToDraw)
{
	UNUSED(maxGlyphsToDraw);

	std::vector<Vertex_PCU> textVerts2D;
	Vec2 textMinsAtOrigin = Vec2::ZERO;

	AddVertsForText2D(textVerts2D, textMinsAtOrigin, cellHeight, text, tint, cellAspect);
	AABB2 vertexBounds = GetVertexBounds(textVerts2D);
	Vec2 vertexBoundDims = vertexBounds.GetDimensions();

	float xOffSet = textMinsAtOrigin.x - vertexBoundDims.x * alignment.x;
	float yOffSet = textMinsAtOrigin.y - vertexBoundDims.y * alignment.y;

	AddVertsForText2D(verts, Vec2(xOffSet, yOffSet), cellHeight, text, tint, cellAspect);
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspectScale)
{
    float textWidth = 0.f;
    for (int textIndex = 0; textIndex < (int)text.size(); ++textIndex)
    {
		int glyphIndex = static_cast<int>(text[textIndex]);
        float glyphAspect = GetGlyphAspect(glyphIndex);
		textWidth += cellHeight * glyphAspect * cellAspectScale;
    }
    return textWidth;
}

float BitmapFont::GetGlyphAspect(int glyphUniCode) const
{
    AABB2 glyphUVCoords = m_fontGlyphsSpriteSheet.GetSpriteUVs(glyphUniCode);
    float glyphWidth = glyphUVCoords.GetDimensions().x;
    float glyphHeight = glyphUVCoords.GetDimensions().y;
    return glyphWidth / glyphHeight;
}
