#pragma once
#include "Engine/Math/IntVec2.h"
#include <string>
#include <vector>
// -----------------------------------------------------------------------------
struct Rgba8;
// -----------------------------------------------------------------------------
class Image
{
public:
	Image();
	~Image();
	Image(char const* imageFilePath);
	Image(IntVec2 size, Rgba8 color);

	std::string const& GetImageFilePath() const;
	IntVec2			   GetDimensions() const;
	const void*		   GetRawData() const;

	Rgba8			   GetTexelColor(int texelX, int texelY) const;
	Rgba8			   GetTexelColor(IntVec2 const& texelCoords) const;
	void			   SetTexelColor(int texelX, int texelY, Rgba8 const& newColor);
	void			   SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor);

private:
	std::string			 m_imageFilePath;
	IntVec2				 m_dimensions = IntVec2(0, 0);
	std::vector<Rgba8>   m_rgbaTexels;  
};
