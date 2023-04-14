#ifndef STATE_WANDERING_H
#define STATE_WANDERING_H
/*!*******************************************************************
\headerfile   State_Wandering.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/19
\brief
              Header file for Wandering enemy AI state class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class State_Wandering
\brief
       Define state when enemy is wandering.
********************************************************************/
class State_Wandering : public State
{
private: // Constructors and assginment operator should be private
    /*!*******************************************************************
    \brief Priavte default constructor.
    ********************************************************************/
    State_Wandering() = default;
    /*!*******************************************************************
    \brief
	   Private copy constructor.

    \param 
	   Reference of this class.
    ********************************************************************/
    State_Wandering(const State_Wandering&);
    /*!*******************************************************************
    \brief
	   Private assignment operator.

    \param
	   Reference of this class.
    ********************************************************************/
    State_Wandering& operator=(const State_Wandering&);

public:
    /*!*******************************************************************
    \brief
	   Singleton pattern method.

    \return State_Wandering*
	    Returns pointer to instance of State_Wandering.
    ********************************************************************/
    static State_Wandering * GetInstance();

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
	   Make entity move to random direction steadily.
           When entity reach to end of tile block, reverse direction.

    \param systemMgr
	   Pointer to system manager to use engine interface.

    \param entity
	   Id of entity in this state.

    \param dt
	   Elapsed time for working base on the time.
    ********************************************************************/
    void Update(SystemManager * systemMgr, EntityId entity, float) override;
};

#endif //STATE_WANDERING_H