/******************************************************************************/
/*!
\file   State_Rush.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/04/10

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Rush.h"

#include "StateMachine.h"
#include "State_Wandering.h"

#include "../SystemManager.h"
#include "../EntityManager.h"

#include "../C_AI.h"
#include "../C_State.h"

State_Rush* State_Rush::GetInstance()
{
	static State_Rush instance;

	return &instance;
}

void State_Rush::Enter(SystemManager* systemMgr, EntityId entity)
{
	auto entityMgr = systemMgr->GetEntityManager();
	auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);

	if (C_State * state = entityMgr->GetC_State(entity))
	{ 
		EntityState entity_state = state->GetState();
		if (entity_state == EntityState::Dead || entity_state == EntityState::Hurt)
			return;
	}


	Message msg(EntityMessage::Dash, entity);

	const auto direction = AI_component->GetDirection().x;

	if (direction == 1) msg.m_direction = Direction::Right;
	else if (direction == -1) msg.m_direction = Direction::Left;
	
	systemMgr->GetMessageHandler()->Dispatch(msg);
}

bool State_Rush::HandleMessage(SystemManager* systemMgr, const Message& msg)
{
	if (msg.m_type != EntityMessage::Became_Idle) return false;

	auto entity = msg.m_receiver;

	auto entityMgr = systemMgr->GetEntityManager();
	auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);

	AI_component->GetStateMachine()->ChangeState(systemMgr, entity, State_Wandering::GetInstance());
	
	return true;
}