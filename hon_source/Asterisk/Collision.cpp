/******************************************************************************/
/*!
\file   Collision.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <iostream>

#include "Collision.h"

bool Physics::LineCollision(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
	float denominator = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);
	if (denominator == 0) return false;

	float t_numerator = (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
	float s_numerator = (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);
	if (t_numerator == 0.f && t_numerator == 0.f) return false;

	float t = t_numerator / denominator;
	float s = s_numerator / denominator;

	return !(t < 0.0f || t > 1.0f || s < 0.0f || s > 1.0f);
}

bool Physics::DoLinesCollide(Sprite obj1, Sprite obj2)
{
	int ob2PointCount = obj2.GetMesh()->Get_NumOf_PolyVertices();
	int ob1PointCount = obj1.GetMesh()->Get_NumOf_PolyVertices();

	for (int i = 0; i < ob2PointCount; i++)
	{
		vec2 pos1 = obj2.GetTransform().GetModel() * obj1.GetMesh()->GetPolyPoint(i);

		int index = i + 1;
		if (index == ob2PointCount)
			index = 0;

		vec2 pos2 = obj2.GetTransform().GetModel() * obj1.GetMesh()->GetPolyPoint(index);

		for (int j = 0; j < ob1PointCount; j++)
		{
			vec2 pos3 = obj1.GetTransform().GetModel() * obj1.GetMesh()->GetPolyPoint(i);

			int index2 = j + 1;
			if (index2 == ob1PointCount)
				index2 = 0;

			vec2 pos4 = obj1.GetTransform().GetModel() * obj1.GetMesh()->GetPolyPoint(index2);

			if (LineCollision(pos1, pos2, pos3, pos4))
				return true;
		}
	}
	return false;
}

//Polygon collision
void Physics::GetInterval(std::vector<vec2> points, const vec2 & xAxis, float & min, float & max)
{
	min = max = dot(points[0], xAxis);

	for (unsigned i = 1; i < points.size(); i++)
	{
		float d = dot(points[i], xAxis);
		if (d < min)
			min = d;
		else if (d > max)
			max = d;
	}
}

bool Physics::IntervalIntersect(std::vector<vec2> obj1_pnt, std::vector<vec2> obj2_pnt, vec2 & xAxis)
{
	float min1, max1;
	float min2, max2;

	GetInterval(obj1_pnt, xAxis, min1, max1);
	GetInterval(obj2_pnt, xAxis, min2, max2);

	float d1 = min1 - max1;
	float d2 = min2 - max1;

	if (d1 > 0.0f || d2 > 0.0f)
		return false;
	else
		return true;
}


bool Physics::Collide(std::vector<vec2> obj1_pnt, std::vector<vec2> obj2_pnt)
{
	vec2 xAxis[64];
	int iNumAxes = 0;

	int obj1_size = (int)obj1_pnt.size();
	for (int j = obj1_size - 1, i = 0; i < obj1_size; j = i, i++)
	{
		vec2 obj_vector = obj1_pnt[i] - obj1_pnt[j];
		xAxis[iNumAxes] = vec2(-obj_vector.y, obj_vector.x);

		if (!IntervalIntersect(obj1_pnt, obj2_pnt, xAxis[iNumAxes]))
			return false;

		iNumAxes++;
	}

	int obj2_size = (int)obj2_pnt.size();
	for (int j = obj2_size - 1, i = 0; i < obj2_size; j = i, i++)
	{
		vec2 obj_vector = obj2_pnt[i] - obj2_pnt[j];
		xAxis[iNumAxes] = vec2(-obj_vector.y, obj_vector.x);

		if (!IntervalIntersect(obj1_pnt, obj2_pnt, xAxis[iNumAxes]))
			return false;

		iNumAxes++;
	}
	return true;
}

// vertex num == 4 -> rhombus
bool Physics::PolygonCollisionDetection(Sprite obj1, Sprite obj2)
{
	std::vector<vec2> obj1_pos;
	std::vector<vec2> obj2_pos;

	int ob2PointCount = obj2.GetMesh()->Get_NumOf_PolyVertices();
	int ob1PointCount = obj1.GetMesh()->Get_NumOf_PolyVertices();

	for (int i = 0; i < ob1PointCount; i++)
	{
		vec2 temp = obj1.GetTransform().GetModel() * obj1.GetMesh()->GetPolyPoint(i);
		obj1_pos.push_back(temp);
	}

	for (int i = 0; i < ob2PointCount; i++)
	{
		vec2 temp = obj2.GetTransform().GetModel() * obj2.GetMesh()->GetPolyPoint(i);

		obj2_pos.push_back(temp);
	}

	return Collide(obj1_pos, obj2_pos);
}

bool Physics::AreTheyColliding(Sprite obj1, Sprite obj2)
{
	std::vector<vec3> A, B;

	for (int i = 0; i < 4; i++)
	{
		A.push_back(obj1.GetTransform().GetModel() * obj1.GetMesh()->GetPoint(i));
		B.push_back(obj2.GetTransform().GetModel() * obj2.GetMesh()->GetPoint(i));
	}
	return (A[2].x >= B[3].x && A[3].x <= B[2].x && A[0].y <= B[3].y && A[3].y >= B[0].y);
}
