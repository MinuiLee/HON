#ifndef BASESTATE_H
#define BASESTATE_H
/******************************************************************************/
/*!
\headerfile   BaseState.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for base class for state classes.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "View.h"

class StateManager;
class BaseState
{
public:
    //Ctor & Dtor
    BaseState(StateManager * stateMgr)
	: m_stateMgr(stateMgr) {}
    virtual ~BaseState() {}

    //State Initializing - This is called when entering current state
    virtual void Create()  = 0;
    virtual void Destroy() = 0;

    //Game updating
    virtual void Update(float dt) = 0;
    virtual void Draw()				  = 0;

    View * GetView() { return &m_view; }

protected:
    StateManager * m_stateMgr;
    View m_view; // == camera
};

#endif
