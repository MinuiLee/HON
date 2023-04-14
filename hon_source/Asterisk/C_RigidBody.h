#ifndef C_RIGIDBODY_H
#define C_RIGIDBODY_H
/******************************************************************************/
/*!
\headerfile   C_RigidBody.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for RigidBody class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <glm.hpp>
#include "Transform.h"
#include "C_Base.h"
#include "Sprite.h"

#include "Direction.h"

using namespace glm;

enum BodyShape
{
	Box,
	Polygon_
};
enum BodyType
{
	Static,
	Dynamic
};

struct Tile;
class C_RigidBody : public C_Base
{
public:
	C_RigidBody() : C_Base(Component::RigidBody) {}
	void ReadIn(std::stringstream & sstream);
	
    void AddVelocity(const vec3 vel);
    void AddBeyondMaxVel(float x, float y, float z) { m_velocity += Vector3f(x, y, z); }
	void AddForce(const vec3 force) { m_acceleration += force; }
    const Vector2f& GetMaxVelocity() { return m_maxvelocity; }

	void SetDirection(Direction dir) { m_dir = dir; }
	void SetBodyShape(const BodyShape bodyshape) { m_bodyshape = bodyshape; }
	void SetBodyType(const BodyType bodytype) { m_bodytype = bodytype; }
	void SetVelocity(const vec3 vel);
    void SetBeyondMaxVel(float x, float y, float z) { m_velocity = Vector3f(x, y, z); }
    void SetAcceleration(const Vector3f& force) { m_acceleration = force; }
    void SetAccelerationX(float accel) { m_acceleration.x = accel; }
    void SetAccelerationY(float accel) { m_acceleration.y = accel; }

    void ApplyFriction(float x, float y);

	vec3 GetVelocity() { return m_velocity; }
	vec3 GetAcceleration() { return m_acceleration; }
	float GetForce() { return m_force; }
	Direction GetDirection() { return m_dir; }

    void SetReferenceTile(Tile * tile, int x = 0, int y = 0)
	{ 
		if (tile)
		{
			m_last_ref_tile_coord.x = x;
			m_last_ref_tile_coord.y = y;
		}
		m_reference_tile = tile;
	}
    Tile * GetReferenceTile(){ return m_reference_tile; }
    const Vector2f & GetDefaultFriction() { return m_default_friction; }
	const Vector2i & GetLastRefTileCoord() { return m_last_ref_tile_coord; }

    float GetJumpSpeed() { return m_jump_speed; }

    bool IsEntityOnGround() { return m_reference_tile != nullptr; }

    void SetFly(bool can_fly) { m_can_fly = can_fly; }
    bool CanEntityFly() const { return m_can_fly; }

private:
	float m_force = 0.f;
    Vector2f m_maxvelocity;

	vec3 m_velocity = { 0.f, 0.f ,0.f };
	vec3 m_acceleration = { 0.f, 0.f, 0.f };
    float m_jump_speed = 0.f;

    Tile * m_reference_tile = nullptr;
    Vector2f m_default_friction = Vector2f(0.8f, 0.f);
	Vector2i m_last_ref_tile_coord = Vector2i(0, 0);

    bool m_can_fly = false;

	BodyType m_bodytype = Dynamic;
	BodyShape m_bodyshape = Box;
	Direction m_dir = Direction::Down;
};

#endif