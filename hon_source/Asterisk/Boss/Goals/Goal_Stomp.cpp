/******************************************************************************/
/*!
\file   Goal_Stomp.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/05/23

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Goal_Stomp.h"

#include "..\Asterisk/SystemManager.h"
#include "..\Asterisk\EntityManager.h"
#include "..\Asterisk\C_State.h"

void Goal_Stomp::Activate(SystemManager* systemMgr, EntityId entity)
{
	Message msg(EntityMessage::Air_Stomp, entity);
	systemMgr->GetMessageHandler()->Dispatch(msg);

	m_status = completed;
}

int Goal_Stomp::Process(SystemManager* systemMgr, EntityId entity, float /*dt*/)
{
	// if status is inactive, call Activate()
	ActivateIfInactive(systemMgr, entity);

	return m_status;
}

void Goal_Stomp::Terminate(SystemManager* /*systemMgr*/, EntityId /*entity*/)
{
	m_status = completed;
}