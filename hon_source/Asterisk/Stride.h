#ifndef STRIDE_H
#define STRIDE_H
/******************************************************************************/
/*!
\headerfile   Stride.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for stride and position class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Color.h"

struct Position
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Stride
{
	Stride(float _x, float _y, float _z)
	{
		position.x = _x;
		position.y = _y;
		position.z = _z;
	}

	Position position;
	float textureCoordinates[2];
};

#endif