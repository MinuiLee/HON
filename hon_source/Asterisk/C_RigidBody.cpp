/******************************************************************************/
/*!
\file   C_RigidBody.cpp
\author Jooho Jeong
\par    email: jooho556 \@ gmail.com
\date   2019/01/15

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_RigidBody.h"

void C_RigidBody::ReadIn(std::stringstream & sstream)
{
	sstream >> m_force >> m_maxvelocity.x >> m_maxvelocity.y >> m_jump_speed;
}

void C_RigidBody::AddVelocity(const vec3 vel)
{
    if (m_velocity.x > m_maxvelocity.x && vel.x <= 0)
        m_velocity.x += vel.x;
    else if (m_velocity.x < -m_maxvelocity.x && vel.x >= 0)
        m_velocity.x += vel.x;
    else
    {
        m_velocity.x += vel.x;
        if (m_velocity.x > m_maxvelocity.x)
            m_velocity.x = m_maxvelocity.x;
        else if (m_velocity.x < -m_maxvelocity.x)
            m_velocity.x = -m_maxvelocity.x;
    }

    if (m_velocity.y > m_maxvelocity.y && vel.y <= 0)
        m_velocity.y += vel.y;
    else if (m_velocity.y < -m_maxvelocity.y && vel.y >= 0)
        m_velocity.y += vel.y;
    else
    {
        m_velocity.y += vel.y;
        if (m_velocity.y > m_maxvelocity.y)
            m_velocity.y = m_maxvelocity.y;
        else if (m_velocity.y < -m_maxvelocity.y)
            m_velocity.y = -m_maxvelocity.y;
    }
}

void C_RigidBody::SetVelocity(const vec3 vel)
{
    m_velocity = vel;
}

void C_RigidBody::ApplyFriction(float x, float y)
{
    if (m_velocity.x != 0 && m_acceleration.x == 0)
    {
        if (abs(m_velocity.x) - abs(x) < 0)
            m_velocity.x = 0;
        else
        {
            if (m_velocity.x < 0)
                m_velocity.x += x;
            else
                m_velocity.x -= x;
        }
    }

    if (m_velocity.y != 0)
    {
        if (abs(m_velocity.y) - abs(y) < 0)
            m_velocity.y = 0;
        else
        {
            if (m_velocity.y < 0)
                m_velocity.y += y;
            else
                m_velocity.y -= y;
        }
    }
}
