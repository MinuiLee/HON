/******************************************************************************/
/*!
\file   S_AI.cpp
\author Primary: Minui Lee
		Second: Kim Hyungseob
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <math.h>

#include "S_AI.h"

#include "C_AI.h"
#include "C_Position.h"
#include "C_RigidBody.h"

#include "SystemManager.h"
#include "EntityManager.h"

#include "AI_States\StateMachine.h"
#include "Boss/Goals/Goal_Think.h"

#define OFFSET 1
#define SEED 71
using namespace glm;

S_AI::S_AI(SystemManager * sysMgr) : S_Base(System::AI, sysMgr)
{
	Bitset bits;
	bits.set((size_t)Component::Position, 1);
	bits.set((size_t)Component::RigidBody, 1);
	bits.set((size_t)Component::AI, 1);
	m_requirement.emplace_back(bits);

    m_systemMgr->GetMessageHandler()->Subscribe(EntityMessage::GetAttacked, this);
	m_systemMgr->GetMessageHandler()->Subscribe(EntityMessage::Became_Idle, this);
	m_systemMgr->GetMessageHandler()->Subscribe(EntityMessage::SucceedAttack, this);
}

void S_AI::Update(Window * window, float dt, bool /*editor_mode*/)
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
    FloatRect view_space = window->GetBaseWindow()->GetUpdateCullingSpace();

    for (auto entity : m_entities)
    {
		if (auto AI = entityMgr->GetC_AI(entity); AI)
		{
			if (AI->GetType() == EnemyType::Boss)
				AI->GetBrain()->Process(m_systemMgr, entity, dt);
			else
				AI->GetStateMachine()->Update(m_systemMgr, entity, dt);
		}
    }
}

void S_AI::Notify(Message & message)
{
    auto entity = message.m_receiver;

    EntityManager* entityMgr = m_systemMgr->GetEntityManager();
    C_AI* AI = entityMgr->GetC_AI(entity);

	if (AI->GetType() == EnemyType::Boss)
	{
		AI->GetBrain()->HandleMessage(m_systemMgr, message);
		return;
	}
	
    switch (message.m_type)
    {
    case EntityMessage::GetAttacked:
        AI->GetStateMachine()->HandleMessage(m_systemMgr, message);
        break;

	case EntityMessage::Became_Idle:
		AI->GetStateMachine()->HandleMessage(m_systemMgr, message);
		break;
    }
}

void S_AI::SendMessages(const EntityId & entity)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();

	// Patrol(entity);
	C_AI * AI = entityMgr->GetComponent<C_AI>(entity, Component::AI);
	if (!AI) return;

	unsigned int target = m_systemMgr->GetPlayerID();

	if (!entityMgr->HasEntity(target)) return;

	C_Position * entity_tran = entityMgr->GetComponent<C_Position>(entity, Component::Position);
	C_Position * target_tran = entityMgr->GetComponent<C_Position>(target, Component::Position);

    vec3 entitypos = entity_tran->GetPosition();
    vec3 targetpos = target_tran->GetPosition();

	float xdiff = targetpos.x - entitypos.x;
	float zdiff = targetpos.z - entitypos.z;

	Message m(EntityMessage::State_Changed, entity);

	C_RigidBody * rigid = entityMgr->GetComponent<C_RigidBody>(entity, Component::RigidBody);

	// Too close or too far => stop !
	if (sqrt(xdiff * xdiff + zdiff * zdiff) >= 200.f || sqrt(xdiff * xdiff + zdiff * zdiff) <= 50.f)
	{
		rigid->SetVelocity(vec3(0, 0, 0));

		if (sqrt(xdiff * xdiff + zdiff * zdiff) <= 50.f)
		{
			if (GetRandomBool(SEED))
			{
				Message attack_message(EntityMessage::Attack, entity);
				m_systemMgr->GetMessageHandler()->Dispatch(attack_message);
			}
		}
		return;
	}

	if (entitypos.x < targetpos.x - OFFSET)
	{
		m.m_direction = Direction::Right;
        m.m_state = EntityState::Walk;
		m_systemMgr->GetMessageHandler()->Dispatch(m);
	}
	else if (entitypos.x > targetpos.x + OFFSET)
	{
		m.m_direction = Direction::Left;
        m.m_state = EntityState::Walk;
		m_systemMgr->GetMessageHandler()->Dispatch(m);
	}
}

void S_AI::Patrol(int entityid)
{
	if (GetRandomBool(SEED))
	{
		Message m(EntityMessage::Moving, entityid);
		Direction dir	= static_cast<Direction>(GetRandomInt(4) - 1);
		m.m_direction	= dir;
		m.float_value = 100;

		m_systemMgr->GetMessageHandler()->Dispatch(m);
	}
}
