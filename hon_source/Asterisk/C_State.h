#ifndef C_STATE_H
#define C_STATE_H
/******************************************************************************/
/*!
\headerfile   C_State.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for state component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"
#include "EntityState.h"

class C_State : public C_Base
{
public:
    C_State() : C_Base(Component::State), m_state(EntityState::Idle), 
        m_direction(Direction::Right) {}

    void ReadIn(std::stringstream & /*sstream*/) {}

    const EntityState & GetState() const  { return m_state; }
    void SetState(const EntityState & state)
    { 
        m_old_state = m_state;
        m_state = state; 
    }

    const EntityState & GetOldState() const { return m_old_state; }
    void SetOldState(const EntityState& state) { m_old_state = state; }

    const std::string GetStateString() const
    {
	switch (m_state)
	{
	case EntityState::Idle:
	    return "Idle";
	case EntityState::Walk:
	    return "Walk";
    case EntityState::Jump:
        return "Jump";
    case EntityState::Dash:
        return "Dash";
    case EntityState::Hurt:
        return "Hurt";
    case EntityState::Knockdown:
        return "Knockdown";
    case EntityState::Uppercut:
        return "Uppercut";
    case EntityState::Stomp:
        return "Stomp";
    case EntityState::Air_Stomp:
        return "Air_Stomp";
    case EntityState::Dead:
        return "Dead";
	}
	return "";
    }

    void SetDirection(Direction dir) { m_direction = dir; }
    Direction GetDirection() const { return m_direction; }

    bool IsAction() const { return m_is_action; }
    void SetAction(bool action) { m_is_action = action; }

    //State delay
    void UpdateDelay(float dt) { m_delay += dt; }
    void ResetDelay() { m_delay = 0.f; }
    float GetDelay() const { return m_delay; }

    //Timing - falling time
    void UpdateTimer(float dt) { m_timer += dt; }
    void ResetTimer() { m_timer = 0.f; }
    float GetTimer() const { return m_timer; }

	bool CanAct() const { return !m_is_action; }

private:
    EntityState m_old_state;
    EntityState m_state;
    Direction m_direction;

    float m_delay = 0.f;
    float m_timer = 0.f;
    bool m_is_action = false;
};

#endif