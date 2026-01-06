#include "Engine/Core/StringUtils.hpp"
#include <stdarg.h>
#include <sstream>


//-----------------------------------------------------------------------------------------------
constexpr int STRINGF_STACK_LOCAL_TEMP_LENGTH = 2048;


//-----------------------------------------------------------------------------------------------
const std::string Stringf( char const* format, ... )
{
	char textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, STRINGF_STACK_LOCAL_TEMP_LENGTH, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ STRINGF_STACK_LOCAL_TEMP_LENGTH - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	return std::string( textLiteral );
}


//-----------------------------------------------------------------------------------------------
const std::string Stringf( int maxLength, char const* format, ... )
{
	char textLiteralSmall[ STRINGF_STACK_LOCAL_TEMP_LENGTH ];
	char* textLiteral = textLiteralSmall;
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		textLiteral = new char[ maxLength ];

	va_list variableArgumentList;
	va_start( variableArgumentList, format );
	vsnprintf_s( textLiteral, maxLength, _TRUNCATE, format, variableArgumentList );	
	va_end( variableArgumentList );
	textLiteral[ maxLength - 1 ] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

	std::string returnValue( textLiteral );
	if( maxLength > STRINGF_STACK_LOCAL_TEMP_LENGTH )
		delete[] textLiteral;

	return returnValue;
}

Strings SplitStringOnDelimiter(std::string const& originalString, char delimiterToSplitOn)
{
	Strings splitString;
	std::string fullString;
	for (size_t stringIndex = 0; stringIndex < originalString.size(); ++stringIndex)
	{
		if (originalString[stringIndex] == delimiterToSplitOn)
		{
			splitString.push_back(fullString);
			fullString.clear();
		}
		else
		{
			fullString.push_back(originalString[stringIndex]);
		}
	}
	splitString.push_back(fullString);
	return splitString;
}

Strings SplitStringOnWhiteSpace(std::string const& line)
{
	std::istringstream lineRead(line);
	Strings result;
	std::string args;

	while (lineRead >> args)
	{
		result.push_back(args);
	}
	return result;
}

Vec2 CreateVec2FromStrings(std::string const& arg0, std::string const& arg1)
{
	float x = std::stof(arg0);
	float y = std::stof(arg1);
	return Vec2(x, y);
}

Vec3 CreateVec3FromStrings(std::string const& arg0, std::string const& arg1, std::string const& arg2)
{
	float x = std::stof(arg0);
	float y = std::stof(arg1);
	float z = std::stof(arg2);
	return Vec3(x, y, z);
}

int CreateIntFromStrings(std::string const& arg)
{
	int stringToInt = std::stoi(arg);
	return stringToInt;
}

float CreateFloatFromStrings(std::string const& arg)
{
	float stringToFloat = std::stof(arg);
	return stringToFloat;
}





