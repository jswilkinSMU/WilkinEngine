#pragma once
#include "Engine/Math/Vec4.hpp"
// -----------------------------------------------------------------------------
struct Rgba8
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	//default constructor
	Rgba8();

	//explicit constructor
	explicit Rgba8(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
// -----------------------------------------------------------------------------
	void  SetFromText(char const* text);
	void  GetAsFloats(float* colorAsFloats) const;
	Vec4  GetAsVec4() const;
	static Rgba8 Rgba8Interpolate(Rgba8 start, Rgba8 end, float fractionOfEnd);
// -----------------------------------------------------------------------------
	static const Rgba8 WHITE;
	static const Rgba8 SNOW;
	static const Rgba8 HONEYDEW;
	static const Rgba8 MINTCREAM;
	static const Rgba8 AZURE;
	static const Rgba8 ALICEBLUE;
	static const Rgba8 GHOSTWHITE;
	static const Rgba8 WHITESMOKE;
	static const Rgba8 SEASHELL;
	static const Rgba8 BEIGE;
	static const Rgba8 OLDLACE;
	static const Rgba8 FlORALWHITE;
	static const Rgba8 IVORY;
	static const Rgba8 ANTIQUEWHITE;
	static const Rgba8 LINEN;
	static const Rgba8 MISTYROSE;
	static const Rgba8 GAINSBORO;
	static const Rgba8 LAVENDERBLUSH;
	static const Rgba8 LIGHTGRAY;
	static const Rgba8 SILVER;
	static const Rgba8 GRAY;
	static const Rgba8 DARKGRAY;
	static const Rgba8 DIMGRAY;
	static const Rgba8 LIGHTSLATEGRAY;
	static const Rgba8 SLATEGRAY;
	static const Rgba8 DARKSLATEGRAY;
	static const Rgba8 BLACK;
	static const Rgba8 SEAWEED;
	static const Rgba8 SAPPHIRE;
	static const Rgba8 BLUE;
	static const Rgba8 CYAN;
	static const Rgba8 ORANGERED;
	static const Rgba8 TOMATO;
	static const Rgba8 CORAL;
	static const Rgba8 DARKORANGE;
	static const Rgba8 ORANGE;
	static const Rgba8 GOLD;
	static const Rgba8 SALMON;
	static const Rgba8 CRIMSON;
	static const Rgba8 FIREBRICK;
	static const Rgba8 DARKRED;
	static const Rgba8 RED;
	static const Rgba8 YELLOW;
	static const Rgba8 LIGHTYELLOW;
	static const Rgba8 LEMONCHIFFON;
	static const Rgba8 PEACHPUFF;
	static const Rgba8 PALEGOLDENROD;
	static const Rgba8 KHAKI;
	static const Rgba8 DARKKHAKI;
	static const Rgba8 LIMEGREEN;
	static const Rgba8 GREEN;
	static const Rgba8 LIGHTBLUE;
	static const Rgba8 PINK;
	static const Rgba8 BROWN;
	static const Rgba8 SKIN;
	static const Rgba8 DARKBROWN;
	static const Rgba8 PURPLE;
	static const Rgba8 MAGENTA;
// -----------------------------------------------------------------------------
};