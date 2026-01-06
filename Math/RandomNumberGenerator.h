#pragma once
#include "Engine/Math/Vec3.h"

class RandomNumberGenerator
{
public:
	int RollRandomIntLessThan(int maxNotInclusive);
	int RollRandomIntInRange(int minInclusive, int maxInclusive);
	
	float RollRandomFloatZeroToOne();
	float RollRandomFloatInRange(float minInclusive, float maxInclusive);

	Vec3 RollRandomUnitVector();
	bool RollRandomChance(float chanceToReturnTrue);

private:
	/*unsigned int m_seed = 0;*/
};
