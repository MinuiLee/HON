/******************************************************************************/
/*!
\file   State_Bouncing.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2019/04/03

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Bouncing.h"

#include "StateMachine.h"

#include "..\SystemManager.h"
#include "..\EntityManager.h"

#include "..\C_AI.h"
#include "..\C_Position.h"
#include "..\C_Collidable.h"
#include "..\Mathematics.h"
#include "../C_RigidBody.h"

constexpr float TIME  = 0.24f;
constexpr float X_VEL = 100.f;
constexpr float Y_VEL = 200.f;

State_Bouncing * State_Bouncing::GetInstance()
{
	static State_Bouncing instance;
	return &instance;
}

void State_Bouncing::Enter(SystemManager * systemMgr, EntityId entity)
{
	Message msg(EntityMessage::Became_Idle, entity);
	systemMgr->GetMessageHandler()->Dispatch(msg);
}

void State_Bouncing::Update(SystemManager * systemMgr, EntityId entity, float dt)
{
	auto entityMgr			  = systemMgr->GetEntityManager();
	const unsigned int target = systemMgr->GetPlayerID();

	auto AI_component	= entityMgr->GetC_AI(entity);
	auto RIGID_component = entityMgr->GetC_Rigidbody(entity);
	const auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
	const auto targetPos = entityMgr->GetC_Position(target)->GetPosition();

	const float xdiff	= targetPos.x - entityPos.x;
	const float ydiff	= targetPos.y - entityPos.y;
	const float distance = sqrt(xdiff * xdiff + ydiff * ydiff);

	bool collideWithTile = entityMgr->GetC_Collidable(entity)->GetIsCollidingWithTile();

	if (collideWithTile)
	{
		AI_component->AddElapsedTime(dt);
		RIGID_component->SetVelocity({ 0.f, 0.f, 0.f });

		Message msg(EntityMessage::Direction_Changed, entity);
		if (AI_component->GetDirection().x)
			msg.m_direction = Direction::Right;
		else
			msg.m_direction = Direction::Left;

		systemMgr->GetMessageHandler()->Dispatch(msg);

		Message msg1(EntityMessage::Became_Idle, entity);
		systemMgr->GetMessageHandler()->Dispatch(msg1);
	}
	else
	{
		Message msg(EntityMessage::Moving, entity);
		msg.m_direction = Direction::None;
		systemMgr->GetMessageHandler()->Dispatch(msg);
	}

	if (AI_component->GetElapsedTime() > TIME)
	{
		if (collideWithTile)
		{
			AI_component->SetElapsedTime(0.f);
			AI_component->SetDirection(
				{ Mathematics::GetRandomInt(0, 1), 0 } // 0 = left, 1 = right
			);
		}

		if (distance <= AI_component->GetChaseRange())
		{ RIGID_component->SetVelocity({ xdiff, 256 + ydiff, 0.f }); } else
		{
			RIGID_component->SetVelocity(
				{ AI_component->GetDirection().x ? X_VEL : -X_VEL, 256 + Y_VEL, 0.f }
			);
		}
	}
}