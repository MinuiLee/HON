#ifndef MATHEMATICS_H
#define MATHEMATICS_H
/*!*******************************************************************
\headerfile Mathematics.h <>
\author     Kim HyungSeob
\par        email: hn02415 \@ gmail.com
\date		2019/01/05
\brief
			Header file for arithmetic helper functions.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Vector.h"

namespace Mathematics
{
    #define PI 3.14159265f

    int GetRandomInt(int min, int max);
    float GetAngleBetween(Vector2f origin, Vector2f target);
	double Clamp(double min, double max, double value);
}

#endif //MATHEMATICS_H