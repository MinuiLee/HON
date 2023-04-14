/******************************************************************************/
/*!
\file   MoveCharacter.cpp
\author Kim Hyungseob
\par    email: hn02415@gmail.com
\date   2019/06/12

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "MoveCharacter.h"

float BestFit::GetY(float t, float FinishTime)
{
	float InflectionTime = FinishTime / 5.f;

	// Construct matrix to save coefficients of three equation (start point, middle point and last point).
	glm::mat3x4 A;
	A[0] = glm::vec4(1.f, 1.f, 1.f, 1.f);
	A[1] = glm::vec4(0.f, InflectionTime, FinishTime, 2 * FinishTime - InflectionTime);
	A[2] = glm::vec4(0.f, InflectionTime*InflectionTime, FinishTime*FinishTime, (2 * FinishTime - InflectionTime) * (2 * FinishTime - InflectionTime));

	// Solving equation by multiplying inverse matrix.
	glm::mat4x3 AT = glm::transpose(A);
	glm::vec3 equation = glm::inverse(AT * A) * AT * glm::vec4(-250.f, -100.f, -10.f, -10.f);

	return glm::dot(equation, glm::vec3(1.f, t, t*t));
}

float BestFit::GetX(float y)
{
	// Construct matrix to save coefficients of three equation (start point, middle point and last point).
	glm::mat3x3 A;
	A[0] = glm::vec3(1.f, 1.f, 1.f);
	A[1] = glm::vec3(-250.f, -100.f, -10.f);
	A[2] = glm::vec3(A[1].x * A[1].x, A[1].y * A[1].y, A[1].z * A[1].z);

	// Solving equation by multiplying inverse matrix.
	glm::vec3 equation = glm::inverse(A) * glm::vec3(150.f, -400.f, -350.f);

	return glm::dot(equation, glm::vec3(1.f, y, y*y));
}

float BestFit::GetScale(float t, float StartTime, float FinishTime)
{
	float InflectionTime = FinishTime / 5.f;

	// Construct matrix to save coefficients of two equation (x-scale and y-scale).
	glm::mat2x3 A;
	A[0] = glm::vec3(1.f, 1.f, 1.f);
	A[1] = glm::vec3(StartTime, InflectionTime, FinishTime);

	// Solving equation by multiplying inverse matrix.
	glm::mat3x2 AT = glm::transpose(A);
	glm::vec2 equation = glm::inverse(AT * A) * AT * glm::vec3(1.f, 0.5f, 0.1f);

	return glm::dot(equation, glm::vec2(1.f, t));
}