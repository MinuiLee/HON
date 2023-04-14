#ifndef C_MOVABLE_H
#define C_MOVABLE_H

#include <SFML/System/Vector2.hpp>
#include "C_Base.h"
#include "Direction.h"
#include "ECS_Types.h"

class C_Movable : public C_Base
{
public:
    C_Movable() : C_Base(Component::Movable),
		  m_maxVelocity(0), m_dir((Direction)2) {}

    void SetVelocity(const sf::Vector2f & vel) { m_velocity = vel; }
    void AddVelocity(const sf::Vector2f & vel) { m_velocity += vel; }
    const sf::Vector2f & GetVelocity() { return m_velocity; }

    void SetDirection(const Direction & dir) { m_dir = dir; }
    const Direction & GetDirection() { return m_dir; }
    const sf::Vector2f & GetSpeed() { return m_speed; }
    float GetMaxVelocity() { return m_maxVelocity; }

    void ReadIn(std::stringstream & sstream)
    {
	sstream >> m_speed.x >> m_speed.y >> m_maxVelocity;
    }

private:
    sf::Vector2f m_velocity;
    sf::Vector2f m_speed;
    float m_maxVelocity;

    Direction m_dir;
};

#endif