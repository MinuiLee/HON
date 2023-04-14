#ifndef VIEW_H
#define VIEW_H
/******************************************************************************/
/*!
\headerfile   View.h
\author Primary : Jooho Jung
		Second  : Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/10/28

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Vector.h"

class View
{
public:
    View() : m_center(Vector3f(0.f, 0.f, 0.f)), m_size(Vector2f(1280, 800)) {}

    void SetPosition(float x, float y, float z) { m_center = Vector3f(x, y, z); }
    const Vector3f & GetPosition() const { return m_center; }
    void SetSize(float x, float y)
    {
		m_size.x = x;
		m_size.y = y;
    }
    const Vector2f & GetSize() const { return m_size; }
    void SetCenter(float x, float y)
    {
		m_center.x = x;
		m_center.y = y;
    }
    void SetCenter(const Vector2f & center)
    {
		m_center.x = center.x;
		m_center.y = center.y;
    }
	void SetLeft(float left)
    {
		m_center.x = left + m_size.x / 2;
    }
    const Vector3f & GetCenter() const { return m_center; }
    const glm::vec3 & GetZoom() { return m_zoom; }
    void SetZoom(float x, float y, float z = 1.0f)
    {
	    m_zoom.x = x;
	    m_zoom.y = y;
	    m_zoom.z = z;
    }

	void SetDelta(glm::vec2& input)
    {
		m_delta_view = input;
    }
	glm::vec2 const& GetDelta() { return m_delta_view; }

private:
    Vector3f m_center; ///< Center of the view, in scene coordinates
    Vector2f m_size;   ///< Size of the view, in scene coordinates
    float m_rotation;  ///< Angle of rotation of the view rectangle, in degrees
    glm::vec3 m_zoom = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 m_delta_view;
};

#endif