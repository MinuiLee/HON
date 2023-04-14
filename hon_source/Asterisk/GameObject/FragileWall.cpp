/*!*******************************************************************
\file         FragileWall.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/03/18

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "FragileWall.h"

#include "../Observer.h"
#include "../SystemManager.h"
#include "../EntityManager.h"
#include "../AudioManager.h"

#include "../C_Sprite.h"
#include "../C_Collidable.h"
#include "../S_State.h"

void FragileWall::RotateSpriteAndCollision(EntityManager* entityMgr, EntityId entity)
{
	FloatRect& rect = entityMgr->GetC_Collidable(entity)->GetAABB();

	auto swap = [&]() {
		auto temp = rect.m_width;
		rect.m_width = rect.m_height;
		rect.m_height = temp;
	};

	switch (m_direction)
	{
	case Direction::Up:
		entityMgr->GetC_Sprite(entity)->GetSprite().SetRotation(270);
		if (rect.m_height > rect.m_width) swap();
		break;

	case Direction::Left:
		entityMgr->GetC_Sprite(entity)->GetSprite().SetRotation(0);
		if (rect.m_width > rect.m_height) swap();
		break;

	case Direction::Down:
		entityMgr->GetC_Sprite(entity)->GetSprite().SetRotation(90);
		if (rect.m_height > rect.m_width) swap();
		break;

	case Direction::Right:
		entityMgr->GetC_Sprite(entity)->GetSprite().SetRotation(180);
		if (rect.m_width > rect.m_height) swap();
		break;
	}
}

int FragileWall::GetDirectionAsInt() const
{
	switch (m_direction)
	{
	case Direction::Up: return 0;

	case Direction::Left: return 1;

	case Direction::Down: return 2;

	case Direction::Right: return 3;

    default: return -1;
	}
}

void FragileWall::SetDirection(EntityManager* entityMgr, EntityId entity, int direction)
{
	switch (direction%4)
	{
	case 0:
		m_direction = Direction::Up;
		RotateSpriteAndCollision(entityMgr, entity);
		break;

	case 1:
		m_direction = Direction::Left;
		RotateSpriteAndCollision(entityMgr, entity);
		break;

	case 2:
		m_direction = Direction::Down;
		RotateSpriteAndCollision(entityMgr, entity);
		break;

	case 3:
		m_direction = Direction::Right;
		RotateSpriteAndCollision(entityMgr, entity);
		break;
	}
}

void FragileWall::RotateWall(EntityManager* entityMgr, EntityId entity)
{
	switch (m_direction)
	{
	case Direction::Left:
	{
		m_direction = Direction::Down;
		RotateSpriteAndCollision(entityMgr, entity);
		break;
	}
	case Direction::Down:
	{
		m_direction = Direction::Right;
		RotateSpriteAndCollision(entityMgr, entity);
		break;
	}
	case Direction::Right:
		m_direction = Direction::Up;
		RotateSpriteAndCollision(entityMgr, entity);
		break;

	case Direction::Up:
		m_direction = Direction::Left;
		RotateSpriteAndCollision(entityMgr, entity);
		break;
	}
}

void FragileWall::HandleMessage(SystemManager* systemMgr, Message* message)
{
	auto entityMgr = systemMgr->GetEntityManager();
	const EntityId entity = message->m_receiver;
	const Direction direction = message->m_direction;

	switch (direction)
	{
	case Direction::Left:
		if (m_direction == Direction::Right)
		{
			entityMgr->RemoveEntity(entity);
			Message msg(EntityMessage::Wall_Break, 
				systemMgr->GetEntityManager()->GetPlayerID());
			systemMgr->GetMessageHandler()->Dispatch(msg);
		}
		break;

	case Direction::Right:
		if (m_direction == Direction::Left) 
		{
			entityMgr->RemoveEntity(entity);
			Message msg(EntityMessage::Wall_Break, 
				systemMgr->GetEntityManager()->GetPlayerID());
			systemMgr->GetMessageHandler()->Dispatch(msg);
		}
		break;

	case Direction::Up:
		if (m_direction == Direction::Down)
		{
			entityMgr->RemoveEntity(entity);
			Message msg(EntityMessage::Wall_Break, 
				systemMgr->GetEntityManager()->GetPlayerID());
			systemMgr->GetMessageHandler()->Dispatch(msg);
		}
		break;

	case Direction::Down:
		if (m_direction == Direction::Up) 
		{
			entityMgr->RemoveEntity(entity);
			Message msg(EntityMessage::Wall_Break, 
				systemMgr->GetEntityManager()->GetPlayerID());
			systemMgr->GetMessageHandler()->Dispatch(msg);
		}
		break;
	}
}