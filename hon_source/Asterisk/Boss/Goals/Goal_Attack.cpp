/******************************************************************************/
/*!
\file   Goal_Attack.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/18

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Goal_Attack.h"

#include "Boss.h"

#include "Goal_Dash.h"
#include "Goal_Uppercut.h"
#include "Goal_Stomp.h"

#include "..\Asterisk\SystemManager.h"
#include "..\Asterisk\EntityManager.h"

#include "..\Asterisk\C_Position.h"

void Goal_Attack::Activate(SystemManager* systemMgr, EntityId entity)
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
	
	switch (boss->CanHitPlayer(systemMgr, entity))
	{
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
		AddSubgoal(new Goal_Dash(direction.first));
}

int Goal_Attack::Process(SystemManager* systemMgr, EntityId entity, float dt)
{
	Activate(systemMgr, entity);

	// process the subgoals
	if (ProcessSubgoals(systemMgr, entity, dt) == failed)
		m_status = failed;
	else
		m_status = active;

	return m_status;
}

void Goal_Attack::Terminate(SystemManager* /*systemMgr*/, EntityId /*entity*/)
{
	m_status = completed;
}

bool Goal_Attack::HandleMessage(SystemManager* /*systemMgr*/, const Message& msg)
{
	const auto type = msg.m_type;

	if (type == EntityMessage::GetAttacked)
	{
		m_status = failed;
		return true;
	}

	return false;
}