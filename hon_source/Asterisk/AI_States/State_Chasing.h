#ifndef STATE_CHASING_H
#define STATE_CHASING_H
/*!*******************************************************************
\headerfile   State_Chasing.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/24
\brief
	          Header file for Chasing enemy AI state class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class State_Chasing
\brief
       Define state where enemy is chasing player.
********************************************************************/
class State_Chasing : public State
{
private: // Constructors and assginment operator should be private
    /*!*******************************************************************
    \brief Priavte default constructor.
    ********************************************************************/
    State_Chasing() = default;
    /*!*******************************************************************
    \brief
	    Private copy constructor.

    \param
	    Reference of this class.
    ********************************************************************/
    State_Chasing(const State_Chasing&);
    /*!*******************************************************************
    \brief
	    Private assignment operator.

    \param
	    Reference of this class.
    ********************************************************************/
    State_Chasing& operator=(const State_Chasing&);

public:
    /*!*******************************************************************
    \brief
	    Singleton pattern method.

    \return State_Chasing*
	    Returns pointer to instance of State_Chasing.
    ********************************************************************/
    static State_Chasing* GetInstance();

    /*!*******************************************************************
    \brief
	   Initialize condition and timer.

    \param systemMgr
	   Pointer to system manager to use engine interface.

    \param entity
	   Id of entity in this state.
    ********************************************************************/
    void Enter(SystemManager* systemMgr, EntityId entity) override;

    /*!*******************************************************************
    \brief
	   Make enemy move to position of player.

    \param systemMgr
	   Pointer to system manager to use engine interface.

    \param entity
	   Id of entity in this state.

    \param dt
	   Elapsed time for working base on the time.
    ********************************************************************/
    void Update(SystemManager* systemMgr, EntityId entity, float) override;
};

#endif //STATE_CHASING_H