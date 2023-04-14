/******************************************************************************/
/*!
\file   Goal_Dash.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/22

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Goal_Dash.h"

#include "Boss.h"

#include "..\Asterisk/SystemManager.h"
#include "..\Asterisk\EntityManager.h"

#include "..\Asterisk\C_Status.h"

void Goal_Dash::Activate(SystemManager* systemMgr, EntityId entity)
{
	Message msg(EntityMessage::Dash, entity);
	msg.m_direction = m_left ? Direction::Left : Direction::Right;

	systemMgr->GetMessageHandler()->Dispatch(msg);

	msg.m_type = EntityMessage::Direction_Changed;
	systemMgr->GetMessageHandler()->Dispatch(msg);

	if (!Boss::GetInstance()->CanAct(systemMgr, entity))
		systemMgr->GetEntityManager()->GetC_Status(entity)->SetDoubleDashUsed(true);
	m_status = completed;
}

int Goal_Dash::Process(SystemManager* systemMgr, EntityId entity, float /*dt*/)
{
	// if status is inactive, call Activate()
	ActivateIfInactive(systemMgr, entity);

	return m_status;
}

void Goal_Dash::Terminate(SystemManager* /*systemMgr*/, EntityId /*entity*/)
{
	m_status = completed;
}