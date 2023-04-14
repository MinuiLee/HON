#ifndef STATE_H
#define STATE_H
/*!*******************************************************************
\headerfile   State.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/02
\brief
	          Header file for base enemy AI state class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "..\Observer.h"

using EntityId = unsigned int;
class SystemManager;

/*!*******************************************************************
\class State
\brief
       Abstract base class to define an interface for a state.
********************************************************************/
class State
{
public:
    /*!*******************************************************************
    \brief Virtual destructor.
    ********************************************************************/
    virtual ~State() {}

    /*!*******************************************************************
    \brief
	       This will execute when the state is entered.

    \param systemMgr
	       Pointer to system manager to use engine interface.

    \param entity
           Id of entity in this state.
    ********************************************************************/
    virtual void Enter(SystemManager*, EntityId) {}

    /*!*******************************************************************
    \brief
	       This is the states normal update function.
           It is the only method that must be implemented.

    \param systemMgr
	       Pointer to system manager to use engine interface.

    \param entity
	       Id of entity in this state.

    \param dt
           Elapsed time for working base on the time.
    ********************************************************************/
	virtual void Update(SystemManager*, EntityId, float) {}

    /*!*******************************************************************
    \brief
           This will execute when the state is exited.

    \param systemMgr
	       Pointer to system manager to use engine interface.

    \param entity
	       Id of entity in this state.
    ********************************************************************/
    virtual void Exit(SystemManager*, EntityId) {}

    /*!*******************************************************************
    \brief
	       This executes if the agent receives a message.

    \param systemMgr
	       Pointer to system manager to use engine interface.

    \param msg
           Message to handle.

    \return bool
            If message processed properly, return true. If not, false.
    ********************************************************************/
    virtual bool HandleMessage(SystemManager*, const Message&) { return false; }
};

#endif