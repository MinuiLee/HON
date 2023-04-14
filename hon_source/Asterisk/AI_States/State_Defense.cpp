/******************************************************************************/
/*!
\file   State_Defense.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/03/22

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Defense.h"

#include "StateMachine.h"
#include "State_Wandering.h"
#include "State_Rush.h"

#include "../SystemManager.h"
#include "../EntityManager.h"

#include "../C_AI.h"
#include "../C_Position.h"
#include "..\C_Sprite.h"
#include "..\C_SpriteSheet.h"

#define WAITING_TIME 2.f

State_Defense* State_Defense::GetInstance()
{
	static State_Defense instance;

	return &instance;
}

void State_Defense::Enter(SystemManager* systemMgr, EntityId entity)
{
	Message msg(EntityMessage::Became_Idle, entity);
	systemMgr->GetMessageHandler()->Dispatch(msg);

	auto entityMgr = systemMgr->GetEntityManager();
	auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);

	AI_component->SetCondition(WAITING_TIME);
	AI_component->SetTimer(0.f);
}

void State_Defense::Update(SystemManager * systemMgr, EntityId entity, float)
{
	auto entityMgr = systemMgr->GetEntityManager();
	const unsigned int target = systemMgr->GetPlayerID();

	const auto entityPos = entityMgr->GetComponent<C_Position>(entity, Component::Position)->GetPosition();
	const auto targetPos = entityMgr->GetComponent<C_Position>(target, Component::Position)->GetPosition();

	auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);

	//----------------------- 1. Check time is enough to change state -------------------------
	if (AI_component->GetCondition() <= AI_component->GetTimer())
		AI_component->GetStateMachine()->ChangeState(systemMgr, entity, State_Rush::GetInstance());

	//----------------------- 2. Check position of player to change direction ---------------------
	Message msg(EntityMessage::Direction_Changed, entity);

	if (targetPos.x < entityPos.x)
	{
		msg.m_direction = Direction::Left;
		AI_component->SetDirection(Vector2i(-1, 0));
	}
	else
	{
		msg.m_direction = Direction::Right;
		AI_component->SetDirection(Vector2i(1, 0));
	}

	systemMgr->GetMessageHandler()->Dispatch(msg);
}