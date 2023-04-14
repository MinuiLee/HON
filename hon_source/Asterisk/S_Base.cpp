/******************************************************************************/
/*!
\file   S_Base.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>
#include "SystemManager.h"
#include "EntityManager.h"
#include "S_Base.h"
#include "C_RigidBody.h"
#include "C_State.h"

bool S_Base::AddEntity(const EntityId id)
{
	if (HasEntity(id)) return false;

	m_entities.emplace_back(id);
	return true;
}

bool S_Base::HasEntity(const EntityId & id)
{
	auto itr = std::find(m_entities.begin(), m_entities.end(), id);
	return (itr != m_entities.end() ? true : false);
}

bool S_Base::RemoveEntity(const EntityId & id)
{
	if (!HasEntity(id)) return false;

	auto entity = std::find(m_entities.begin(), m_entities.end(), id);
	if (entity != m_entities.end()) m_entities.erase(entity);

	return true;
}

bool S_Base::FitsRequitrment(const Bitset & _bits)
{
	auto itr = std::find_if(m_requirement.begin(), m_requirement.end(),
		[&_bits](std::bitset<N_COMPONENT> bits) { return (_bits == (_bits | bits)); });
	return (itr != m_requirement.end());
}

void S_Base::Purge()
{
	m_entities.clear();
	m_requirement.clear();
}

bool S_Base::GetRandomBool(int seed) { return rand() % seed == 0; }

int S_Base::GetRandomInt(int range) { return rand() % range + 1; }

std::pair<EntityState, bool> S_Base::RequireState(const EntityId & entity) const
{
	Message msg(EntityMessage::Require_State, entity);
	m_systemMgr->GetMessageHandler()->Dispatch(msg);
	return std::make_pair(msg.m_state, msg.is_action);
}

void S_Base::ShieldBlockMessage(const EntityId & target, const EntityId & attacker)
{
	C_RigidBody * rigid = m_systemMgr->GetEntityManager()->GetC_Rigidbody(target);
	C_State * state		= m_systemMgr->GetEntityManager()->GetC_State(target);
	if (state->GetState() == EntityState::Hurt ||
        rigid->GetVelocity().x == 0)
	{
		Message shield_sound_msg(EntityMessage::ShieldBlocking, target);
		shield_sound_msg.int_value = attacker;
		m_systemMgr->GetMessageHandler()->Dispatch(shield_sound_msg);
	}
}

bool S_Base::IsEntityOnAir(const EntityId & entity)
{
	Message msg(EntityMessage::Is_OnAir, entity);
	m_systemMgr->GetMessageHandler()->Dispatch(msg);
	return msg.is_action;
}

bool S_Base::IsCollidingFragileWall(const EntityId & entity)
{
	Message msg(EntityMessage::IsCollidingFragileWall, entity);
	m_systemMgr->GetMessageHandler()->Dispatch(msg);
	return msg.is_action;
}
