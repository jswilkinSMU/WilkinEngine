#include "Engine/Math/RandomNumberGenerator.h"
#include <stdlib.h>

int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
    return rand() % maxNotInclusive;
}

int RandomNumberGenerator::RollRandomIntInRange(int minInclusive, int maxInclusive)
{
    int range = (maxInclusive - minInclusive) + 1;
    return minInclusive + RollRandomIntLessThan(range);
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
    float range = (maxInclusive - minInclusive);
    return minInclusive + RollRandomFloatZeroToOne() * range;
}

Vec3 RandomNumberGenerator::RollRandomUnitVector()
{
    // Needs to roll either Vec3::XAXE, Vec3::YAXE, or Vec3::ZAXE
    Vec3 unitVectors[3] = { Vec3::XAXE, Vec3::YAXE, Vec3::ZAXE };
    int  unitVectorIndex = RollRandomIntInRange(0, 2);
    return unitVectors[unitVectorIndex];
}

bool RandomNumberGenerator::RollRandomChance(float chanceToReturnTrue)
{
    if (chanceToReturnTrue <= 0.0f)
    {
        chanceToReturnTrue = 0.0f;
    }
    if (chanceToReturnTrue >= 1.0f)
    {
        chanceToReturnTrue = 1.0f;
    }

    float randomChance = RollRandomFloatZeroToOne();
    return randomChance <= chanceToReturnTrue;
}
