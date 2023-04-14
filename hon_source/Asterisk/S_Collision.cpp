/******************************************************************************/
/*!
\file   S_Collision.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>

#include "S_Collision.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_Position.h"
#include "C_Collidable.h"
#include "C_Status.h"
#include "C_RigidBody.h" // Reference tile setting
#include "C_State.h"
#include "C_AI.h"
#include "C_Sprite.h"
#include "C_SpriteSheet.h"
#include "Map.h"
#include "Tile.h"

// Ctor
S_Collision::S_Collision(SystemManager * systemMgr)
	: S_Base(System::Collision, systemMgr), m_map(nullptr)
{
	Bitset bits;
	bits.set((size_t)Component::Collidable, 1);
	bits.set((size_t)Component::Position, 1);
	m_requirement.push_back(bits);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::Colliding, this);
	handler->Subscribe(EntityMessage::IsCollidingFragileWall, this);
}

// S_Base methods
void S_Collision::Update(Window * /*window*/, float /*dt*/, bool EditorMode)
{
	if (!m_map) return;

	EntityManager * entityMgr = m_systemMgr->GetEntityManager();

	for (unsigned int entity : m_entities)
	{
		C_Position * tran = entityMgr->GetC_Position(entity);
		C_Collidable * col = entityMgr->GetC_Collidable(entity);

		CollidingTileUpdate(entity);
		col->SetPosition(tran->GetPosition());
        if (EditorMode) continue;

		if (entity == m_playerID)
			EntityCollision(entity);
		else if (entityMgr->GetEntityType(entity) == "Object_Plate" && col->IsResponsive())
			PlateCollision(entity);

		TileCollision(entity);
		TileCollisionRayCasting(col, tran, entity);
	}
}

void S_Collision::Notify(Message & message)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	switch (message.m_type)
	{
	case EntityMessage::Colliding:
	{
		std::string entity_type = entityMgr->GetEntityType(message.m_receiver);
		if (entity_type == "Object_Soul_purple" || entity_type == "Object_Soul_blue")
		{
			if (message.m_axis == Message::Axis::X)
			{
				C_Collidable * col = entityMgr->GetC_Collidable(message.m_receiver);
				col->SetGhost(false);
			}
		}
		else if (entity_type == "Object_Plate" &&
			RequireState(message.m_receiver).first != EntityState::Dead)
		{
			Message msg_dead(EntityMessage::Dead, message.m_receiver);
			m_systemMgr->GetMessageHandler()->Dispatch(msg_dead);
		}
		break;
	}
	case EntityMessage::IsCollidingFragileWall:
	{
		C_Collidable * receiver_col = entityMgr->GetC_Collidable(message.m_receiver);
		FloatRect & receiver_AABB   = receiver_col->GetAABB();
		for (auto & entity : m_entities)
		{
			if (entityMgr->GetEntityType(entity) == "Entity_FragileWall")
			{
				C_Collidable * entity_col = entityMgr->GetC_Collidable(entity);
				FloatRect & entity_AABB   = entity_col->GetAABB();

				const Vector2f wall_center = entity_col->GetCenter();
				const Vector2f receiver_center = receiver_col->GetCenter();
				float distance = static_cast<float>((receiver_center - wall_center).length());
				distance *= distance;

				if (receiver_AABB.Intersects(entity_AABB) )
				{
					message.is_action = true;
					break;
				}
			}
		}
		break;
	}
	}
}

void S_Collision::TileCollision(const EntityId & entity) const
{
	EntityManager * entityMgr		 = m_systemMgr->GetEntityManager();
	C_Collidable * col				 = entityMgr->GetC_Collidable(entity);
	const FloatRect & entity_AABB	= col->GetAABB();
	CollidingTiles & colliding_tiles = col->GetCollidingTiles();

	int x_start = static_cast<int>(floor(entity_AABB.m_left / Tile_Size));
	int x_end   = static_cast<int>(floor((entity_AABB.m_left + entity_AABB.m_width) / Tile_Size));
	int y_start = static_cast<int>(floor((entity_AABB.m_top - entity_AABB.m_height) / Tile_Size));
	int y_end   = static_cast<int>(floor(entity_AABB.m_top / Tile_Size));

	CollisionInfo collision;
	collision.entity = entity;

	col->SetIsCollidingWithTile(false);

	for (int x = x_start; x <= x_end; ++x)
	{
		for (int y = y_start; y <= y_end; ++y)
		{
			auto tile = colliding_tiles.find(std::make_pair(x, y));
			if (tile != colliding_tiles.end()) continue;

			Tile * current_tile = m_map->GetTile(x, y);
			if (!current_tile) continue;

			FloatRect tile_AABB(static_cast<float>(x * Tile_Size),
				static_cast<float>((y + 1) * Tile_Size), Tile_Size, Tile_Size);

			FloatRect intersection;
			if (entity_AABB.Intersects(tile_AABB, intersection))
			{
				if (intersection.m_width < 1.f && intersection.m_height < 1.f) continue;

				if (current_tile->tile->is_deadly)
				{
					C_RigidBody * entity_rigid = entityMgr->GetC_Rigidbody(entity);
					entity_rigid->SetVelocity(Vector3f(0, 0, 0));
					Vector2i last_tile_pos = entity_rigid->GetLastRefTileCoord();

					C_Position * entity_pos = entityMgr->GetC_Position(entity);
					entity_pos->SetPosition(Vector3f(last_tile_pos.x * Tile_Size + Tile_Size / 2,
						(last_tile_pos.y + 3) * Tile_Size, 0.2f));
				}

				/////////////////////////////////////////////////////////////////////////////////
				// DIRECTIONAL COLLISION DETECTION
				bool collision_x = false, collision_y = false;

				if (entity_AABB.m_left < tile_AABB.m_left &&
					current_tile->tile->m_collision_x == Direction::Left)
				{
					collision_x = true;
					if (intersection.m_width > Tile_Size / 2) collision_x = false;
				}

				else if (entity_AABB.m_left > tile_AABB.m_left &&
					current_tile->tile->m_collision_x == Direction::Right)
				{
					collision_x = true;
					if (intersection.m_width > Tile_Size / 2) collision_x = false;
				}

				if (entity_AABB.m_top < tile_AABB.m_top &&
					current_tile->tile->m_collision_y == Direction::Down)
				{
					collision_y = true;
					if (intersection.m_height > Tile_Size / 2) collision_y = false;
				}
				else if (entity_AABB.m_top > tile_AABB.m_top &&
					current_tile->tile->m_collision_y == Direction::Up)
				{
					collision_y = true;
					if (intersection.m_height > Tile_Size / 2) collision_y = false;
					if (C_RigidBody * entity_rigid = entityMgr->GetC_Rigidbody(entity))
					{
						if (entity_rigid->GetVelocity().y > 0) collision_y = false;
					}
				}

				if (!collision_x && !collision_y)
				{
					colliding_tiles.emplace(std::make_pair(std::make_pair(x, y), current_tile));
					continue;
				}
				//
				/////////////////////////////////////////////////////////////////////////////////

				// NORMAL COLLISION DETECTION
				// Update largest intersection
				if (collision.area < intersection.m_width * intersection.m_height)
				{
					collision.Set(intersection.m_width, intersection.m_height);
					entity_AABB.m_left < tile_AABB.m_left ? collision.dir_X = Direction::Left :
															collision.dir_X = Direction::Right;
					entity_AABB.m_top < tile_AABB.m_top ? collision.dir_Y   = Direction::Down :
														  collision.dir_Y   = Direction::Up;
					collision.currentTile									= current_tile;
					collision.tile_coord_x									= x;
					collision.tile_coord_y									= y;
					col->SetIsCollidingWithTile(true);
				}
			}
		}
	}

	if (collision.area > 0.f)
		ResolveTileCollision(collision);
	else
	{
		if (C_RigidBody * rigid_entity = entityMgr->GetC_Rigidbody(collision.entity))
		{rigid_entity->SetReferenceTile(nullptr); } }
}

void S_Collision::ResolveTileCollision(const CollisionInfo & collision) const
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	C_Position * pos		  = entityMgr->GetC_Position(collision.entity);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	if (collision.width < collision.height)
	{
		if (collision.dir_X == Direction::Left)
			pos->MoveBy(-collision.width);
		else if (collision.dir_X == Direction::Right)
			pos->MoveBy(collision.width);

		Message msg(EntityMessage::Colliding, collision.entity);
		msg.m_axis = Message::Axis::Y;
		handler->Dispatch(msg);
	}
	else
	{
		if (collision.dir_Y == Direction::Up)
			pos->MoveBy(0, collision.height);
		else if (collision.dir_Y == Direction::Down)
			pos->MoveBy(0, -collision.height);

		if (C_RigidBody * rigid = entityMgr->GetC_Rigidbody(collision.entity);
			rigid && collision.dir_Y == Direction::Up)
		{
			if (C_Status * status = entityMgr->GetC_Status(collision.entity))
				status->SetAirUppercutUsed(false);

			
			if (RequireState(collision.entity).first != EntityState::Knockdown)
				rigid->SetReferenceTile(
					collision.currentTile, collision.tile_coord_x, collision.tile_coord_y);
		}


		Message msg(EntityMessage::Colliding, collision.entity);
		msg.m_axis = Message::Axis::X;
		handler->Dispatch(msg);
	}
}

void S_Collision::CollidingTileUpdate(const EntityId & entity)
{
	C_Collidable * col	 = m_systemMgr->GetEntityManager()->GetC_Collidable(entity);
	CollidingTiles & tiles = col->GetCollidingTiles();

	for (auto tile_itr = tiles.begin(); tile_itr != tiles.end();)
	{
		FloatRect tile_AABB(static_cast<float>(tile_itr->first.first * Tile_Size),
			static_cast<float>((tile_itr->first.second + 1) * Tile_Size), Tile_Size, Tile_Size);

		if (col->GetAABB().Intersects(tile_AABB))
			tile_itr++;
		else
			tile_itr = tiles.erase(tile_itr);
	}
}

void S_Collision::EntityCollision(const EntityId & entity)
{
	EntityManager * entityMgr	 = m_systemMgr->GetEntityManager();
	C_Collidable * entity_col	 = entityMgr->GetC_Collidable(entity);
	C_Position * entity_pos		  = entityMgr->GetC_Position(entity);
	const FloatRect & player_AABB = entity_col->GetAABB();


	C_Status * player_status = entityMgr->GetC_Status(m_playerID);
	EntityState player_state = RequireState(m_playerID).first;
	for (unsigned int enemy : m_entities)
	{
		if (enemy == entity) continue;

		C_Collidable * enemy_col = entityMgr->GetC_Collidable(enemy);

		const FloatRect & enemy_AABB = enemy_col->GetAABB();
		FloatRect intersection;

		if (player_AABB.Intersects(enemy_AABB, intersection) && !enemy_col->IsGhost())
		{
			std::string entity_type = entityMgr->GetEntityType(enemy);
			if (entity_type == "Object_Soul_purple")
			{
				if (RequireState(enemy).first == EntityState::Dead) continue;
				Message msg_dead(EntityMessage::Dead, enemy);
				m_systemMgr->GetMessageHandler()->Dispatch(msg_dead);

				player_status->AddSouls(1, 0);

				Message msg_drain(EntityMessage::Drain_Soul, m_playerID);
				msg_drain.m_f2D.m_x = entity_pos->GetPosition().x;
				msg_drain.m_f2D.m_y = entity_pos->GetPosition().y;
				msg_drain.int_value = 1; // 1 == purple
				m_systemMgr->GetMessageHandler()->Dispatch(msg_drain);

				continue;
			}
			else if (entity_type == "Object_Soul_blue")
			{
				if (RequireState(enemy).first == EntityState::Dead) continue;
				Message msg_dead(EntityMessage::Dead, enemy);
				m_systemMgr->GetMessageHandler()->Dispatch(msg_dead);

				player_status->AddSouls(0, 1);

				Message msg_drain(EntityMessage::Drain_Soul, m_playerID);
				msg_drain.m_f2D.m_x = entity_pos->GetPosition().x;
				msg_drain.m_f2D.m_y = entity_pos->GetPosition().y;
				msg_drain.int_value = 0; // 0 == blue
				m_systemMgr->GetMessageHandler()->Dispatch(msg_drain);

				continue;
			}
			else if (entity_type == "Entity_Door") // Blocking player
			{
				if (player_AABB.m_left < enemy_AABB.m_left)
					entity_pos->MoveBy(-intersection.m_width);
				else
					entity_pos->MoveBy(intersection.m_width);
			}
			else if (entity_type == "Entity_FragileWall")
			{
				const auto width  = intersection.m_width;
				const auto height = intersection.m_height;

				if (width < height)
				{
					if (player_AABB.m_left < enemy_AABB.m_left)
						entity_pos->MoveBy(-width);
					else
						entity_pos->MoveBy(width);
				}
				else
				{
					if (player_AABB.m_top < enemy_AABB.m_top)
						entity_pos->MoveBy(0.f, -height);
					else
						entity_pos->MoveBy(0.f, height);
					if (C_RigidBody * rigid = entityMgr->GetC_Rigidbody(entity))
					{
						Vector3f vel = rigid->GetVelocity();
						rigid->SetVelocity(Vector3f(vel.x, 0, vel.z));
					}
					if (C_Status * status = entityMgr->GetC_Status(entity))
					{
						status->SetAirUppercutUsed(false);
					}
				}
			}
			else if (entity_type == "Object_Flower_Red" || entity_type == "Object_Flower_Blue" ||
				entity_type == "Object_Flower_Yellow" || entity_type == "Object_Flower_White")
			{
				if (!player_status->IsHoldingFlower())
				{
					int int_value = 0;
					entityMgr->RemoveEntity(enemy);
					Message msg(EntityMessage::GetFlower, m_playerID);
					if (entity_type == "Object_Flower_Red")
						int_value = 1;
					else if (entity_type == "Object_Flower_Blue")
						int_value = 2;
					else if (entity_type == "Object_Flower_Yellow")
						int_value = 3;
					else if (entity_type == "Object_Flower_White")
						int_value = 4;
					msg.int_value = int_value;
					player_status->SetFlowerHolding(msg.int_value);
					m_systemMgr->GetMessageHandler()->Dispatch(msg);

					m_map->SetFlowerSignExist(false, int_value);
					continue;
				}
			}
			else if (entity_type == "Object_Vase")
			{
				if (RequireState(enemy).first == EntityState::Other) continue;

				int flower_id = player_status->IsHoldingFlower();
				if (flower_id)
				{
					MessageHandler * handler = m_systemMgr->GetMessageHandler();

					player_status->PutFlowerToVase(flower_id);
					player_status->SetFlowerHolding(0);
					Message msg_vase(EntityMessage::ReleaseFlower, enemy);
					msg_vase.int_value = flower_id;
					handler->Dispatch(msg_vase);

					Message msg_player(EntityMessage::ReleaseFlower, m_playerID);
					msg_player.int_value = 0;
					handler->Dispatch(msg_player);

					m_map->SaveMap("current");
					continue;
				}
			}
			else if (entity_type == "Object_Spike")
			{
				if (player_state != EntityState::Knockdown)
				{
					if (player_status->IsInvincible() || player_state == EntityState::Dead)
						continue;

					player_status->TakeAwaySoul();
					Vector2i souls = player_status->GetSouls();
					if (souls.x <= 0 && souls.y <= 0)
					{
						Message dead_msg(EntityMessage::Dead, m_playerID);
						m_systemMgr->GetMessageHandler()->Dispatch(dead_msg);
						Message spike_sound_msg(EntityMessage::DeadBySpike, m_playerID);
						m_systemMgr->GetMessageHandler()->Dispatch(spike_sound_msg);
						continue;
					}

					Message msg_knockdown(EntityMessage::Knockdown, m_playerID);
					m_systemMgr->GetMessageHandler()->Dispatch(msg_knockdown);
				}
				continue;
			}

			Message msg(EntityMessage::GetAttacked, m_playerID);
			player_state = RequireState(m_playerID).first;
			if (player_state == EntityState::Dash) // Player attacks enemy
			{
				if (entity_type == "Object_Plate" && !enemy_col->IsResponsive())
					enemy_col->SetResponsive(true);

				if (entity_type == "Entity_Defense")
				{
					if (RequireState(enemy).first == EntityState::Dash)
					{
						Message push_msg(EntityMessage::Pushed, m_playerID);
						push_msg.int_value = enemy;
						m_systemMgr->GetMessageHandler()->Dispatch(push_msg);
						push_msg.m_receiver = enemy;
						push_msg.int_value  = m_playerID;
						m_systemMgr->GetMessageHandler()->Dispatch(push_msg);
					}
					ShieldBlockMessage(enemy, m_playerID);

					continue;
				}

				EntityState enemy_state = RequireState(enemy).first;
				if (enemy_state == EntityState::Dead || enemy_state == EntityState::Knockdown)
					continue; // Dash attack ignored

				auto ai = entityMgr->GetC_AI(enemy);
				if (ai && ai->GetType() == EnemyType::Boss)
				{
					if (enemy_state == EntityState::Dash || enemy_state == EntityState::Air_Stomp || enemy_state == EntityState::Uppercut || enemy_state == EntityState::Hurt || enemy_state == EntityState::Dead)
						continue;
				}

				msg.m_receiver = enemy;		 // target
				msg.int_value  = m_playerID; // attacker

				if (player_AABB.m_left < enemy_AABB.m_left)
					msg.m_direction = Direction::Right;
				else
					msg.m_direction = Direction::Left;

				if (C_Status * receiver_status = entityMgr->GetC_Status(msg.m_receiver);
					receiver_status && receiver_status->IsAlreadyAttacked())
					continue;
				m_systemMgr->GetMessageHandler()->Dispatch(msg);
			}
			else if (player_state == EntityState::Stomp)
				continue; // Nothing happen
			else if (player_state == EntityState::Uppercut ||
				player_state == EntityState::Air_Stomp)
			{
				auto ai = entityMgr->GetC_AI(enemy);

				EntityState enemy_state = RequireState(enemy).first;
				if (ai && ai->GetType() == EnemyType::Boss)
				{
					if (enemy_state == EntityState::Dash || enemy_state == EntityState::Air_Stomp || enemy_state == EntityState::Uppercut || enemy_state == EntityState::Hurt || enemy_state == EntityState::Dead)
						continue;
				}

				if (ai && ai->GetType() == EnemyType::Defense)
				{
					Message message(EntityMessage::Became_Idle, m_playerID);

					float entityWidth;
					if (auto sprite = entityMgr->GetC_Sprite(enemy); sprite)
						entityWidth = sprite->GetSpriteSize().x / 2.f;
					else
						entityWidth = entityMgr->GetC_SpriteSheet(enemy)->GetSpriteSize().x / 2.f;

					const auto playerPos = entityMgr->GetC_Position(m_playerID)->GetPosition();
					const auto entityPos = entityMgr->GetC_Position(enemy)->GetPosition();

					if (playerPos.x < entityPos.x - entityWidth ||
						playerPos.x > entityPos.x + entityWidth)
						m_systemMgr->GetMessageHandler()->Dispatch(message);
				}
			}
			else // Player get damaged
			{
				if (player_status->IsInvincible()) continue;

                std::string enemy_type = entityMgr->GetEntityType(enemy);
                //Check the object is harmful
                if (enemy_type != "Entity_Statue" &&
					enemy_type != "Object_Plate" &&
					enemy_type != "Entity_Book" &&
					enemy_type != "Entity_Defense" &&
					enemy_type != "Entity_Fork" &&
					enemy_type != "Entity_Boss") continue;

				auto playerState = RequireState(entity);
				auto enemyState = RequireState(enemy);

				if (playerState.first == EntityState::Hurt ||
					playerState.first == EntityState::Dead ||
					playerState.first == EntityState::Knockdown ||
					enemyState.first == EntityState::Hurt ||
					enemyState.first == EntityState::Dead)
					continue;

				if (enemy_type == "Entity_Defense" &&
					RequireState(enemy).first != EntityState::Dash)
					continue;

				if (player_AABB.m_left < enemy_AABB.m_left)
					msg.m_direction = Direction::Left;
				else
					msg.m_direction = Direction::Right;
				msg.int_value = enemy; // attacker
				m_systemMgr->GetMessageHandler()->Dispatch(msg);
				player_status->ToggleInvincible();

				msg.m_type = EntityMessage::SucceedAttack;
				msg.m_receiver = enemy;
				m_systemMgr->GetMessageHandler()->Dispatch(msg);

				if (enemy_type == "Object_Plate" && RequireState(enemy).first != EntityState::Dead)
				{
					Message msg_dead(EntityMessage::Dead, enemy);
					m_systemMgr->GetMessageHandler()->Dispatch(msg_dead);
				}
			}
		}
	}
}

void S_Collision::PlateCollision(const EntityId & entity)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	MessageHandler * handler  = m_systemMgr->GetMessageHandler();
	C_Collidable * plate_col  = entityMgr->GetC_Collidable(entity);

	if (C_Status * status = entityMgr->GetC_Status(entity); status && status->IsAlreadyAttacked())
		return;

	Message msg(EntityMessage::GetAttacked, 0);
	msg.int_value = entity; // attacker

	for (auto & other_entity : m_entities)
	{
		if (other_entity == entity || other_entity == m_playerID) continue;
		C_Collidable * other_col = entityMgr->GetC_Collidable(other_entity);
		if (!plate_col->GetAABB().Intersects(other_col->GetAABB())) continue;
		EntityState state = RequireState(other_entity).first;
		if (state == EntityState::Hurt || state == EntityState::Dead ||
			RequireState(entity).first == EntityState::Dead)
			continue;

		if (plate_col->GetAABB().m_left < other_col->GetAABB().m_left)
			msg.m_direction = Direction::Right;
		else
			msg.m_direction = Direction::Left;

		msg.m_receiver = other_entity;
		handler->Dispatch(msg);

		Message msg_dead(EntityMessage::Dead, entity);
		handler->Dispatch(msg_dead);
	}
}

void S_Collision::TileCollisionRayCasting(C_Collidable * col, C_Position * pos, 
    const EntityId & entity) const
{
	if (C_State * state = m_systemMgr->GetEntityManager()->GetC_State(entity))
		if (state->GetOldState() == EntityState::Knockdown) return;

	const Vector2f old_center = col->GetOldCenter();
	const Vector2f center	 = col->GetCenter();
	Vector2f ray			  = center - old_center;

	std::vector<int> scanlines_vertical;
	std::vector<int> scanlines_horizontal;

	if (ray.x > 0) // old x position < current x position
	{
		int x_start = (static_cast<int>(old_center.x / Tile_Size) + 1);
        if (old_center.x < 0) x_start--;
		int x_end   = static_cast<int>(center.x / Tile_Size);
        if (center.x < 0) x_end--;

		while (x_start <= x_end)
		{
			scanlines_vertical.push_back(x_start);
			x_start++;
		}
	}
	else if(ray.x < 0) // old x position > current x position
	{
		int x_start = static_cast<int>(old_center.x / Tile_Size);
        if (old_center.x < 0) x_start--;
		int x_end   = static_cast<int>(center.x / Tile_Size) + 1;
        if (center.x < 0) x_end--;

		while (x_start >= x_end)
		{
			scanlines_vertical.push_back(x_start);
			x_start--;
		}
	}

	if (ray.y > 0) // old y position < current y position
	{
		int y_start = static_cast<int>(old_center.y / Tile_Size + 1);
        if (old_center.y < 0) y_start--;
		int y_end   = static_cast<int>(center.y / Tile_Size);
        if (center.y < 0) y_end--;

		while (y_start <= y_end)
		{
			scanlines_horizontal.push_back(y_start);
			y_start++;
		}
	}
	else if(ray.y < 0) // old y position > current y position
	{
		int y_start = static_cast<int>(old_center.y / Tile_Size);
        if (old_center.y < 0) y_start--;
		int y_end   = static_cast<int>(center.y / Tile_Size + 1);
        if (center.y < 0) y_end--;
        
		while (y_start >= y_end)
		{
			scanlines_horizontal.push_back(y_start);
			y_start--;
		}
	}

	// Find all t (horizontal & vertical)
	std::vector<std::pair<float, Direction>> t_values; //t

	for (int vertical_line : scanlines_vertical)
	{
		float t = std::abs(vertical_line * Tile_Size - old_center.x) /
			std::abs(static_cast<float>(ray.x));
		Vector2f collision_point = old_center + t * ray;
		if (ray.x > 0)
		{ 
			// Check the tile collision direction && existence
			if (TileExist(collision_point, Direction::Right, entity))
				t_values.push_back(std::make_pair(t, Direction::Right));
		}
		else if(ray.x < 0)
		{
			// Check the tile collision direction && existence
			if (TileExist(collision_point, Direction::Left, entity))
				t_values.push_back(std::make_pair(t, Direction::Left));
		}
	}


	for (int horizontal_line : scanlines_horizontal)
	{
		float t = std::abs(horizontal_line * Tile_Size - old_center.y) /
			std::abs(static_cast<float>(ray.y));
		Vector2f collision_point = old_center + t * ray;
		if (ray.y > 0)
		{
			// Check the tile collision direction && existence
			if (TileExist(collision_point, Direction::Up, entity))
				t_values.push_back(std::make_pair(t, Direction::Up));
		}
		else if(ray.y < 0)
		{
			// Check the tile collision direction && existence
			if (TileExist(collision_point, Direction::Down, entity))
				t_values.push_back(std::make_pair(t, Direction::Down));
		}
	}

	// Find smallest t
    if (t_values.empty()) return;
	auto t_min = std::min_element(t_values.begin(), t_values.end());
	float t = t_min->first;
    Direction collision_dir = t_min->second;

    // old_position + (t * x_distance, t * y_distance)
    Vector2f updated_pos = old_center + t * ray;
    const FloatRect & entity_AABB = col->GetAABB();

	float z_pos = pos->GetPosition().z;
    if (collision_dir == Direction::Up)
        pos->SetPosition(Vector3f(updated_pos.x, updated_pos.y - entity_AABB.m_height / 2 - 1, z_pos));
    else if (collision_dir == Direction::Down)
        pos->SetPosition(Vector3f(updated_pos.x, updated_pos.y + entity_AABB.m_height / 2 + 1, z_pos));
    else if (collision_dir == Direction::Left)
        pos->SetPosition(Vector3f(updated_pos.x + entity_AABB.m_width / 2 + 1, updated_pos.y, z_pos));
    else if (collision_dir == Direction::Right)
        pos->SetPosition(Vector3f(updated_pos.x - entity_AABB.m_width / 2 - 1, updated_pos.y, z_pos));

    col->SetPosition(pos->GetPosition());

	// Send collision message
    if (collision_dir == Direction::Up || collision_dir == Direction::Down)
    {
        Message msg(EntityMessage::Colliding, entity);
        msg.m_axis = Message::Axis::X;
        m_systemMgr->GetMessageHandler()->Dispatch(msg);
    }
    else if (collision_dir == Direction::Left || collision_dir == Direction::Right)
    {
        Message msg(EntityMessage::Colliding, entity);
        msg.m_axis = Message::Axis::Y;
        m_systemMgr->GetMessageHandler()->Dispatch(msg);
    }
}

bool S_Collision::TileExist(const Vector2f & pos, Direction dir, const EntityId & entity) const
{ 
    int current_tile_coord_x  = static_cast<int>(pos.x / Tile_Size);
    int current_tile_coord_y = static_cast<int>(pos.y / Tile_Size);

	if (dir == Direction::Down)
	{
        int x_coord = current_tile_coord_x <= 0 ? current_tile_coord_x - 1 : current_tile_coord_x;
		Tile * tile = m_map->GetTile(
            x_coord,
            current_tile_coord_y - 1);
		if(tile == nullptr || tile->tile->m_collision_y != Direction::Up)
			return false;
        if (C_RigidBody * rigid = m_systemMgr->GetEntityManager()->GetC_Rigidbody(entity))
            rigid->SetReferenceTile(tile, x_coord, current_tile_coord_y - 1);
	}
	else if (dir == Direction::Up)
	{ 
		Tile * tile = m_map->GetTile(
            current_tile_coord_x <= 0 ? current_tile_coord_x - 1 : current_tile_coord_x,
            current_tile_coord_y);
		if (tile == nullptr || tile->tile->m_collision_y != Direction::Down)
			return false;
	}
	else if (dir == Direction::Right)
	{
		Tile * tile = m_map->GetTile(current_tile_coord_x, 
            current_tile_coord_y < 0 ? current_tile_coord_y - 1 : current_tile_coord_y);
		if (tile == nullptr || tile->tile->m_collision_x != Direction::Left)
			return false;
	}
	else if (dir == Direction::Left)
	{
		Tile * tile = m_map->GetTile(current_tile_coord_x - 1, 
            current_tile_coord_y < 0 ? current_tile_coord_y - 1 : current_tile_coord_y);
		if (tile == nullptr || tile->tile->m_collision_x != Direction::Right)
			return false;
	}
	return true;
}
