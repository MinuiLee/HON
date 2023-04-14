/******************************************************************************/
/*!
\file   Goal_Flee.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/26

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Goal_Flee.h"

#include "Boss.h"

#include "Goal_Dash.h"
#include "Goal_Uppercut.h"
#include "Goal_Stomp.h"

#include "..\Asterisk\SystemManager.h"
#include "..\Asterisk\EntityManager.h"

#include "..\Asterisk\C_Position.h"

#include "..\Asterisk\Tile.h"

#define LEFT -87
#define RIGHT -57
#define COUNT 3

void Goal_Flee::Activate(SystemManager* systemMgr, EntityId entity)
{
	m_status = active;

	// If this goal is reactivated then there may be some existing subgoals.
	RemoveAllSubgoals(systemMgr, entity);

	auto boss = Boss::GetInstance();
	if (boss->IsStomping(systemMgr, entity)) return;

	bool canDash = boss->CanDash(systemMgr, entity);
	bool canUppercut = boss->CanUppercut(systemMgr, entity);

	auto entityMgr = systemMgr->GetEntityManager();
	const EntityId target = systemMgr->GetPlayerID();

	auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
	auto targetPos = entityMgr->GetC_Position(target)->GetPosition();

	auto direction = boss->GetDirectionToPlayer(entityPos, targetPos);
	if (m_count && canDash)
	{
		if (m_count == COUNT)
		{
			if (entityPos.x > (LEFT + RIGHT) / 2 * Tile_Size)
				m_left = true;
			else m_left = false;
		}

		AddSubgoal(new Goal_Dash(m_left));
		m_count--;
		return;
	}

	switch (boss->CanHitPlayer(systemMgr, entity))
	{
	case Boss::Dash:
		if (canDash)
			AddSubgoal(new Goal_Dash(direction.first));
		return;

	case Boss::Uppercut:
		if (canUppercut)
			AddSubgoal(new Goal_Uppercut());
		return;

	case Boss::Stomp:
		if (boss->CanAct(systemMgr, entity))
			AddSubgoal(new Goal_Stomp());
		return;
	}

	if (canDash)
		AddSubgoal(new Goal_Dash(!direction.first));
}

int Goal_Flee::Process(SystemManager* systemMgr, EntityId entity, float dt)
{
	if (m_count == 1)
	{
		m_status = completed;
		return m_status;
	}

	Activate(systemMgr, entity);

	// process the subgoals
	if (ProcessSubgoals(systemMgr, entity, dt) == failed)
		m_status = failed;
	else
		m_status = active;

	return m_status;
}

void Goal_Flee::Terminate(SystemManager* /*systemMgr*/, EntityId /*entity*/)
{
	m_status = completed;
}

bool Goal_Flee::HandleMessage(SystemManager* /*systemMgr*/, const Message& msg)
{
	const auto type = msg.m_type;

	switch (type)
	{
	case EntityMessage::GetAttacked:
		m_status = failed;
		return true;

	case EntityMessage::SucceedAttack:
		m_count = COUNT;
		return true;
	}

	return false;
}