/******************************************************************************/
/*!
\file   FloatRect.cpp
\author Primary: Minui Lee
		Secondary: Jaejun Jang
\par    email: alsdml0727@gmail.com
\date   2019/05/13

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "FloatRect.h"

bool FloatRect::Intersects(const Vector3f & point) const
{
    if (point.x > m_left && point.x < m_left + m_width &&
        point.y > m_top - m_height && point.y < m_top)
        return true;
    return false;
}

bool FloatRect::Intersects(const FloatRect & rect) const
{
    FloatRect MD = MinkowskiDifference(rect);
    if (MD.m_left <= 0 &&
        MD.m_left + MD.m_width >= 0 &&
        MD.m_top >= 0 &&
        MD.m_top - MD.m_height <= 0)
        return true;
    return false;
}

bool FloatRect::Intersects(const FloatRect & rect, FloatRect & intersect) const
{
    if (Intersects(rect))
    {
        intersect.m_left = m_left >= rect.m_left ? m_left : rect.m_left;
        intersect.m_top = m_top <= rect.m_top ? m_top : rect.m_top;

        float right = m_left + m_width <= rect.m_left + rect.m_width ? m_left + m_width : rect.m_left + rect.m_width;
        float bottom = m_top - m_height >= rect.m_top - rect.m_height ? m_top - m_height : rect.m_top - rect.m_height;

        intersect.m_width = abs(right - intersect.m_left);
        intersect.m_height = abs(intersect.m_top - bottom);
        return true;
    }

    return false;
}

float FloatRect::GetRayIntersectionFraction(const Vector3f & origin, 
    const Vector3f & relative_motion) const
{
    Vector3f end = origin + relative_motion;
    Vector3f bottom_left(m_left, m_top - m_height, 0);
    Vector3f top_left(m_left, m_top, 0);
    Vector3f top_right(m_left + m_width, m_top, 0);
    Vector3f bottom_right(m_left, m_top - m_height, 0);

    float t = GetRayIntersectionFractionOfFirstRay(origin, end, bottom_left, top_left);

    float x = GetRayIntersectionFractionOfFirstRay(origin, end, top_left, top_right);
    if (t < x) t = x;

    x = GetRayIntersectionFractionOfFirstRay(origin, end, top_right, bottom_right);
    if (t < x) t = x;

    x = GetRayIntersectionFractionOfFirstRay(origin, end, bottom_right, bottom_left);
    if (t < x) t = x;

    return t;
}

FloatRect FloatRect::MinkowskiDifference(const FloatRect & rhs) const
{
    return FloatRect(
        m_left - rhs.m_left - rhs.m_width, //A left - B right
        rhs.m_top - (m_top - m_height), //B top - A bottom
        m_width + rhs.m_width, //A width + B width 
        m_height + rhs.m_height); //A height + B height
}

float FloatRect::GetRayIntersectionFractionOfFirstRay(const Vector3f & originA, 
    const Vector3f & endA, const Vector3f & originB, const Vector3f & endB) const
{
    Vector3f r = endA - originA;
    Vector3f s = endB - originB;

    float numerator = Dot((originB - originA), r);
    float denominator = Dot(r, s);

    if (numerator == 0 && denominator == 0)
        return INFINITY;
    if (numerator == 0)
        return INFINITY;

    float u = numerator / denominator;
    float t = Dot((originB - originA), s) / denominator;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
        return t;

    return INFINITY;
}
