/******************************************************************************/
/*!
\file   Goal_Uppercut.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/23

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Goal_Uppercut.h"

#include "Boss.h"

#include "..\Asterisk\SystemManager.h"
#include "..\Asterisk\EntityManager.h"

#include "..\Asterisk\C_Status.h"
#include "..\Asterisk\C_State.h"

void Goal_Uppercut::Activate(SystemManager* systemMgr, EntityId entity)
{
	C_State * state = systemMgr->GetEntityManager()->GetC_State(entity);
	if (state->GetState() == EntityState::Air_Stomp)
		return;

	Message msg(EntityMessage::Uppercut, entity);
	systemMgr->GetMessageHandler()->Dispatch(msg);

	if (!Boss::GetInstance()->CanAct(systemMgr, entity))
		systemMgr->GetEntityManager()->GetC_Status(entity)->SetAirUppercutUsed(true);
	m_status = completed;
}

int Goal_Uppercut::Process(SystemManager* systemMgr, EntityId entity, float /*dt*/)
{
	// if status is inactive, call Activate()
	ActivateIfInactive(systemMgr, entity);

	return m_status;
}

void Goal_Uppercut::Terminate(SystemManager* /*systemMgr*/, EntityId /*entity*/)
{
	m_status = completed;
}