#ifndef STATEMACHINE_H
#define STATEMACHINE_H
/*!*******************************************************************
\headerfile   StateMachine.h
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/03
\brief
	          Header file for state machine class.
\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <string>

#include "..\Observer.h"

using EntityId = unsigned int;
class State;
class SystemManager;

/*!*******************************************************************
\class StateMachine
\brief
       Define interface of Finite State Machine.
********************************************************************/
class StateMachine
{
private:
    State* m_CurrentState  = nullptr; //!< Pointer to state that agent is in.
    State* m_PreviousState = nullptr; //!< A record of the last state the agent was in.
    State* m_GlobalState   = nullptr; //!< This is called every time the FSM is updated.

public:
    /*!*******************************************************************
    \brief Default constructor.
    ********************************************************************/
    StateMachine() = default;

    /*!*******************************************************************
    \brief Virtual destructor.
    ********************************************************************/
    virtual ~StateMachine() {}

    /*!*******************************************************************
    \brief
	       Setter method for current state to initialize state machine.

    \param state
	       Pointer to state that is set for current state.
    ********************************************************************/
    void SetCurrentState(State* state) { m_CurrentState = state; }
    /*!*******************************************************************
    \brief
	       Setter method for previous state to initialize state machine.

    \param state
	       Pointer to state that is set for previous state.
    ********************************************************************/
    void SetPreviousState(State* state) { m_PreviousState = state; }
    /*!*******************************************************************
    \brief
	       Setter method for global state to initialize state machine.

    \param state
	       Pointer to state that is set for global state.
    ********************************************************************/
    void SetGlobalState(State* state) { m_GlobalState = state; }

    /*!*******************************************************************
    \brief
	       Getter method for current state.

    \return State*
	        Pointer to current state.
    ********************************************************************/
    State* GetCurrentState() const { return m_CurrentState; }
    /*!*******************************************************************
    \brief
	       Getter method for previous state.

    \return State*
	        Pointer to previous state.
    ********************************************************************/
    State* GetPreviousState() const { return m_PreviousState; }
    /*!*******************************************************************
    \brief
	       Getter method for global state.

    \return State*
	        Pointer to global state.
    ********************************************************************/
    State* GetGlobalState() const { return m_GlobalState; }

    /*!*******************************************************************
    \brief
	       Update method for state machine.

    \param systemMgr
	       Pointer to system manager to use engine interface.

    \param entity
	       Id of entity in this state.

    \param dt
           Elapsed time for working base on the time.
    ********************************************************************/
    void Update(SystemManager* systemMgr, EntityId entity, float dt) const;

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
    bool HandleMessage(SystemManager* systemMgr, const Message & msg) const;

    /*!*******************************************************************
    \brief
	       Update method for state machine.

    \param systemMgr
	       Pointer to system manager to use engine interface.
           
    \param entity
	       Id of entity in this state.

    \param newState
	       Pointer to state that will replace current state.
    ********************************************************************/
    void ChangeState(SystemManager* systemMgr, EntityId entity, State* newState);

    /*!*******************************************************************
    \brief
	       Change state back to the previous state.

    \param systemMgr
	       Pointer to system manager to use engine interface.
    ********************************************************************/
    void Revert_To_Previous_State(SystemManager* systemMgr, EntityId entity) { ChangeState(systemMgr, entity, m_PreviousState); }

    /*!*******************************************************************
    \brief
	       Check whether entity is in given state or not.

    \param state
	       Const pointer to state to check.

    \return bool
            Returns true if the current state's type is equal to the
            type of the class passed as a parameter.
            Else, returns false.
    ********************************************************************/
    bool IsInState(State * const state) const;

    /*!*******************************************************************
    \brief
	       Useful for debugging to grab the name of the current state.

    \return std::string
	        The name of the current state.
    ********************************************************************/
    std::string GetNameOfCurrentState() const;
};

#endif //STATEMACHINE_H