/******************************************************************************/
/*!
\file   Boss.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/20

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Boss.h"

#include "..\Asterisk\SystemManager.h"
#include "..\Asterisk\EntityManager.h"

#include "..\Asterisk\C_State.h"
#include "..\Asterisk\C_Status.h"
#include "..\Asterisk\C_Position.h"
#include "..\Asterisk\C_Collidable.h"
#include "..\Asterisk\C_Attackable.h"

#include "..\Asterisk\MovementConstants.h"
#include "..\Asterisk\Mathematics.h"

#define INCREASE 5.f

Boss::Boss()
{
	m_dash_distance = (1.f/(-2*FRICTION*FORCE)) * (DASH_ENDVELOCITY * DASH_ENDVELOCITY - DASH_SPEED*DASH_SPEED);
	m_uppercut_distance = UPPERCUT_VELOCITY_Y * UPPERCUT_DELAY;
}

Boss* Boss::GetInstance()
{
	static Boss instance;

	return &instance;
}

void Boss::Update(SystemManager* systemMgr, EntityId entity, float dt)
{
	m_time += dt;
	if (m_time < INCREASE) return;

	auto status = systemMgr->GetEntityManager()->GetC_Status(entity);

	status->SetHealth(status->GetHealth() + 1);
	status->IncreaseMaxHealth();

	m_time = 0.f;
}

bool Boss::CanAct(SystemManager* systemMgr, EntityId entity) const
{
	return systemMgr->GetEntityManager()->GetC_State(entity)->CanAct();
}

bool Boss::IsStomping(SystemManager* systemMgr, EntityId entity) const
{
	return systemMgr->GetEntityManager()->GetC_State(entity)->GetDelay();
}

bool Boss::CanDash(SystemManager* systemMgr, EntityId entity) const
{
	if (CanAct(systemMgr, entity)) return true;

	auto state = systemMgr->GetEntityManager()->GetC_State(entity)->GetState();
	if (state == EntityState::Hurt || state == EntityState::Dead) return false;

	return !systemMgr->GetEntityManager()->GetC_Status(entity)->IsDoubleDashUsed();
}

bool Boss::CanUppercut(SystemManager* systemMgr, EntityId entity) const
{
	if (CanAct(systemMgr, entity)) return true;

	auto state = systemMgr->GetEntityManager()->GetC_State(entity)->GetState();
	if (state == EntityState::Hurt || state == EntityState::Dead) return false;

	return !systemMgr->GetEntityManager()->GetC_Status(entity)->IsAirUppercutUsed();
}

int Boss::CanHitPlayer(SystemManager* systemMgr, EntityId entity) const
{
	auto entityMgr = systemMgr->GetEntityManager();
	const EntityId target = systemMgr->GetPlayerID();

	const auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
	const auto targetPos = entityMgr->GetC_Position(target)->GetPosition();

	const auto entityCol = entityMgr->GetC_Collidable(entity)->GetAABB();
	const auto attackRange = entityMgr->GetC_Attackable(entity)->GetAttackRange();

	const float xdiff = targetPos.x - entityPos.x;
	const float ydiff = targetPos.y - entityPos.y;

	if (std::abs(xdiff) < attackRange.m_width)
	{
		// Can use uppercut
		if (ydiff > 0 && ydiff < m_uppercut_distance)
			return Uppercut;
		if (ydiff < 0)
			return Stomp;
	}
	if (std::abs(ydiff) < entityCol.m_height)
	{
		// Can use dash
		if (std::abs(xdiff) < m_dash_distance)
			return Dash;
	}
	
	return Impossible;
}

std::pair<bool, bool> Boss::GetDirectionToPlayer(Vector3f entity, Vector3f player) const
{
	std::pair<bool, bool> direction(false, false);

	if (entity.x > player.x) direction.first = true;
	if (entity.y > player.y) direction.second = true;

	return direction;
}