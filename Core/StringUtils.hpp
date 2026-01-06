#pragma once
//-----------------------------------------------------------------------------------------------
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.h"
#include <string>
#include <vector>
// ----------------------------------------------------------------------------------------------
typedef std::vector< std::string >		Strings;
//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... );
const std::string Stringf( int maxLength, char const* format, ... );
// ----------------------------------------------------------------------------------------------
Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn);
Strings SplitStringOnWhiteSpace(std::string const& line);
// ----------------------------------------------------------------------------------------------
Vec2  CreateVec2FromStrings(std::string const& arg0, std::string const& arg1);
Vec3  CreateVec3FromStrings(std::string const& arg0, std::string const& arg1, std::string const& arg2);
int   CreateIntFromStrings(std::string const& arg);
float CreateFloatFromStrings(std::string const& arg);
// ----------------------------------------------------------------------------------------------


