#ifndef COLLISION_H
#define COLLISION_H
/******************************************************************************/
/*!
\headerfile   Collision.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyrigt
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Sprite.h"
using namespace glm;
struct Physics
{

	//only compares lines
	static bool LineCollision(vec2 p1, vec2 p2, vec2 p3, vec2 p4);
	static bool DoLinesCollide(Sprite obj1, Sprite obj2);

    //For extra credit
    static void GetInterval(std::vector<vec2> points, const vec2 & xAxis, float & min, float & max);
    static bool IntervalIntersect(std::vector<vec2> obj1_pnt, std::vector<vec2> obj2_pnt, vec2 & xAxis);
    static bool Collide(std::vector<vec2> obj1_pnt, std::vector<vec2> obj2_pnt);
    static bool PolygonCollisionDetection(Sprite obj1, Sprite obj2);

    //simple square collision detection
    static bool AreTheyColliding(Sprite obj1, Sprite obj2);
};

#endif