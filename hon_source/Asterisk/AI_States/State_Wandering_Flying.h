#ifndef STATE_WANDERING_FLYNG_H
#define STATE_WANDERING_FLYNG_H
/*!*******************************************************************
\headerfile   State_Wandering_Flying.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/26
\brief
              Header file for Wandering AI state class for flying enemy.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class State_Wandering_Flying
\brief
       Define state where flying enemy is wandering.
********************************************************************/
class State_Wandering_Flying : public State
{
private: // Constructors and assginment operator should be private
    /*!*******************************************************************
    \brief Priavte default constructor.
    ********************************************************************/
    State_Wandering_Flying() = default;
    /*!*******************************************************************
    \brief
	       Private copy constructor.

    \param 
	       Reference of this class.
    ********************************************************************/
    State_Wandering_Flying(const State_Wandering_Flying&);
    /*!*******************************************************************
    \brief
	       Private assignment operator.

    \param
	       Reference of this class.
    ********************************************************************/
    State_Wandering_Flying & operator=(const State_Wandering_Flying &);

public:
    /*!*******************************************************************
    \brief
	       Singleton pattern method.

    \return State_Wandering_Flying*
	        Returns pointer to instance of State_Wandering_Flying.
    ********************************************************************/
    static State_Wandering_Flying * GetInstance();

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

    \param systemMgr
	       Pointer to system manager to use engine interface.

    \param entity
	       Id of entity in this state.

    \param dt
	       Elapsed time for working base on the time.
    ********************************************************************/
    void Update(SystemManager * systemMgr, EntityId entity, float) override;
};

#endif //STATE_WANDERING_FLYNG_H