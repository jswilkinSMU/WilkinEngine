#include "Engine/Core/Rgba8.h"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.h"
// -----------------------------------------------------------------------------
/* White, Grays and Black */
const Rgba8 Rgba8::WHITE(Rgba8(255, 255, 255, 255));
const Rgba8 Rgba8::SNOW(Rgba8(255, 250, 250, 255));
const Rgba8 Rgba8::HONEYDEW(Rgba8(240, 255, 240, 255));
const Rgba8 Rgba8::MINTCREAM(Rgba8(245, 255, 250, 255));
const Rgba8 Rgba8::AZURE(Rgba8(240, 255, 255, 255));
const Rgba8 Rgba8::ALICEBLUE(Rgba8(240, 248, 255, 255));
const Rgba8 Rgba8::GHOSTWHITE(Rgba8(248, 248, 255, 255));
const Rgba8 Rgba8::WHITESMOKE(Rgba8(245, 245, 245, 255));
const Rgba8 Rgba8::SEASHELL(Rgba8(255, 245, 238, 255));
const Rgba8 Rgba8::BEIGE(Rgba8(245, 245, 220, 255));
const Rgba8 Rgba8::OLDLACE(Rgba8(253, 245, 230, 255));
const Rgba8 Rgba8::FlORALWHITE(Rgba8(255, 250, 240, 255));
const Rgba8 Rgba8::IVORY(Rgba8(255, 255, 240, 255));
const Rgba8 Rgba8::ANTIQUEWHITE(Rgba8(250, 235, 215, 255));
const Rgba8 Rgba8::LINEN(Rgba8(250, 240, 230, 255));
const Rgba8 Rgba8::LAVENDERBLUSH(Rgba8(255, 240, 245, 255));
const Rgba8 Rgba8::MISTYROSE(Rgba8(255, 228, 225, 255));
const Rgba8 Rgba8::GAINSBORO(Rgba8(220, 220, 220, 255));
const Rgba8 Rgba8::LIGHTGRAY(Rgba8(211, 211, 211, 255));
const Rgba8 Rgba8::SILVER(Rgba8(192, 192, 192, 255));
const Rgba8 Rgba8::GRAY(Rgba8(130, 130, 130, 255));
const Rgba8 Rgba8::DARKGRAY(Rgba8(169, 169, 169, 255));
const Rgba8 Rgba8::DIMGRAY(Rgba8(105, 105, 105, 255));
const Rgba8 Rgba8::LIGHTSLATEGRAY(Rgba8(119, 136, 153, 255));
const Rgba8 Rgba8::SLATEGRAY(Rgba8(112, 128, 144, 255));
const Rgba8 Rgba8::DARKSLATEGRAY(Rgba8(47, 79, 79, 255));
const Rgba8 Rgba8::BLACK(Rgba8(0, 0, 0, 255));
// -----------------------------------------------------------------------------
/* Greens */
const Rgba8 Rgba8::GREEN(Rgba8(0, 255, 0, 255));
const Rgba8 Rgba8::LIMEGREEN(Rgba8(50, 205, 50, 255));
const Rgba8 Rgba8::SEAWEED(Rgba8(20, 60, 20, 255));
// -----------------------------------------------------------------------------
/* Blues */
const Rgba8 Rgba8::BLUE(Rgba8(0, 0, 255, 255));
const Rgba8 Rgba8::LIGHTBLUE(Rgba8(173, 216, 230, 255));
const Rgba8 Rgba8::SAPPHIRE(Rgba8(50, 80, 150, 255));
// -----------------------------------------------------------------------------
/* Cyans */
const Rgba8 Rgba8::CYAN(Rgba8(0, 255, 255, 255));
// -----------------------------------------------------------------------------
/* Oranges */
const Rgba8 Rgba8::ORANGERED(Rgba8(255, 69, 0, 255));
const Rgba8 Rgba8::TOMATO(Rgba8(255, 99, 71, 255));
const Rgba8 Rgba8::CORAL(Rgba8(255, 127, 80, 255));
const Rgba8 Rgba8::DARKORANGE(Rgba8(255, 140, 0, 255));
const Rgba8 Rgba8::ORANGE(Rgba8(255, 165, 0, 255));
const Rgba8 Rgba8::GOLD(Rgba8(255, 215, 0, 255));
// -----------------------------------------------------------------------------
/* Reds */
const Rgba8 Rgba8::SALMON(Rgba8(250, 128, 114, 255));
const Rgba8 Rgba8::CRIMSON(Rgba8(220, 20, 60, 255));
const Rgba8 Rgba8::FIREBRICK(Rgba8(178, 34, 34, 255));
const Rgba8 Rgba8::DARKRED(Rgba8(139, 0, 0, 255));
const Rgba8 Rgba8::RED(Rgba8(255, 0, 0, 255));
// -----------------------------------------------------------------------------
/* Yellows */
const Rgba8 Rgba8::YELLOW(Rgba8(255, 255, 0, 255));
const Rgba8 Rgba8::LIGHTYELLOW(Rgba8(255, 255, 224, 255));
const Rgba8 Rgba8::LEMONCHIFFON(Rgba8(255, 250, 205, 255));
const Rgba8 Rgba8::PEACHPUFF(Rgba8(255, 218, 185, 255));
const Rgba8 Rgba8::PALEGOLDENROD(Rgba8(238, 232, 170, 255));
const Rgba8 Rgba8::KHAKI(Rgba8(240, 230, 140, 255));
const Rgba8 Rgba8::DARKKHAKI(Rgba8(189, 183, 107, 255));
// -----------------------------------------------------------------------------
/* Purples */
const Rgba8 Rgba8::MAGENTA(Rgba8(255, 0, 255, 255));
const Rgba8 Rgba8::PURPLE(Rgba8(128, 0, 128, 255));
// -----------------------------------------------------------------------------
/* Pinks */
const Rgba8 Rgba8::PINK(Rgba8(255, 192, 203, 255));
// -----------------------------------------------------------------------------
/* Browns */
const Rgba8 Rgba8::BROWN(Rgba8(165, 42, 42, 255));
const Rgba8 Rgba8::SKIN(Rgba8(210, 180, 140, 255));
const Rgba8 Rgba8::DARKBROWN(Rgba8(60, 40, 20));
// -----------------------------------------------------------------------------
Rgba8::Rgba8()
	: r(255), g(255), b(255), a(255)
{
}

Rgba8::Rgba8(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	: r(red), g(green), b(blue), a(alpha)
{
}

void Rgba8::SetFromText(char const* text)
{
	Strings commaSplit = SplitStringOnDelimiter(text, ',');

	r = static_cast<unsigned char>(GetClamped(atoi(commaSplit[0].c_str()), 0, 255));
	g = static_cast<unsigned char>(GetClamped(atoi(commaSplit[1].c_str()), 0, 255));
	b = static_cast<unsigned char>(GetClamped(atoi(commaSplit[2].c_str()), 0, 255));

	if (commaSplit.size() == 4)
	{
		a = static_cast<unsigned char>(GetClamped(atoi(commaSplit[3].c_str()), 0, 255));
	}
	else
	{
		a = 255;
	}
}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	if (colorAsFloats == nullptr)
	{
		return;
	}
	colorAsFloats[0] = r / 255.f;
	colorAsFloats[1] = g / 255.f;
	colorAsFloats[2] = b / 255.f;
	colorAsFloats[3] = a / 255.f;
}

Vec4 Rgba8::GetAsVec4() const
{
	float deNormByte = 1.f / 255.f;

	float red	= static_cast<float>(r) * deNormByte;
	float green = static_cast<float>(g) * deNormByte;
	float blue  = static_cast<float>(b) * deNormByte;
	float alpha = static_cast<float>(a) * deNormByte;

	return Vec4(red, green, blue, alpha);
}

Rgba8 Rgba8::Rgba8Interpolate(Rgba8 start, Rgba8 end, float fractionOfEnd)
{
	float red = Interpolate(NormalizeByte(start.r), NormalizeByte(end.r), fractionOfEnd);
	float green = Interpolate(NormalizeByte(start.g), NormalizeByte(end.g), fractionOfEnd);
	float blue = Interpolate(NormalizeByte(start.b), NormalizeByte(end.b), fractionOfEnd);
	float alpha = Interpolate(NormalizeByte(start.a), NormalizeByte(end.a), fractionOfEnd);
	return Rgba8(DenormalizeByte(red), DenormalizeByte(green), DenormalizeByte(blue), DenormalizeByte(alpha));
}

