/******************************************************************************/
/*!
\file   C_RigidBody.cpp
\author Jooho Jeong
\par    email: jooho556 \@ gmail.com
\date   2019/05/13

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Vector.h"

float Dot(const Vector3f & a, const Vector3f & b) { return a.x * b.x + a.y * b.y; }

float Length(const Vector2f & vec)
{ 
	return sqrt(vec.x * vec.x + vec.y * vec.y); 
}
