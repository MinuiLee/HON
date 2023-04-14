/******************************************************************************/
/*!
\file   State_Wandering_Flying.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2019/01/26

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Wandering_Flying.h"

#include "StateMachine.h"
#include "State_Attack_Flying.h"

#include "..\SystemManager.h"
#include "..\EntityManager.h"

#include "..\C_AI.h"
#include "..\C_Position.h"
#include "..\C_Collidable.h"
#include "..\C_SpriteSheet.h"
#include "..\C_Rigidbody.h"
#include "..\S_State.h"

#include "..\Mathematics.h"

constexpr int MOVING_TIME_MIN	  = 1;
constexpr int MOVING_TIME_MAX	  = 5;
constexpr int VELOCITY_MIN		   = 100;
constexpr int VELOCITY_MAX		   = 200;
constexpr int VELOCITY_X_MAX	   = 400;
constexpr float DIVISION_FOR_FLOAT = 1.f;
constexpr float TIME_LIMIT		   = 3.f;

State_Wandering_Flying * State_Wandering_Flying::GetInstance()
{
	static State_Wandering_Flying instance;
	return &instance;
}

void State_Wandering_Flying::Enter(SystemManager * systemMgr, EntityId entity)
{
	auto entityMgr	= systemMgr->GetEntityManager();
	auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);

	AI_component->SetCondition(0.f);
	AI_component->SetTimer(0.f);
	AI_component->SetElapsedTime(0.f);
}

void State_Wandering_Flying::Update(SystemManager * systemMgr, EntityId entity, float dt)
{
	auto entityMgr			  = systemMgr->GetEntityManager();
    if (systemMgr->GetSystem<S_State>(System::State)->RequireState(entity).first == EntityState::Dead)
    {
        entityMgr->GetC_Rigidbody(entity)->SetVelocity(Vector3f(0.f, 0.f, 0.f));
        return;
    }
	const unsigned int target = systemMgr->GetPlayerID();

	auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
	auto targetPos = entityMgr->GetC_Position(target)->GetPosition();

	//----------------------- 1. Compute distance between entity and target -----------------------
	const float xdiff	= targetPos.x - entityPos.x;
	const float ydiff	= targetPos.y - entityPos.y;
	const float distance = sqrt(xdiff * xdiff + ydiff * ydiff);

	//----------------------- 2. Check distance is enough to change state -------------------------
	auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);
	AI_component->AddElapsedTime(dt);

	if (distance < AI_component->GetChaseRange() && AI_component->GetElapsedTime() >= TIME_LIMIT)
	{
		AI_component->GetStateMachine()->ChangeState(
			systemMgr, entity, State_Attack_Flying::GetInstance());
		AI_component->SetDirection({ targetPos.x, targetPos.y });
		AI_component->SetCondition(distance);
		return;
	}

	//----------------------- 3. Check time is passed enough to change direction ------------------
	Vector2i direction = AI_component->GetDirection();

	if (AI_component->GetCondition() <= AI_component->GetTimer() ||
		entityMgr->GetC_Collidable(entity)->GetIsCollidingWithTile())
	{
		float x = Mathematics::GetRandomInt(VELOCITY_MIN, VELOCITY_X_MAX) / DIVISION_FOR_FLOAT;
		float y = Mathematics::GetRandomInt(VELOCITY_MIN, VELOCITY_MAX) / DIVISION_FOR_FLOAT;
		Mathematics::GetRandomInt(0, 1) ? x = -x : x = x;
		Mathematics::GetRandomInt(0, 1) ? y = -y : y = y;

		AI_component->SetDirection({ x, y });
		AI_component->SetCondition(
			Mathematics::GetRandomInt(MOVING_TIME_MIN, MOVING_TIME_MAX) / DIVISION_FOR_FLOAT);
		AI_component->SetTimer(0.f);
	}

	//----------------------- 4. Make entity move -------------------------------------------------
	entityMgr->GetC_Position(entity)->MoveBy(
		AI_component->GetDirection().x * dt, AI_component->GetDirection().y * dt);

	Message msg(EntityMessage::Moving, entity);
	msg.m_direction = Direction::None;
	systemMgr->GetMessageHandler()->Dispatch(msg);

	AI_component->AddElapsedTime(dt);
}