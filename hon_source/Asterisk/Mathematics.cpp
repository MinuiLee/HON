/******************************************************************************/
/*!
\file   Mathematics.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/01/05

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <random>
#include <cmath>

#include "Mathematics.h"

namespace Mathematics
{
    int GetRandomInt(int min, int max)
    {
        std::random_device rnd;
        std::mt19937 rng(rnd());

        std::uniform_int_distribution<int> range(min, max);

        return range(rnd);
    }

    float GetAngleBetween(Vector2f origin, Vector2f target)
    {
        auto direction = target - origin;

        if (!direction.x)
        {
            if (direction.y > 0.f) return 90.f;
            if (direction.y < 0.f) return 270.f;
            return 0.f;
        }

        return atan(direction.y / direction.x) * 180.f / PI;
    }

	double Clamp(double min, double max, double value)
	{
		if (value > max)
			return max;
		if (value < min)
			return min;
		return value;
	}

} // namespace Mathematics