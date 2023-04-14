/*!*******************************************************************
\file         C_GameObject.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/29

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "C_GameObject.h"

#include "GameObject\Door.h"
#include "GameObject\PlateTower.h"
#include "GameObject\FragileWall.h"

void C_GameObject::ReadIn(std::stringstream & sstream)
{
    float type;
    sstream >> type;

    switch (static_cast<int>(type))
    {
    case 0:
        m_type = ObjectType::Door;
		m_object = std::make_any<Door>();
        break;

    case 1:
        m_type = ObjectType::Box;
        break;

    case 2:
        m_type = ObjectType::MovingTile;
        break;

    case 3:
        m_type = ObjectType::PlateTower;
        m_object = std::make_any<PlateTower>();
        break;

	case 4:
		m_type = ObjectType::FragileWall;
		m_object = std::make_any<FragileWall>();
		break;

    default:
        m_type = ObjectType::Last;
    }
}