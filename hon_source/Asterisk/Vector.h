#ifndef VECTOR_H
#define VECTOR_H
/******************************************************************************/
/*!
\headerfile   Vector.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/10/21
\brief
		Header file for definition of vectors.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#define GLM_FORCE_CXX03
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>

using Vector2i = glm::ivec2;
using Vector2f = glm::vec2;
using Vector2u = glm::uvec2;

using Vector3i = glm::ivec3;
using Vector3f = glm::vec3;
using Vector3u = glm::uvec3;

using Vector4i = glm::ivec4;
using Vector4f = glm::vec4;
using Vector4u = glm::uvec4;

float Dot(const Vector3f & a, const Vector3f & b);
float Length(const Vector2f & vec);

#endif
