/******************************************************************************/
/*!
\file   S_Movement.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Movement.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_Position.h"
#include "C_RigidBody.h"
#include "C_State.h"
#include "C_AI.h"
#include "C_Status.h"
#include "Tile.h"
#include "MovementConstants.h"

Vector3f GetNormalizedTangent(const Vector3f & vec)
{
    float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    Vector3f normalized = vec / length;
    return Vector3f(-normalized.y, normalized.x, 0);
}

// Ctor
S_Movement::S_Movement(SystemManager * sysMgr) : S_Base(System::Movement, sysMgr)
{
	Bitset bit;
	bit.set((size_t)Component::Position, 1);
	bit.set((size_t)Component::RigidBody, 1);
	bit.set((size_t)Component::State, 1);
	m_requirement.emplace_back(bit);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::GetAttacked, this);
	handler->Subscribe(EntityMessage::Pushed, this);
	handler->Subscribe(EntityMessage::Colliding, this);
	handler->Subscribe(EntityMessage::Jumping, this);
	handler->Subscribe(EntityMessage::Dash, this);
	handler->Subscribe(EntityMessage::Uppercut, this);
	handler->Subscribe(EntityMessage::Pop, this);
	handler->Subscribe(EntityMessage::Is_OnAir, this);
	handler->Subscribe(EntityMessage::Direction_Changed, this);
	handler->Subscribe(EntityMessage::ShieldBlocking, this);
}

// S_Base methods
void S_Movement::Update(Window * window, float dt, bool /*editor_mode*/)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	MessageHandler * handler  = m_systemMgr->GetMessageHandler();
	FloatRect view_space	  = window->GetBaseWindow()->GetUpdateCullingSpace();

	for (auto itr : m_entities)
	{
		C_Position * tran = entityMgr->GetC_Position(itr);
		C_RigidBody * rigid = entityMgr->GetC_Rigidbody(itr);
		C_State * state		= entityMgr->GetC_State(itr);

		switch (state->GetState())
		{
		case EntityState::Walk:
		{
			Vector3f velocity = rigid->GetVelocity();
			switch (state->GetDirection())
			{
			case Direction::Left:
				rigid->SetAccelerationX(-rigid->GetForce());
				SetDirection(itr, Direction::Left);
				break;
			case Direction::Right:
				rigid->SetAccelerationX(rigid->GetForce());
				SetDirection(itr, Direction::Right);
				break;
			}
			break;
		}
		case EntityState::Jump:
		{
			if ((rigid->IsEntityOnGround() || IsCollidingFragileWall(itr)))
			{
				Message msg(EntityMessage::Became_Idle, itr);
				handler->Dispatch(msg);
			}
			break;
		}
		case EntityState::Dash:
		{
			if (abs(rigid->GetVelocity().x) < rigid->GetMaxVelocity().x)
			{
				Message msg(EntityMessage::Became_Idle, itr);
				state->SetAction(false);
				handler->Dispatch(msg);
				if (C_Status * status = entityMgr->GetC_Status(itr))
					status->SetDoubleDashUsed(false);
			}
			break;
		}
		case EntityState::Hurt:
		{
			state->UpdateDelay(dt);
			if (state->GetDelay() > HURT_DELAY)
			{
				state->ResetDelay();
				state->SetAction(false);
				Message msg(EntityMessage::Became_Idle, itr);
				handler->Dispatch(msg);
			}
			break;
		}
		case EntityState::Stomp:
		{
			rigid->SetVelocity(Vector3f(0, 0, 0));
			state->UpdateDelay(dt);
			if (state->GetDelay() > STOMP_DELAY)
			{
				state->ResetDelay();
				state->SetAction(false);
				Message msg(EntityMessage::Became_Idle, itr);
				handler->Dispatch(msg);
			}
			break;
		}
		case EntityState::Air_Stomp:
		{
			if (!rigid->IsEntityOnGround())
			{
				if (state->GetOldState() != state->GetState())
				{
					state->SetOldState(EntityState::Air_Stomp);

					// Air stomp start
					Message msg(EntityMessage::Air_Stomp_Start, itr);
					handler->Dispatch(msg);
				}

				rigid->SetVelocity(Vector3f(rigid->GetVelocity().x, AIR_STOMP_VELOCITY, 0));
				state->UpdateTimer(dt);

				if (IsCollidingFragileWall(itr)) AirStompEnd(state, window, handler, itr, dt);
			}
			else
				AirStompEnd(state, window, handler, itr, dt);

			break;
		}
		case EntityState::Dead:
		{
			if (IsDeadAnimationEnd(itr)) entityMgr->RemoveEntity(itr);

			if (entityMgr->GetEntityType(itr) == "Object_Plate")
				rigid->SetVelocity(Vector3f(0, 0, 0));
			break;
		}
		case EntityState::Knockdown:
		{
			if (IsDeadAnimationEnd(itr))
			{
				state->SetAction(false);
				Message msg(EntityMessage::Became_Idle, itr);
				handler->Dispatch(msg);

				rigid->SetVelocity(Vector3f(0, 0, 0));
				Vector2i last_tile_pos = rigid->GetLastRefTileCoord();
				tran->SetPosition(Vector3f(last_tile_pos.x * Tile_Size + Tile_Size / 2,
					(last_tile_pos.y + 3) * Tile_Size, 0.2f));
			}
			break;
		}
		case EntityState::Uppercut:
		{
			state->SetOldState(EntityState::Uppercut);
			state->UpdateDelay(dt);
			if (state->GetDelay() > UPPERCUT_DELAY)
			{
				if (state->GetDelay() > UPPERCUT_DELAY + UPPERCUT_DELAY_FREEZE)
				{
					state->SetAction(false);
					state->ResetDelay();
					Message msg_idle(EntityMessage::Became_Idle, itr);
					handler->Dispatch(msg_idle);
				}
				rigid->SetVelocity(Vector3f(0, 0, 0));
				continue;
			}
			break;
		}
		}

		// Update position & velocity
		if (!rigid->CanEntityFly()) rigid->AddForce(Vector3f(0, GRAVITY, 0));
		Vector3f acceleration = rigid->GetAcceleration();
		rigid->AddVelocity(Vector3f(acceleration.x * dt, acceleration.y * dt, 0));

		Vector2f tile_friction;
		if (Tile * reference_tile = rigid->GetReferenceTile())
			tile_friction = reference_tile->tile->m_friction;
		else
			tile_friction = rigid->GetDefaultFriction();

		float friction_x = rigid->GetForce() * tile_friction.x * dt;
		float friction_y = rigid->GetForce() * tile_friction.y * dt;
		if (entityMgr->GetEntityType(itr) != "Object_Plate" &&
			entityMgr->GetEntityType(itr) != "Entity_Fork")
			rigid->ApplyFriction(friction_x, friction_y);

        Vector3f velocity = rigid->GetVelocity();
		tran->MoveBy(velocity.x * dt, velocity.y * dt, 0);
		rigid->SetAcceleration(Vector3f(0, 0, 0));
	}
}

// Observer method
void S_Movement::Notify(Message & message)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	C_RigidBody * rigid		  = entityMgr->GetC_Rigidbody(message.m_receiver);

	switch (message.m_type)
	{
	case EntityMessage::Colliding:
	{
		Vector3f vel = rigid->GetVelocity();

		if (message.m_axis == Message::Axis::X)
			rigid->SetVelocity(Vector3f(vel.x, 0, vel.z));
		else if (message.m_axis == Message::Axis::Y)
			rigid->SetVelocity(Vector3f(0, vel.y, vel.z));
		break;
	}
	case EntityMessage::Pushed:
	case EntityMessage::GetAttacked:
	{
		if (entityMgr->GetEntityType(message.m_receiver) == "Entity_TreasureBox") break;

		C_State * state			 = entityMgr->GetC_State(m_playerID);
		EntityState player_state = state->GetState();

		C_Position * attacker_pos = entityMgr->GetC_Position(message.int_value);
		C_Position * target_pos   = entityMgr->GetC_Position(message.m_receiver);

		if (player_state == EntityState::Uppercut)
		{
			if (attacker_pos->GetPosition().x < target_pos->GetPosition().x)
				rigid->SetBeyondMaxVel(ATTACK_IMPACT, ATTACK_IMPACT * 1.2f, 0);
			else if (attacker_pos->GetPosition().x > target_pos->GetPosition().x)
				rigid->SetBeyondMaxVel(-ATTACK_IMPACT, ATTACK_IMPACT * 1.2f, 0);
		}
		else if (player_state == EntityState::Air_Stomp)
		{
			if (attacker_pos->GetPosition().x < target_pos->GetPosition().x)
				rigid->SetBeyondMaxVel(ATTACK_IMPACT * 2, ATTACK_IMPACT / 2, 0);
			else if (attacker_pos->GetPosition().x > target_pos->GetPosition().x)
				rigid->SetBeyondMaxVel(-ATTACK_IMPACT * 2, ATTACK_IMPACT / 2, 0);
		}
		else if (player_state == EntityState::Stomp)
		{
			float velocity = rigid->GetMaxVelocity().x;
			if (attacker_pos->GetPosition().x < target_pos->GetPosition().x)
				rigid->SetVelocity(Vector3f(velocity, velocity, 0));
			else if (attacker_pos->GetPosition().x > target_pos->GetPosition().x)
				rigid->SetVelocity(Vector3f(-velocity, velocity, 0));
		}
		else
		{
			float velocity = rigid->GetMaxVelocity().x;
			if (attacker_pos->GetPosition().x < target_pos->GetPosition().x)
				rigid->SetBeyondMaxVel(velocity * 2, velocity, 0);
			else if (attacker_pos->GetPosition().x > target_pos->GetPosition().x)
				rigid->SetBeyondMaxVel(-velocity * 2, velocity, 0);
		}
		break;
	}
	case EntityMessage::Jumping:
	{
		C_State * state = entityMgr->GetC_State(message.m_receiver);

		if ((rigid->IsEntityOnGround() || IsCollidingFragileWall(message.m_receiver)) &&
			(!state->IsAction() || message.is_action))
		{
			rigid->SetVelocity(Vector3f(rigid->GetVelocity().x, rigid->GetJumpSpeed(), 0));
			state->SetOldState(EntityState::Jump);

			Message msg_jump(EntityMessage::Jumping_Start, message.m_receiver);
			m_systemMgr->GetMessageHandler()->Dispatch(msg_jump);
		}
		break;
	}
	case EntityMessage::Dash:
	{
		C_State * state = entityMgr->GetC_State(message.m_receiver);
		if (state->GetState() == EntityState::Dash && !message.is_action) return;

		Vector3f vel = rigid->GetVelocity();

		if (message.m_direction == Direction::Left)
			rigid->SetBeyondMaxVel(-DASH_SPEED, 0, 0);
		else if (message.m_direction == Direction::Right)
			rigid->SetBeyondMaxVel(DASH_SPEED, 0, 0);

		Message msg(EntityMessage::Dash_Start, message.m_receiver);
		m_systemMgr->GetMessageHandler()->Dispatch(msg);
		break;
	}
	case EntityMessage::Uppercut:
	{
		C_State * state = entityMgr->GetC_State(message.m_receiver);

		if (state->GetDirection() == Direction::Left)
			rigid->SetBeyondMaxVel(-UPPERCUT_VELOCITY_X, UPPERCUT_VELOCITY_Y, 0);
		else if (state->GetDirection() == Direction::Right)
			rigid->SetBeyondMaxVel(UPPERCUT_VELOCITY_X, UPPERCUT_VELOCITY_Y, 0);

		state->ResetDelay();

		Message msg(EntityMessage::Uppercut_Start, message.m_receiver);
		m_systemMgr->GetMessageHandler()->Dispatch(msg);
		break;
	}
	case EntityMessage::Pop:
	{
		rigid->AddVelocity(Vector3f(0, STOMP_IMPACT_VELOCITY, 0));
		break;
	}
	case EntityMessage::Is_OnAir:
	{
		message.is_action = !rigid->IsEntityOnGround();
		break;
	}
	case EntityMessage::Direction_Changed:
	{
		rigid->SetDirection(message.m_direction);
		break;
	}
	case EntityMessage::ShieldBlocking:
	{
		C_Position * attacker_pos = entityMgr->GetC_Position(message.int_value);
		C_Position * target_pos   = entityMgr->GetC_Position(message.m_receiver);
		if (attacker_pos->GetPosition().x < target_pos->GetPosition().x)
			rigid->SetBeyondMaxVel(ATTACK_IMPACT, 0, 0);
		else if (attacker_pos->GetPosition().x > target_pos->GetPosition().x)
			rigid->SetBeyondMaxVel(-ATTACK_IMPACT, 0, 0);
		break;
	}
	}
}

void S_Movement::SetDirection(const EntityId & entity, const Direction & dir)
{
	Message m(EntityMessage::Direction_Changed, entity);

	C_RigidBody * rigid		 = m_systemMgr->GetEntityManager()->GetC_Rigidbody(entity);
	MessageHandler * handler = m_systemMgr->GetMessageHandler();

	if (rigid->GetDirection() != dir)
	{
		if ((dir == Direction::Up || dir == Direction::Down) && rigid->GetVelocity().z != 0) return;
		rigid->SetDirection(dir);
		m.m_direction = dir;
		handler->Dispatch(m);
	}
}

bool S_Movement::IsDeadAnimationEnd(const EntityId & entity)
{
	Message msg(EntityMessage::IsDeadAnimationEnd, entity);
	m_systemMgr->GetMessageHandler()->Dispatch(msg);
	return msg.is_action;
}

void S_Movement::AirStompEnd(
	C_State * state, Window * window, MessageHandler * handler, EntityId id, float dt)
{
	if (state->GetDelay() == 0.f)
	{
		// Air stomp end
		Message msg(EntityMessage::Air_Stomp_End, id);
		msg.is_action = state->GetTimer() > HIGH_AIR_STOMP_FALLING_TIME;
		state->ResetTimer();
		handler->Dispatch(msg);

		if (!window->GetBaseWindow()->m_is_shake) window->GetBaseWindow()->m_is_shake = true;
	}

	state->UpdateDelay(dt);

	if (state->GetDelay() > AIR_STOMP_DELAY)
	{
		state->ResetDelay();
		state->ResetTimer();
		state->SetAction(false);
		Message msg_idle(EntityMessage::Became_Idle, id);
		handler->Dispatch(msg_idle);
	}
}
