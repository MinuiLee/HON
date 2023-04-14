/******************************************************************************/
/*!
\file   S_State.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_State.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_State.h"

S_State::S_State(SystemManager * sysMgr)
    : S_Base(System::State, sysMgr)
{
    Bitset bits;
    bits.set((size_t)Component::State, 1);
    m_requirement.push_back(bits);

    MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::Direction_Changed, this);
    handler->Subscribe(EntityMessage::Require_State, this);

    handler->Subscribe(EntityMessage::Moving, this);
    handler->Subscribe(EntityMessage::Jumping, this);
    handler->Subscribe(EntityMessage::Became_Idle, this);
    handler->Subscribe(EntityMessage::Dash, this);
    handler->Subscribe(EntityMessage::Stomp, this);
    handler->Subscribe(EntityMessage::Air_Stomp, this);
    handler->Subscribe(EntityMessage::Uppercut, this);
	handler->Subscribe(EntityMessage::Knockdown, this);

    handler->Subscribe(EntityMessage::GetAttacked, this);
    handler->Subscribe(EntityMessage::Pushed, this);
    handler->Subscribe(EntityMessage::Dead, this);
    handler->Subscribe(EntityMessage::ReleaseFlower, this);
}

//Observer method
void S_State::Notify(Message & message)
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    C_State * state = entityMgr->GetC_State(message.m_receiver);

    switch (message.m_type)
    {
	case EntityMessage::Direction_Changed:
	{
		if (state->GetDirection() != message.m_direction)
			state->SetDirection(message.m_direction);
		break;
	}
    case EntityMessage::Require_State:
    {
        message.m_state = state->GetState();
        message.is_action = state->IsAction();
        message.int_value = static_cast<int>(state->GetOldState());
        break;
    }
    case EntityMessage::Pushed:
    case EntityMessage::GetAttacked:
    {
        if(state->GetState() == EntityState::Dead) break;
        state->SetAction(true);
        state->SetState(EntityState::Hurt);

        State_Changed_Message(message.m_receiver, EntityState::Hurt);
        break;
    }
    case EntityMessage::Moving:
    {
        if (state->IsAction()) return;
        state->SetState(EntityState::Walk);
        state->SetDirection(message.m_direction);

        State_Changed_Message(message.m_receiver, EntityState::Walk);
        break;
    }
    case EntityMessage::Became_Idle:
    {
		EntityState current_state = state->GetState();
		if (state->IsAction() && current_state != EntityState::Jump) return;
		if (current_state == EntityState::Jump) state->SetAction(false);

        state->SetState(EntityState::Idle);
        State_Changed_Message(message.m_receiver, EntityState::Idle);
        break;
    }
    case EntityMessage::Jumping:
    {
        state->SetState(EntityState::Jump);
        State_Changed_Message(message.m_receiver, EntityState::Jump);
        break;
    }
    case EntityMessage::Dash:
    {
        state->SetState(EntityState::Dash);
        state->SetDirection(message.m_direction);
        state->SetAction(true);
        break;
    }
    case EntityMessage::Stomp:
    {
        state->SetState(EntityState::Stomp);
        state->SetAction(true);
        State_Changed_Message(message.m_receiver, EntityState::Stomp);
        break;
    }
    case EntityMessage::Air_Stomp:
    {
        state->SetState(EntityState::Air_Stomp);
        state->SetAction(true);
        State_Changed_Message(message.m_receiver, EntityState::Air_Stomp);
        break;
    }
	case EntityMessage::Knockdown:
	{
		state->SetState(EntityState::Knockdown);
		state->SetAction(true);
		State_Changed_Message(message.m_receiver, EntityState::Knockdown);
		break;
	}
    case EntityMessage::Dead:
    {
        state->SetState(EntityState::Dead);
        state->SetAction(true);
        State_Changed_Message(message.m_receiver, EntityState::Dead);
        break;
    }
    case EntityMessage::Uppercut:
    {
        state->SetState(EntityState::Uppercut);
        state->SetAction(true);
        State_Changed_Message(message.m_receiver, EntityState::Uppercut);
        break;
    }
    case EntityMessage::ReleaseFlower:
    {
        if(message.int_value) // Is vase object?
            state->SetState(EntityState::Other);
        break;
    }
    }
}

void S_State::State_Changed_Message(const EntityId & entity, EntityState state)
{
    Message msg(EntityMessage::State_Changed, entity);
    msg.m_state = state;
    m_systemMgr->GetMessageHandler()->Dispatch(msg);
}
