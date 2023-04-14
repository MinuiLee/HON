/******************************************************************************/
/*!
\file   StateMachine.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/01/03

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "StateMachine.h"
#include "State.h"

#include <cassert>
#include <typeinfo>

void StateMachine::Update(SystemManager * systemMgr, EntityId entity, float dt) const
{
    // If a global state exists, call its update method, else do nothing.
    if (m_GlobalState) m_GlobalState->Update(systemMgr, entity, dt);

    // Same for the current state
    if (m_CurrentState) m_CurrentState->Update(systemMgr, entity, dt);
}

bool StateMachine::HandleMessage(SystemManager * systemMgr, const Message & msg) const
{
    // See if the current state is valid and that it can handle the message
    if (m_CurrentState && m_CurrentState->HandleMessage(systemMgr, msg)) return true;

    // If not, and if a global state has been implemented,
    // send the message to the global state
    if (m_GlobalState && m_GlobalState->HandleMessage(systemMgr, msg)) return true;

    return false;
}

void StateMachine::ChangeState(SystemManager * systemMgr, EntityId entity, State * newState)
{
    assert(newState && "<StateMachine::ChangeState> Trying to assign null state to current");

    // Keep a record of the previous state
    m_PreviousState = m_CurrentState;

    // Call the exit method of the existing state
    m_CurrentState->Exit(systemMgr, entity);

    // Change state to the new state
    m_CurrentState = newState;

    // Call the enter method of the new state
    m_CurrentState->Enter(systemMgr, entity);
}

bool StateMachine::IsInState(State * const state) const
{
    if (typeid(*m_CurrentState) == typeid(*state)) return true;

    return false;
}

std::string StateMachine::GetNameOfCurrentState() const
{
    std::string name(typeid(*m_CurrentState).name());

    // Remove the 'class' part from the front of the string.
    if (name.size() > 5) name.erase(0, 6);

    return name;
}