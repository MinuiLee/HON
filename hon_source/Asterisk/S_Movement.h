#ifndef S_MOVEMENT_H
#define S_MOVEMENT_H
/******************************************************************************/
/*!
\headerfile   S_Movement.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for movement system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"

class C_State;
class C_RigidBody;
class MessageHandler;
class Map;
class C_Position;
class S_Movement : public S_Base
{
public:
    //Ctor
    S_Movement(SystemManager * sysMgr);

    //S_Base methods
    void Update(Window * window, float dt, bool editor_mode);

    //Observer method
    void Notify(Message & message);

private:
    void SetDirection(const EntityId & entity, const Direction & dir);
    bool IsDeadAnimationEnd(const EntityId & entity);
    void AirStompEnd(C_State * state, Window * window, MessageHandler * handler, 
        EntityId id, float dt);

};

#endif