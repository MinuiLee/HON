/******************************************************************************/
/*!
\file   S_SpriteSheet.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_SpriteSheet.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_SpriteSheet.h"
#include "C_State.h"
#include "C_Shader.h"

// Ctor
S_SpriteSheet::S_SpriteSheet(SystemManager * systemMgr)
	: S_Base(System::SheetAnimation, systemMgr), m_shaders(nullptr)
{
	Bitset bits;
	bits.set((size_t)Component::State, 1);
	bits.set((size_t)Component::SpriteSheet, 1);
	bits.set((size_t)Component::Shader, 1);
	m_requirement.push_back(bits);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::Direction_Changed, this);
	handler->Subscribe(EntityMessage::Request_Shader, this);
	// Main(Body) animation update
	handler->Subscribe(EntityMessage::State_Changed, this);
	// Additional animation update
	handler->Subscribe(EntityMessage::Dash, this);
	handler->Subscribe(EntityMessage::Uppercut, this);
	handler->Subscribe(EntityMessage::GetAttacked, this);
	handler->Subscribe(EntityMessage::Jumping_Start, this);
	handler->Subscribe(EntityMessage::Stomp, this);
	handler->Subscribe(EntityMessage::Air_Stomp_Start, this);
	handler->Subscribe(EntityMessage::Air_Stomp_End, this);
	// Reponding information
	handler->Subscribe(EntityMessage::IsDeadAnimationEnd, this);
	handler->Subscribe(EntityMessage::Has_State_Animation, this);
	handler->Subscribe(EntityMessage::GetFlower, this);
	handler->Subscribe(EntityMessage::ReleaseFlower, this);
	handler->Subscribe(EntityMessage::ShieldBlocking, this);
}

void S_SpriteSheet::Initialize(ShaderContainer & input_)
{
	RequestShader(input_);
}

// Observer method
void S_SpriteSheet::Notify(Message & message)
{
	EntityManager * entityMgr  = m_systemMgr->GetEntityManager();
	C_SpriteSheet * sheet	  = entityMgr->GetC_SpriteSheet(message.m_receiver);
	C_State * state			   = entityMgr->GetC_State(message.m_receiver);
	SpriteSheet * sprite_sheet = sheet->GetSpriteSheet();

	switch (message.m_type)
	{
	case EntityMessage::Direction_Changed:
	{
		if (sprite_sheet->GetDirection() != message.m_direction)
			sprite_sheet->SetDirection(message.m_direction);
		break;
	}
	case EntityMessage::State_Changed:
	{
		if (state->GetOldState() != state->GetState())
		{
			bool loop				  = true;
			std::string current_state = state->GetStateString();
			if (current_state == "Dead" || current_state == "Knockdown")
			{ 
				current_state = "Dead";
				loop = false;
				if (state->GetOldState() == EntityState::Air_Stomp)
					sprite_sheet->SetCurrentAnimation("ShockWaveDown", "Disappear", true, false);
				sprite_sheet->SetCurrentAnimation("HitEffect", "Hit", true, false);
			}

			if (current_state == "Stomp") current_state = "Air_Stomp";
			sprite_sheet->SetCurrentAnimation("Body", current_state, true, loop);

			if (current_state == "Hurt")
				sprite_sheet->SetCurrentAnimation("Star", "Star", true, true);
			else if (current_state == "Idle")
				sprite_sheet->SetCurrentAnimation("Star", "Star", false, false);
		}

		break;
	}
	case EntityMessage::Dash:
	{
		if (state->GetOldState() != state->GetState() || message.is_action)
		{
			sprite_sheet->SetCurrentAnimation("Body", "Walk", true, true);
			if (state->GetDirection() == Direction::Left)
			{
				sprite_sheet->SetCurrentAnimation("SteamLeft", "Boom", true, false);
				if (message.is_action)
					sprite_sheet->SetCurrentAnimation("ShockWaveLeft", "Disappear", true, false);
			}

			else if (state->GetDirection() == Direction::Right)
			{
				sprite_sheet->SetCurrentAnimation("SteamRight", "Boom", true, false);
				if (message.is_action)
					sprite_sheet->SetCurrentAnimation("ShockWaveRight", "Disappear", true, false);
			}
		}

		break;
	}
	case EntityMessage::Uppercut:
	{
		sprite_sheet->SetCurrentAnimation("SteamUp", "Boom", true, false);
		if (message.is_action)
			sprite_sheet->SetCurrentAnimation("ShockWaveUp", "Disappear", true, false);

		break;
	}
	case EntityMessage::GetAttacked:
	{
        EntityState entity_state = state->GetState();
		if (entity_state != EntityState::Dead)
			sprite_sheet->SetCurrentAnimation("HitEffect", "Hit", true, false);

        if(entity_state == EntityState::Hurt && 
            state->GetOldState() == EntityState::Air_Stomp) //Probably attacked by Steam enemy
            sprite_sheet->SetCurrentAnimation("ShockWaveDown", "Disappear", true, false);
		break;
	}
	case EntityMessage::Request_Shader:
	{
		if (C_Shader * shader = entityMgr->GetC_Shader(message.m_receiver))
		{
			auto shaderName = shader->GetName();
			sheet->SetShader(*m_shaders, shaderName);
		}
		else
			assert(!"Unable to find shader component.");
		break;
	}
	case EntityMessage::IsDeadAnimationEnd:
	{
		if (state->GetState() != EntityState::Dead && state->GetState() != EntityState::Knockdown)
		{
			message.is_action = false;
			break;
		}

		message.is_action = sheet->GetSpriteSheet()->IsDeadAnimationEnd(message.m_receiver);
		break;
	}
	case EntityMessage::Air_Stomp_Start:
	{
		if (!message.is_action) sprite_sheet->SetSpriteScale("SteamDown", 2.f);
		sprite_sheet->SetCurrentAnimation("SteamDown", "Boom", true, false);
		sprite_sheet->SetCurrentAnimation("ShockWaveDown", "Boom", true, true);
		break;
	}
	case EntityMessage::Air_Stomp_End:
	{
		if (message.is_action)
		{
			sprite_sheet->SetSpriteScale("SteamDown", 5.f);
			sprite_sheet->SetCurrentAnimation("SteamDown", "Boom", true, false);
		}
		else
			sprite_sheet->SetSpriteScale("SteamDown", 2.f);
		sprite_sheet->SetCurrentAnimation("ShockWaveDown", "Boom", false, false);
		sprite_sheet->SetCurrentAnimation("ShockWaveDown", "Disappear", true, false);
		break;
	}
	case EntityMessage::Jumping_Start:
	{
		sprite_sheet->SetCurrentAnimation("SteamUp", "Boom", true, false);
		break;
	}
	case EntityMessage::Stomp:
	{
		sprite_sheet->SetCurrentAnimation("SteamRight", "Stomp", true, false);
		sprite_sheet->SetCurrentAnimation("SteamLeft", "Stomp", true, false);
		break;
	}
	case EntityMessage::Has_State_Animation:
	{
		message.is_action = sprite_sheet->HasState("Dead");
		break;
	}
	case EntityMessage::GetFlower:
	{
		if (message.int_value == 1) sprite_sheet->SetCurrentAnimation("Flower", "Red", true, true);
		if (message.int_value == 2) sprite_sheet->SetCurrentAnimation("Flower", "Blue", true, true);
		if (message.int_value == 3)
			sprite_sheet->SetCurrentAnimation("Flower", "Yellow", true, true);
		if (message.int_value == 4)
			sprite_sheet->SetCurrentAnimation("Flower", "White", true, true);
		break;
	}
	case EntityMessage::ReleaseFlower:
	{
		if (message.int_value)
		{
			switch (message.int_value)
			{
			case 1:
				sprite_sheet->SetCurrentAnimation("Body", "Red", true, true);
				break;
			case 2:
				sprite_sheet->SetCurrentAnimation("Body", "Blue", true, true);
				break;
			case 3:
				sprite_sheet->SetCurrentAnimation("Body", "Yellow", true, true);
				break;
			case 4:
				sprite_sheet->SetCurrentAnimation("Body", "White", true, true);
				break;
			}
		}
		else if(message.int_value == 0)
		{
			sprite_sheet->SetCurrentAnimation("Flower", "Red", false, false);
			break;
		}
	}
	case EntityMessage::ShieldBlocking:
	{
		sprite_sheet->SetCurrentAnimation("HitEffect", "Block", true, false);
		break;
	}
	}
}

void S_SpriteSheet::RequestShader(ShaderContainer & input_)
{
	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	m_shaders				 = &input_;

	for (auto itr : m_entities)
	{
		Message m(EntityMessage::Request_Shader, itr);
		handler->Dispatch(m);
	}
}