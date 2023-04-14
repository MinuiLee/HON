#ifndef RECT_H
#define RECT_H
/******************************************************************************/
/*!
\headerfile   FloatRect.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/10/21

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>

#include "Vector.h"

struct FloatRect
{
    FloatRect() : m_left(0), m_top(0), m_width(0), m_height(0) {}
    FloatRect(float left, float top, float width, float height)
	: m_left(left), m_top(top), m_width(width), m_height(height)
    {
    }

    bool Intersects(const Vector3f & point) const;
    bool Intersects(const FloatRect & rect) const;
    bool Intersects(const FloatRect & rect, FloatRect & intersect) const;

    float GetRayIntersectionFraction(const Vector3f & origin, const Vector3f & relative_motion) const;
    FloatRect MinkowskiDifference(const FloatRect & rhs) const;
	Vector2f GetTopLeft() const { return Vector2f(m_top, m_left); }

    float m_left;
    float m_top;
    float m_width;
    float m_height;

private:
    float GetRayIntersectionFractionOfFirstRay(const Vector3f & originA, const Vector3f & endA,
        const Vector3f & originB, const Vector3f & endB) const;
};

#endif