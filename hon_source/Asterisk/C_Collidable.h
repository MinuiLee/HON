#ifndef C_COLLIDABLE_H
#define C_COLLIDABLE_H
/******************************************************************************/
/*!
\headerfile   C_Collidable.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for collidable component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <map>
#include "C_Base.h"
#include "FloatRect.h"

struct Tile;
using CollidingTiles = std::map<std::pair<int, int>, Tile*>;

class C_Collidable : public C_Base
{
public:
    C_Collidable() : C_Base(Component::Collidable) {}

    void ReadIn(std::stringstream & sstream)
    {
	    sstream >> m_AABB.m_width >> m_AABB.m_height;
    }

    void SetPosition(const Vector3f & pos)
    {
        if (m_initial_displacement)
        {
            m_old_topleft.x = m_AABB.m_top;
            m_old_topleft.y = m_AABB.m_left;
            m_initial_displacement = false;
        }
        else
        {
            m_old_topleft.x = m_AABB.m_top;
            m_old_topleft.y = m_AABB.m_left;
        }
	    m_AABB.m_left = pos.x - m_AABB.m_width / 2;
	    m_AABB.m_top = pos.y + m_AABB.m_height / 2;
    }

    const FloatRect & GetAABB() const { return m_AABB; }
    FloatRect & GetAABB() { return m_AABB; }
	const Vector2f GetOldCenter()
	{ 
		return Vector2f(m_old_topleft.y + m_AABB.m_width / 2,
			m_old_topleft.x - m_AABB.m_height / 2);
	}

	const Vector2f GetCenter()
	{ 
		return Vector2f(m_AABB.m_left + m_AABB.m_width / 2, m_AABB.m_top - m_AABB.m_height / 2); 
	}

    void SetGhost(bool ghost) { m_isGhost = ghost; }
    bool IsGhost() const { return m_isGhost; }

    void SetResponsive(bool responsive) { m_responsive = responsive; }
    bool IsResponsive() { return m_responsive; }

    CollidingTiles & GetCollidingTiles() { return m_colliding_tiles; }

	bool GetIsCollidingWithTile() { return m_isCollideTile; }
	void SetIsCollidingWithTile(bool b) { m_isCollideTile = b; }

private:
    FloatRect m_AABB;
	Vector2f m_old_topleft;

    bool m_isGhost = false;
    bool m_responsive = false;
    CollidingTiles m_colliding_tiles; //Tile coordinates which collision is ignored
	bool m_isCollideTile = false; //if the entity is colliding with any tile or not
    bool m_initial_displacement = true;
};

#endif