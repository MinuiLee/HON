#ifndef OBSERVER_H
#define OBSERVER_H
/******************************************************************************/
/*!
\headerfile   Observer.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>
#include "Direction.h"
#include "EntityState.h"

enum class EntityMessage
{
	Player_Moving,                  // S_Control
    Player_Jumping,                //S_Control
    Player_Idle,                        //S_Control
    Player_Dash,                     //S_Control
    Player_Uppercut,            //S_Control
    Player_Stomp,                //S_Control
    Pay_Purple_Soul,               //S_Control, S_GameObject
    Pay_Blue_Soul,               //S_Control, S_GameObject

    Dash,
    Dash_Start,

    Air_Stomp,
    Air_Stomp_Start,
    Air_Stomp_End,
    High_Air_Stomp,

    Uppercut,
    Uppercut_Start,

    Stomp,
	Knockdown,

	Direction_Changed,
	State_Changed,               // S_SpriteSheet
	Request_Shader,
	GetFlower,
	ReleaseFlower,

    Require_State,
    Has_State_Animation,
    Is_OnAir,                           //S_Movement
    IsDeadAnimationEnd,
    IsCollidingFragileWall,
	Wall_Break,

	Became_Idle,                 // S_State

    Dead,
    DeadBySpike,
	Moving,                             // S_Movement
    Jumping,                        //S_Movement
    Jumping_Start,
	Colliding,                          // S_Movement

	Attack,
	GetAttacked,                //S_AI, S_Movement, S_GameObject
    Pushed,
	ShieldBlocking,

	SucceedAttack,
    Stuck,                              // S_AI
    Pop,
    Event,

	Drain_Soul
};

struct Message
{
	Message(EntityMessage m, int receiver)
        : m_type(m), m_receiver(receiver) {}
    ~Message() {}

    EntityMessage m_type;
    int m_receiver; // 4 -> 3

    // Additional Information
	struct Float2D
	{
	    Float2D(float x = 0, float y = 0) : m_x(x), m_y(y) {}
	    float m_x;
	    float m_y;
	};

	enum class Axis
	{
	    X,
	    Y
	};

	union {
	    Direction m_direction;
	    Float2D m_f2D;
	    Axis m_axis;
	};

    std::string m_name;
    EntityState m_state;
    int int_value = 0;
	float float_value = 0.f;
    bool is_action = false;
};

class Observer
{
public:
	Observer() {}
	virtual ~Observer() {}
	virtual void Notify(Message & message) = 0;
};

#endif