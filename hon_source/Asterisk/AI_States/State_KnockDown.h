#ifndef STATE_KNOCKDOWN_H
#define STATE_KNOCKDOWN_H
/*!*******************************************************************
\headerfile   State_KnockDown.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/28
\brief
              Header file for Knock downed enemy AI state class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "State.h"

/*!*******************************************************************
\class State_KnockDown
\brief
       Define state where enemy is knock downed.
********************************************************************/
class State_KnockDown : public State
{
private: // Constructors and assginment operator should be private
    /*!*******************************************************************
    \brief Priavte default constructor.
    ********************************************************************/
    State_KnockDown() = default;
    /*!*******************************************************************
    \brief
           Private copy constructor.

    \param
           Reference of this class.
    ********************************************************************/
    State_KnockDown(const State_KnockDown&);
    /*!*******************************************************************
    \brief
           Private assignment operator.

    \param
           Reference of this class.
    ********************************************************************/
    State_KnockDown & operator=(const State_KnockDown &);

public:
    /*!*******************************************************************
    \brief
           Singleton pattern method.

    \return State_KnockDown*
            Returns pointer to instance of State_KnockDown.
    ********************************************************************/
    static State_KnockDown * GetInstance();

    /*!*******************************************************************
    \brief
           Set time to stand again. Initialize timer.

    \param systemMgr
           Pointer to system manager to use engine interface.

    \param entity
           Id of entity in this state.
    ********************************************************************/
    void Enter(SystemManager* systemMgr, EntityId entity) override;
    /*!*******************************************************************
    \brief
           Waiting for standing again.

    \param systemMgr
           Pointer to system manager to use engine interface.

    \param entity
           Id of entity in this state.

    \param dt
           Elapsed time for working base on the time.
    ********************************************************************/
    void Update(SystemManager * systemMgr, EntityId entity, float) override;
};

#endif STATE_KNOCKDOWN_H