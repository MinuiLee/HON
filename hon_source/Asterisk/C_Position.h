#ifndef C_POSITION_H
#define C_POSITION_H
/******************************************************************************/
/*!
\headerfile   C_Position.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\breif
		Header file for Position component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"
#include "Vector.h"

class C_Position : public C_Base
{
public:
	// Ctor
	C_Position() : C_Base(Component::Position) {}

	// C_Base method
	void ReadIn(std::stringstream & sstream)
	{
		sstream >> m_position.x >> m_position.y >> m_position.z;
		m_depth = m_position.z;
	}

	// C_Position methods
	void SetPosition(const Vector3f & pos)
    { 
        m_old_position = m_position;
        m_position = pos; 
    }
	void SetPosition(float x, float y, float z)
	{
        m_old_position = m_position;
        m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	void SetInitialPosition(const Vector3f pos) { m_initPosition = pos; }
	void SetInitialPosition(float x, float y, float z) 
	{
		m_initPosition.x = x;
		m_initPosition.y = y;
		m_initPosition.z = z;
	}

	void MoveBy(const Vector3f & distance)
	{
        m_old_position = m_position;
		m_position.x += distance.x;
		m_position.y += distance.y;
		m_position.z += distance.z;
	}
	void MoveBy(float x = 0.f, float y = 0.f, float z = 0.f)
	{
        m_old_position = m_position;
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
	}

	const Vector3f & GetPosition() const { return m_position; }
	Vector3f & GetPosition() { return m_position; }
    const Vector3f & GetOldPosition() { return m_old_position; }
	const Vector3f GetInitialPosition() const { return m_initPosition; }

	float GetDepth() { return m_depth; }

private:
	Vector3f m_position;
    Vector3f m_old_position;
	Vector3f m_initPosition;
	float m_depth = 0;
};

#endif