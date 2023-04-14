/******************************************************************************/
/*!
\file   S_Attackable.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "SystemManager.h"
#include "EntityManager.h"
#include "S_Attackable.h"
#include "C_Collidable.h"
#include "C_Attackable.h"
#include "C_Status.h"
#include "C_Audio.h"
#include "C_Position.h"
#include "C_GameObject.h"
#include "C_State.h"
#include "C_AI.h"
#include "C_Sprite.h"
#include "C_SpriteSheet.h"
#include "StateManager.h"

#include "Mathematics.h"


namespace
{
    constexpr float PLAYER_INVINCIBLE = 2.f;
}


// Ctor
S_Attackable::S_Attackable(SystemManager * sysMgr)
	: S_Base(System::Attackable, sysMgr), m_stateMgr(nullptr)
{
	Bitset bits;
	bits.set((size_t)Component::Status, 1);
	bits.set((size_t)Component::Collidable, 1);
	m_requirement.push_back(bits); // For Box

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::Attack, this);
	handler->Subscribe(EntityMessage::GetAttacked, this);
    handler->Subscribe(EntityMessage::Dash, this);
    handler->Subscribe(EntityMessage::Uppercut, this);
    handler->Subscribe(EntityMessage::Air_Stomp_End, this);

    handler->Subscribe(EntityMessage::Player_Stomp, this);
    handler->Subscribe(EntityMessage::Player_Dash, this);
    handler->Subscribe(EntityMessage::Player_Uppercut, this);
}

// C_Base method
void S_Attackable::Update(Window * window, float dt, bool editor_mode)
{
    if (editor_mode) return;
	EntityManager * entityMgr	= m_systemMgr->GetEntityManager();
	C_Attackable * player_attack = entityMgr->GetC_Attackable(m_playerID);
    FloatRect view_space = window->GetBaseWindow()->GetUpdateCullingSpace();

    C_Status * player_status = entityMgr->GetC_Status(m_playerID);

    if (player_status->IsInvincible())
    {
        player_status->UpdateInvincibleTimer(dt);
        if (player_status->GetInvincibleTimer() > PLAYER_INVINCIBLE)
            player_status->ToggleInvincible();
    }

	// Update position
    // Damage enemies
	for (auto & itr : m_entities)
	{
		C_Collidable * col = entityMgr->GetC_Collidable(itr);

        //Reset the damage when it falsl to the ground
        C_Status * entity_status = entityMgr->GetC_Status(itr);
        if (!IsEntityOnAir(itr))
            entity_status->SetDamage(1);

		C_Attackable * attack = entityMgr->GetC_Attackable(itr);
		if (attack)
		{
			// Attack range position update
			attack->SetTopleft(
				col->GetAABB().m_top,
				col->GetAABB().m_left + 
					col->GetAABB().m_width / 2 -
					attack->GetAttackRange().m_width / 2
			);
		}

        std::string entity_type = entityMgr->GetEntityType(itr);
		if (entity_type != "Entity_Statue" && 
            entity_type != "Entity_TreasureBox" &&  
            entity_type != "Entity_PlateTower" &&
            entity_type != "Entity_Book" &&
			entity_type != "Entity_FragileWall" &&
			entity_type != "Entity_Defense" &&
			entity_type != "Entity_Steam" &&
			entity_type != "Entity_Fork" &&
			entity_type != "Entity_Boss") continue;
        
        EntityState player_state = RequireState(m_playerID).first;
        EntityState enemy_state = RequireState(itr).first;

		if (player_state == EntityState::Dead || player_state == EntityState::Hurt) continue;

		if (entity_type == "Entity_Steam")
		{
			const auto direction = entityMgr->GetC_State(itr)->GetDirection();
			const auto attackRange = attack->GetAttackRange();

			if (direction == Direction::Right)
				attack->SetTopleft(
					col->GetAABB().m_top,
					col->GetAABB().m_left + col->GetAABB().m_width - attackRange.m_width / 2.f);
			else
				attack->SetTopleft(
					col->GetAABB().m_top,
					col->GetAABB().m_left - attackRange.m_width / 2.f);

			if (attack->GetAttackRange().Intersects(entityMgr->GetC_Collidable(m_playerID)->GetAABB()))
			{
                if (player_status->IsInvincible()) continue;
				Message msg(EntityMessage::GetAttacked, m_playerID);
				msg.int_value = itr;
				m_systemMgr->GetMessageHandler()->Dispatch(msg);
				
				msg.m_type = EntityMessage::SucceedAttack;
				msg.m_receiver = itr;
				m_systemMgr->GetMessageHandler()->Dispatch(msg);

                player_status->ToggleInvincible();
				continue;
			}
		}

		if ((player_state == EntityState::Air_Stomp || 
            player_state == EntityState::Stomp ||
            player_state == EntityState::Uppercut) &&
			enemy_state != EntityState::Dead &&
			player_attack->GetAttackRange().Intersects(col->GetAABB()))
		{
			if (player_state == EntityState::Uppercut &&
				enemy_state == EntityState::Knockdown) continue;

			auto ai = entityMgr->GetC_AI(itr);

			if (ai && ai->GetType() == EnemyType::Boss)
			{
				if (enemy_state == EntityState::Air_Stomp) continue;

				if (player_state == EntityState::Uppercut && enemy_state == EntityState::Uppercut) continue;
			}

			if (player_state == EntityState::Stomp)
			{
				C_Collidable * player_col =
					entityMgr->GetC_Collidable(m_playerID);
				Message msg(EntityMessage::Pushed, itr);
				msg.int_value = m_playerID;
				if (player_col->GetAABB().m_left < col->GetAABB().m_left)
					msg.m_direction = Direction::Right;
				else
					msg.m_direction = Direction::Left;
				msg.int_value = m_playerID;
				m_systemMgr->GetMessageHandler()->Dispatch(msg);
				continue;
			}

			if (player_state == EntityState::Uppercut || player_state == EntityState::Air_Stomp)
			{
				if (ai && ai->GetType() == EnemyType::Defense)
				{
					if (enemy_state == EntityState::Dash) continue;

					const auto entityAABB = col->GetAABB();
					const auto playerAABB = entityMgr->GetC_Collidable(m_playerID)->GetAABB();
					FloatRect intersection;

					if (playerAABB.Intersects(entityAABB, intersection))
					{
						if (intersection.m_width < entityAABB.m_width / 2.f)
						{ 
							ShieldBlockMessage(itr, m_playerID);
							continue;
						}
					}
					else
					{
						continue;
					}
				}
			}

            if (entity_status->IsAlreadyAttacked())
                continue;

			Message msg(EntityMessage::GetAttacked, itr);
			msg.int_value = m_playerID;
			if (player_state == EntityState::Uppercut) msg.m_direction = Direction::Up;
			else if (player_state == EntityState::Air_Stomp) msg.m_direction = Direction::Down;

			m_systemMgr->GetMessageHandler()->Dispatch(msg);
		}


	}
}

// Observer method
void S_Attackable::Notify(Message & message)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	C_Attackable * attack	 = entityMgr->GetC_Attackable(message.m_receiver);
	auto entity = message.m_receiver;
	C_Status * status		  = entityMgr->GetC_Status(entity);

	switch (message.m_type)
	{
	case EntityMessage::Attack:
	{
		for (auto & target : m_entities)
		{
			if (static_cast<unsigned int>(entity) == target) continue;

			C_Collidable * target_body = entityMgr->GetC_Collidable(target);
			if (attack->GetAttackRange().Intersects(target_body->GetAABB()))
			{
				// Knock-back effect on target
				SendGetAttackedMessage(entity, target);

				// Target health decreased
				Damage(entity, target);
			}
		}
		break;
	}
	case EntityMessage::GetAttacked:
	{
		if (auto gameObject = entityMgr->GetC_GameObject(entity);
			gameObject && gameObject->GetType() == ObjectType::FragileWall) break;
		Damage(message.int_value, entity);
        status->ResetHRTimer();
		break;
	}
    case EntityMessage::Dash:
    {
        if (message.is_action) // Double Dash
            status->SetDamage(status->GetDamage() + 2);
        break;
    }
    case EntityMessage::Uppercut:
    {
        if (message.is_action) // Air Uppercut
            status->SetDamage(status->GetDamage() + 3);
        break;
    }
    case EntityMessage::Air_Stomp_End:
    {
        if (message.is_action) // High Air Stomp
            status->SetDamage(status->GetDamage() * 2);
        break;
    }
    case EntityMessage::Player_Dash:
    case EntityMessage::Player_Stomp:
    case EntityMessage::Player_Uppercut:
    {
        for (auto & entity_ : m_entities)
        {
            if (entity_ == m_playerID) continue;
            C_Status * entity_status = entityMgr->GetC_Status(entity_);
            entity_status->SetAttacked(false);
        }
        break;
    }
	}
}

// S_Attackable method
void S_Attackable::Damage(const EntityId & attacker, const EntityId target)
{
	EntityManager * entityMgr  = m_systemMgr->GetEntityManager();
	C_Status * attacker_status = entityMgr->GetC_Status(attacker);
	C_Status * target_status   = entityMgr->GetC_Status(target);

    target_status->SetAttacked(true);

	// Target health decreased
	if (!target_status->IsInvincible())
	{
		if (target == m_playerID)
			target_status->TakeAwaySoul();
        else
        {
            int health = target_status->GetHealth() -
                (attacker_status ? attacker_status->GetDamage() : 1);
            target_status->SetHealth(health >= 0 ? health : 0);
        }
			
	}

	// Remove target entity
	// Restart level when player dies
	if (target == m_playerID && !target_status->GetSouls().x && !target_status->GetSouls().y)
	{
		Message msg(EntityMessage::Dead, target);
		m_systemMgr->GetMessageHandler()->Dispatch(msg);
		return;
	}

	if (target_status->GetHealth() <= 0)
	{
        if (entityMgr->HasComponent(target, Component::State)
            && RequireState(target).first != EntityState::Dead)
        {
            if (target_status->GetRespawnTime() != 0.f)
            {
                entityMgr->ReserveEntity(entityMgr->GetEntityType(target),
                    entityMgr->GetC_Position(target)->GetInitialPosition(),
                    target_status->GetRespawnTime());
            }
            GenerateSoul(target);
        }
        else
            entityMgr->RemoveEntity(target);
	}
}

void S_Attackable::SendGetAttackedMessage(int attacker, int target)
{
    EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	if (target == static_cast<int>(m_playerID))
    {
        C_Status * status = entityMgr->GetC_Status(m_playerID);
        if (status->IsInvincible()) return;
    }

    if (C_Status * status = entityMgr->GetC_Status(target); status && status->IsAlreadyAttacked())
        return;

	Message m(EntityMessage::GetAttacked, target);
	m.int_value = attacker;

    C_Collidable * attacker_body = entityMgr->GetC_Collidable(attacker);
    C_Collidable * target_body = entityMgr->GetC_Collidable(target);

	if (attacker_body->GetAABB().m_left < target_body->GetAABB().m_left)
		m.m_direction = Direction::Right;
	else
		m.m_direction = Direction::Left;

	m_systemMgr->GetMessageHandler()->Dispatch(m);
}

void S_Attackable::GenerateSoul(const EntityId & dead_enemy)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	std::string entity_type   = entityMgr->GetEntityType(dead_enemy);
	if (entity_type != "Entity_TreasureBox")
	{
		if (Mathematics::GetRandomInt(0, 1))
			entityMgr->ReserveEntity("Object_Soul_purple", entityMgr->GetC_Position(dead_enemy)->GetPosition());
	}
	else
		entityMgr->ReserveEntity("Object_Soul_blue", entityMgr->GetC_Position(dead_enemy)->GetPosition());

    if (!entityMgr->HasComponent(dead_enemy, Component::State) || 
		!HasStateAnimation(dead_enemy, EntityState::Dead))
    {
        entityMgr->RemoveEntity(dead_enemy);
        return;
    }
    Message msg(EntityMessage::Dead, dead_enemy);
    m_systemMgr->GetMessageHandler()->Dispatch(msg);
}

bool S_Attackable::HasStateAnimation(const EntityId & entity, EntityState state)
{
    Message msg(EntityMessage::Has_State_Animation, entity);
    msg.m_state = state;
    m_systemMgr->GetMessageHandler()->Dispatch(msg);
    return msg.is_action;
}
