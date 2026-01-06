#define STB_IMAGE_IMPLEMENTATION // Exactly one .CPP (this Image.cpp) should #define this before #including stb_image.h
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/Image.hpp"
#include "Engine/Core/Rgba8.h"
#include "Engine/Core/EngineCommon.h"

Image::Image()
{
}

Image::~Image()
{
}

Image::Image(char const* imageFilePath)
	:m_imageFilePath(imageFilePath)
{
	int texelSizeX = 0;
	int texelSizeY = 0;
	int colorComponents = 0;

	// Load the image and check if the image was loaded
	stbi_set_flip_vertically_on_load(1);
	unsigned char* imageData = stbi_load(imageFilePath, &texelSizeX, &texelSizeY, &colorComponents, STBI_rgb_alpha);
	GUARANTEE_OR_DIE(imageData, Stringf("Failed to load image \"%s\"", imageFilePath));

	// Set dimensions and resize vector
	m_dimensions = IntVec2(texelSizeX, texelSizeY);
	m_rgbaTexels.resize(texelSizeX * texelSizeY);

	for (int y = 0; y < texelSizeY; ++y)
	{
		for (int x = 0; x < texelSizeX; ++x)
		{
			int texelIndex = (y * texelSizeX + x) * 4;
			unsigned char r = imageData[texelIndex];
			unsigned char g = imageData[texelIndex + 1];
			unsigned char b = imageData[texelIndex + 2];
			unsigned char a = imageData[texelIndex + 3];
			m_rgbaTexels[y * texelSizeX + x] = Rgba8(r, g, b, a);
		}
	}
	stbi_image_free(imageData);
}

Image::Image(IntVec2 size, Rgba8 color)
	:m_dimensions(size)
{
	int numPixels = size.x * size.y;
	m_rgbaTexels.resize(numPixels, color);
}

std::string const& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
	return m_dimensions;
}

const void* Image::GetRawData() const
{
	return m_rgbaTexels.data();
}

Rgba8 Image::GetTexelColor(int texelX, int texelY) const
{
	int imageIndex = (texelY * m_dimensions.x) + texelX;
	return m_rgbaTexels[imageIndex];
}

Rgba8 Image::GetTexelColor(IntVec2 const& texelCoords) const
{
	return GetTexelColor(texelCoords.x, texelCoords.y);
}

void Image::SetTexelColor(int texelX, int texelY, Rgba8 const& newColor)
{
	int imageIndex = (texelY * m_dimensions.x) + texelX;
	m_rgbaTexels[imageIndex] = newColor;
}

void Image::SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor)
{
	SetTexelColor(texelCoords.x, texelCoords.y, newColor);
}






