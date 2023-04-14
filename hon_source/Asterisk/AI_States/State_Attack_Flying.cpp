/******************************************************************************/
/*!
\file   State_Attack_Flying.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2019/01/26

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>

#include "State_Attack_Flying.h"

#include "StateMachine.h"
#include "State_Wandering_Flying.h"

#include "..\SystemManager.h"
#include "..\EntityManager.h"

#include "..\C_AI.h"
#include "..\C_Position.h"
#include "..\C_Collidable.h"
#include "..\C_Rigidbody.h"
#include "..\S_State.h"

#include "..\Mathematics.h"

constexpr float SPEED = 600.f;

State_Attack_Flying * State_Attack_Flying::GetInstance()
{
    static State_Attack_Flying instance;

    return &instance;
}

void State_Attack_Flying::Enter(SystemManager * systemMgr, EntityId entity)
{
    auto entityMgr	= systemMgr->GetEntityManager();
	m_initPosition = entityMgr->GetC_Position(entity)->GetPosition();
	m_bool			= false;
}

void State_Attack_Flying::Update(SystemManager * systemMgr, EntityId entity, float dt)
{
    auto entityMgr = systemMgr->GetEntityManager();
    if (systemMgr->GetSystem<S_State>(System::State)->RequireState(entity).first == EntityState::Dead)
    {
        entityMgr->GetC_Rigidbody(entity)->SetVelocity(Vector3f(0.f, 0.f, 0.f));
        return;
    }
    auto AI_component = entityMgr->GetC_AI(entity);

	auto targetPos = AI_component->GetDirection();

	const auto xdiff	 = targetPos.x - m_initPosition.x;
	const auto ydiff	 = targetPos.y - m_initPosition.y;
	const float distance = sqrt(xdiff * xdiff + ydiff * ydiff);

    //----------------------- 2. Check attacking is finished --------------------------------------
	if (AI_component->GetCondition() <= 0.f && !m_bool)
    {
		auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
		m_bool		   = true;
		
		AI_component->SetCondition(distance);
		AI_component->SetDirection({ m_initPosition.x + xdiff * 2.f, m_initPosition.y });
		m_initPosition = entityPos;
		return;
    }

	if (entityMgr->GetC_Collidable(entity)->GetIsCollidingWithTile() || 
		(m_bool && AI_component->GetCondition() <= 0.f))
		AI_component->GetStateMachine()->ChangeState(systemMgr, entity, State_Wandering_Flying::GetInstance());
    //----------------------- 3. Make entity keep moving -------------------------------------------------

	float x = xdiff / distance * SPEED, y = ydiff / distance * SPEED;
	entityMgr->GetC_Position(entity)->MoveBy(x * dt, y *dt);

	AI_component->SetCondition(AI_component->GetCondition() - sqrt(x * x + y * y) * dt);

	Message msg(EntityMessage::Became_Idle, entity);
	systemMgr->GetMessageHandler()->Dispatch(msg);
}