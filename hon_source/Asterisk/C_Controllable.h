#ifndef C_CONTROLLABLE_H
#define C_CONTROLLABLE_H
/******************************************************************************/
/*!
\headerfile   C_Controllable.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for controllable component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"

class C_Controllable : public C_Base
{
public:
    C_Controllable() : C_Base(Component::Controllable) {}
    void ReadIn(std::stringstream & /*sstream*/) {}

    void SetPlayerCloseToDoor(int entity) { m_DoorCloseToPlayer = entity; }
    int IsPlayerCloseToDoor() { return m_DoorCloseToPlayer; }

private:
    int m_DoorCloseToPlayer = -1;
};

#endif