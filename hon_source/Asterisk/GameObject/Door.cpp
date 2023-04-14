/*!*******************************************************************
\file         Door.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/02/04

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <string>

#include "Door.h"

#include "../SystemManager.h"
#include "../EntityManager.h"
#include "../FontManager.h"

#include "../C_Position.h"
#include "../C_Status.h"

#include "../Text.h"
#include "../Tile.h"
#include "../BaseWindow.h"

#define DETECTRANGE Tile_Size * 4

#define CONDITION_TEXT_SCALE 0.5f

Door::Door()
{
	m_condition_UI.SetFont(*(FontManager::GetInstance()->GetPixelFont()));
	m_condition_UI.SetScale(CONDITION_TEXT_SCALE, CONDITION_TEXT_SCALE, 1.f);
}

void Door::Initialize(int condition)
{
	m_condition = condition;
	m_condition_UI.SetString(std::to_string(m_condition));
}

void Door::DrawUI(Window * window)
{
	if (m_condition) 
	{ 
		m_condition_UI.Draw(window, ENTITIES_TEXT_DEPTH);
	}
}

bool Door::IsPlayerCloseToDoor(SystemManager * systemMgr, EntityId entity, float /*dt*/)
{
	//----------------------- 1. Check the door opening animation end -----------------------------
	Message msg(EntityMessage::IsDeadAnimationEnd, entity);
	systemMgr->GetMessageHandler()->Dispatch(msg);
	if (msg.is_action)
	{
		systemMgr->GetEntityManager()->RemoveEntity(entity);
		return false;
	}

	if (!m_condition) return false;

	//----------------------- 2. Check player is enough to close ----------------------------------
	auto entityMgr			  = systemMgr->GetEntityManager();
	const unsigned int player = systemMgr->GetPlayerID();

	auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
	auto playerPos = entityMgr->GetC_Position(player)->GetPosition();

	m_condition_UI.SetPosition(entityPos.x - 11.f, entityPos.y + 33.f);

	const float xdiff	= playerPos.x - entityPos.x;
	const float ydiff	= playerPos.y - entityPos.y;
	const float distance = sqrt(xdiff * xdiff + ydiff * ydiff);

	if (distance > DETECTRANGE) return false;

	//----------------------- 3. Set UI Position --------------------------------------------------
	m_UI->SetPosition(entityPos);

	return true;
}

bool Door::IsSoulEnoughToOpenDoor(SystemManager * systemMgr, Message * message)
{
	if (!m_condition) return true;

	auto entityMgr = systemMgr->GetEntityManager();

	const unsigned int entity = message->m_receiver;
	const unsigned int player = systemMgr->GetPlayerID();

	auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
	auto playerPos = entityMgr->GetC_Position(player)->GetPosition();

	//----------------------- 1. Check player is enough to close ----------------------------------
	const float xdiff	= playerPos.x - entityPos.x;
	const float ydiff	= playerPos.y - entityPos.y;
	const float distance = sqrt(xdiff * xdiff + ydiff * ydiff);

	if (distance > DETECTRANGE) return false;

	//----------------------- 2. Process message --------------------------------------------------
	auto msg_type = message->m_type;

	if (msg_type == EntityMessage::Pay_Purple_Soul)
	{
		auto status = entityMgr->GetC_Status(player);

		auto souls = status->GetSouls();
		if (souls.x + souls.y <= 1) return false;

		if (souls.x)
		{
			status->AddSouls(-1, 0);
			--m_condition;

			m_condition_UI.SetString(std::to_string(m_condition));
		}
	}
	else if (msg_type == EntityMessage::Pay_Blue_Soul)
	{
		auto status = entityMgr->GetC_Status(player);

		auto souls = status->GetSouls();
		if (souls.x + souls.y <= 1) return false;

		if (souls.y)
		{
			status->AddSouls(0, -1);
			--m_condition;

			m_condition_UI.SetString(std::to_string(m_condition));
		}
	}

	if (!m_condition)
	{
		Message msg(EntityMessage::Dead, entity);
		systemMgr->GetMessageHandler()->Dispatch(msg);
		return true;
	}

	return false;
}

void Door::IncreaseCondition()
{
	++m_condition;
	m_condition_UI.SetString(std::to_string(m_condition));
}

void Door::DecreaseCondition()
{
	--m_condition;
	m_condition_UI.SetString(std::to_string(m_condition));
}