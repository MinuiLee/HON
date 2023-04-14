/******************************************************************************/
/*!
\file   S_Control.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_Controllable.h"
#include "C_Status.h"
#include "C_State.h"
#include "S_Control.h"
#include "EntityState.h"

// Ctor
S_Control::S_Control(SystemManager * systemMgr) : S_Base(System::Control, systemMgr)
{
	Bitset bits;
	bits.set((size_t)Component::Controllable, 1);
	m_requirement.emplace_back(bits);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::Player_Moving, this);
	handler->Subscribe(EntityMessage::Player_Jumping, this);
	handler->Subscribe(EntityMessage::Player_Idle, this);
	handler->Subscribe(EntityMessage::Player_Dash, this);
	handler->Subscribe(EntityMessage::Player_Uppercut, this);
	handler->Subscribe(EntityMessage::Player_Stomp, this);
}

// Observer method
void S_Control::Notify(Message & message)
{
	auto state = RequireState(message.m_receiver);
	if (state.second && (state.first != EntityState::Dash && state.first != EntityState::Uppercut))
		return;

	auto entityMgr = m_systemMgr->GetEntityManager();
	auto player	= m_systemMgr->GetPlayerID();

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	switch (message.m_type)
	{
	case EntityMessage::Player_Moving:
	{
		Message msg(EntityMessage::Moving, message.m_receiver);
		msg.m_direction = message.m_direction;
		handler->Dispatch(msg);
		break;
	}

	case EntityMessage::Player_Jumping:
	{
		Message msg(EntityMessage::Jumping, message.m_receiver);
		if (state.second)
		{
			if (state.first != EntityState::Dash) return;
		}
		msg.is_action = true;
		handler->Dispatch(msg);
		break;
	}

	case EntityMessage::Player_Idle:
	{
		Message msg(EntityMessage::Became_Idle, message.m_receiver);
		handler->Dispatch(msg);
		break;
	}
	case EntityMessage::Player_Dash:
	{
		if (auto entity = entityMgr->GetC_Controllable(player)->IsPlayerCloseToDoor(); entity >= 0)
		{
			if (auto souls = entityMgr->GetC_Status(m_playerID)->GetSouls(); souls.x > 0 && (souls.x + souls.y) > 1)
			{
				Message msg(EntityMessage::Pay_Purple_Soul, entity);
				handler->Dispatch(msg);
			}
			break;
		}

		Message msg(EntityMessage::Dash, message.m_receiver);
        msg.m_direction = message.m_direction;

        Message direction_msg(EntityMessage::Direction_Changed, message.m_receiver);
        direction_msg.m_direction = message.m_direction;

		if (state.first == EntityState::Dash)
		{
			C_Status * player_status = entityMgr->GetC_Status(m_playerID);
			if (!player_status->IsDoubleDashUsed())
			{
				player_status->SetDoubleDashUsed(true);
				msg.is_action = true; // double dash
				handler->Dispatch(msg);
                handler->Dispatch(direction_msg);
			}
			break;
		}

		handler->Dispatch(msg);
        handler->Dispatch(direction_msg);
		break;
	}
	case EntityMessage::Player_Uppercut:
	{
		Message msg(EntityMessage::Uppercut, message.m_receiver);

		C_Status * player_status = entityMgr->GetC_Status(m_playerID);
		if (IsEntityOnAir(message.m_receiver))
		{
			if (player_status->IsAirUppercutUsed()) break;
			player_status->SetAirUppercutUsed(true);
            player_status->SetDoubleDashUsed(false);
			msg.is_action = true; // air uppercut
		}

		handler->Dispatch(msg);
		break;
	}
	case EntityMessage::Player_Stomp:
	{
		if (auto entity = entityMgr->GetC_Controllable(player)->IsPlayerCloseToDoor(); entity >= 0)
		{
			if (auto souls = entityMgr->GetC_Status(m_playerID)->GetSouls(); souls.y > 0 && (souls.x + souls.y) > 1)
			{
				Message msg(EntityMessage::Pay_Blue_Soul, entity);
				handler->Dispatch(msg);
			}
			
			break;
		}

		if (IsCollidingFragileWall(message.m_receiver))
		{
			break;
		}

		if (IsEntityOnAir(message.m_receiver))
		{
			if(C_State * player_state = entityMgr->GetC_State(message.m_receiver))
                player_state->ResetDelay();
			Message msg(EntityMessage::Air_Stomp, m_playerID);
			handler->Dispatch(msg);
			break;
		}

		Message msg(EntityMessage::Stomp, message.m_receiver);
		handler->Dispatch(msg);
		break;
	}
	}
}
