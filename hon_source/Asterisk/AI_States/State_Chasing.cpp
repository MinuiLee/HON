/******************************************************************************/
/*!
\file   State_Chasing.cpp
\author Kim Hyungseob
\par    email: hn02415 \@ gmail.com
\date   2019/01/24

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <cmath>

#include "State_Chasing.h"

#include "StateMachine.h"
#include "State_Wandering.h"

#include "..\SystemManager.h"
#include "..\EntityManager.h"

#include "..\ECS_Types.h"
#include "..\C_AI.h"
#include "..\C_Position.h"
#include "..\C_Collidable.h"
#include "..\C_RigidBody.h"

#include "..\S_Collision.h"
#include "..\Map.h"

#include "..\Mathematics.h"

#define OFFSET 5
#define MOVING_TIME_MIN 2
#define MOVING_TIME_MAX 5

State_Chasing * State_Chasing::GetInstance()
{
    static State_Chasing instance;

    return &instance;
}

void State_Chasing::Enter(SystemManager * systemMgr, EntityId entity)
{
    auto entityMgr = systemMgr->GetEntityManager();
    auto AI_component = entityMgr->GetComponent<C_AI>(entity, Component::AI);

    AI_component->SetCondition(0.f);
    AI_component->SetTimer(0.f);
}

void State_Chasing::Update(SystemManager * systemMgr, EntityId entity, float)
{
    auto entityMgr = systemMgr->GetEntityManager();
    const EntityId target = systemMgr->GetPlayerID();

    auto entityPos = entityMgr->GetC_Position(entity)->GetPosition();
    auto targetPos = entityMgr->GetC_Position(target)->GetPosition();

    auto AI_component = entityMgr->GetC_AI(entity);

    //----------------------- 1. Check distance is enough to change state -------------------------
    const float xdiff = targetPos.x - entityPos.x;
    const float ydiff = targetPos.y - entityPos.y;
    const float distance = sqrt(xdiff * xdiff + ydiff * ydiff);

    if (distance > AI_component->GetChaseRange() * 3)
    {
        AI_component->GetStateMachine()->ChangeState(systemMgr, entity, State_Wandering::GetInstance());
        return;
    }

    //----------------------- 2. Check entity is outside the block --------------------------------
	Vector2i direction = AI_component->GetDirection();

	auto entityCol = entityMgr->GetComponent<C_Collidable>(entity, Component::Collidable);
	auto map = systemMgr->GetSystem<S_Collision>(System::Collision)->GetMap();
	auto box = entityCol->GetAABB();

	const int tile_left = static_cast<int>(floor((entityPos.x - box.m_width / 2) / static_cast<float>(Tile_Size)));
	const int tile_right = static_cast<int>(floor((entityPos.x + box.m_width / 2) / static_cast<float>(Tile_Size)));
	const int tile_x = static_cast<int>(floor(entityPos.x / static_cast<float>(Tile_Size)));
	const int tile_y = static_cast<int>(floor((entityPos.y - box.m_height / 2.f) / static_cast<float>(Tile_Size)));

	auto floor_left = map->GetTile(tile_left, tile_y - 1);
	auto floor_right = map->GetTile(tile_right, tile_y - 1);
	auto floor_center = map->GetTile(tile_x, tile_y - 1);

	auto IsTherePlatform = [](Tile* tile) {
		return tile && !tile->tile->m_isPassable;
	};

	auto AssignNewDirection = [&](int leftORright) {
		entityMgr->GetC_Rigidbody(entity)->SetVelocity(Vector3f{ 0.f, 0.f, 0.f });

		direction.x = leftORright;

		AI_component->SetDirection(direction);
		AI_component->SetCondition(Mathematics::GetRandomInt(MOVING_TIME_MIN, MOVING_TIME_MAX) / 5.f);
		AI_component->SetTimer(0.f);
	};

	bool isFalling = false;

	if (IsTherePlatform(floor_left) || IsTherePlatform(floor_right) || IsTherePlatform(floor_center))
	{
		if ((!IsTherePlatform(floor_left) && IsTherePlatform(floor_right)) || IsTherePlatform(map->GetTile(tile_left, tile_y))) // on left boundary
			AssignNewDirection(1);
		else if ((!IsTherePlatform(floor_right) && IsTherePlatform(floor_left)) || IsTherePlatform(map->GetTile(tile_right, tile_y))) // on right boundary
			AssignNewDirection(-1);
	}
	else isFalling = true;

    //----------------------- 3. Check time is passed enough to change direction ------------------
    if (AI_component->GetCondition() <= AI_component->GetTimer())
    {
        direction.x = static_cast<int>(targetPos.x - entityPos.x);

        AI_component->SetDirection(direction);
        AI_component->SetCondition(Mathematics::GetRandomInt(MOVING_TIME_MIN, MOVING_TIME_MAX)/5.f);
        AI_component->SetTimer(0.f);
    }

    //----------------------- 4. Make entity move -------------------------------------------------

    Message msg(EntityMessage::Moving, entity);
    if (direction.x > 0)
    {
        msg.m_direction = Direction::Right;
        systemMgr->GetMessageHandler()->Dispatch(msg);
    }
    else if (direction.x < 0)
    {
        msg.m_direction = Direction::Left;
        systemMgr->GetMessageHandler()->Dispatch(msg);
    }
}