#ifndef S_STATE_H
#define S_STATE_H
/******************************************************************************/
/*!
\headerfile   S_State.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for state system class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"
#include "C_State.h"

class S_State : public S_Base
{
public:
    //Ctor
    S_State(SystemManager * sysMgr);

    //S_Base methods
    void Update(Window * /*window*/, float /*dt*/, bool /*editor_mode*/) {} // Do nothing

    //Observer method
    void Notify(Message & message);

private:
    void State_Changed_Message(const EntityId & entity, EntityState state);
};

#endif