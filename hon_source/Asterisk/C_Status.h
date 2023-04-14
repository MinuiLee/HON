#ifndef C_STATUS_H
#define C_STATUS_H
/******************************************************************************/
/*!
\headerfile   C_Status.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for status component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"
#include "Sprite.h"

class C_Status : public C_Base
{
public:
	C_Status() : C_Base(Component::Status) {}

	void ReadIn(std::stringstream & sstream)
	{
		float health, damage, attack_speed;
		sstream >> health >> damage >> attack_speed >> m_respawnTime;

		m_health	   = static_cast<int>(health);
		m_max_health   = m_health;
		m_damage	   = static_cast<int>(damage);
		m_attack_speed = static_cast<int>(attack_speed);
		m_souls.x	  = static_cast<unsigned>(health)/2;
		m_souls.y = static_cast<unsigned>(health) / 2;
	}

	int GetHealth() const { return m_health; }
	void SetHealth(int health) { m_health = health; }
	int GetMaxHealth() const { return m_max_health; }
	void IncreaseMaxHealth() { m_max_health++; }

	int GetDamage() const { return m_damage; }
	void SetDamage(int damage) { m_damage = damage; }

	int GetAttackSpeed() const { return m_attack_speed; }
	void SetAttackSpeed(int attack_speed) { m_attack_speed = attack_speed; }

	// For cheat code
	bool IsInvincible() const { return m_IsInvincible || invincible_cheat; }
	void ToggleInvincible()
	{
		m_IsInvincible	 = !m_IsInvincible;
		m_invincible_timer = 0.f;
	}
	void UpdateInvincibleTimer(float dt) { m_invincible_timer += dt; }
	float GetInvincibleTimer() const { return m_invincible_timer; }

	void AddSouls(int purple, int blue)
	{
		m_souls.x += purple;
		m_souls.y += blue;
	}
	void SetSouls(int purple, int blue)
	{
		m_souls.x = purple;
		m_souls.y = blue;
	}
	void TakeAwaySoul()
	{
		bool soul = m_souls.x >= m_souls.y;
		if (soul)
		{
			m_souls.x--;
			m_recent_lost_soul_color = true;
		}
		else
		{
			m_souls.y--;
			m_recent_lost_soul_color = false;
		}

	}
	Vector2i GetSouls() const { return m_souls; }
	float GetRespawnTime() const { return m_respawnTime; }

	Sprite & GetHealthBar() { return m_health_bar; }
	const Sprite & GetHealthBar() const { return m_health_bar; }
	void UpdateHRTimer(float dt) { m_health_render_timer += dt; }
	float GetHRTimer() { return m_health_render_timer; }
	void ResetHRTimer() { m_health_render_timer = 0.f; }

	void SetDoubleDashUsed(bool toggle) { double_dash_used = toggle; }
	bool IsDoubleDashUsed() { return double_dash_used; }
	void SetAirUppercutUsed(bool toggle) { air_uppercut_used = toggle; }
	bool IsAirUppercutUsed() { return air_uppercut_used; }

	int IsHoldingFlower() { return m_isHoldingFlower;}
	void SetFlowerHolding(int flower_id) { m_isHoldingFlower = flower_id; }
	void PutFlowerToVase(int index) 
	{
		m_flowers.set(size_t(index), 1);
	}
    int FlowerCount() {
    return static_cast<int>(m_flowers.count()); }

    void SetAttacked(bool attacked) { already_attacked = attacked; }
    bool IsAlreadyAttacked() { return already_attacked; }

    bool IsInvincibleCheatEnabled() { return invincible_cheat; }
    void ToggleInvincibleCheat() {invincible_cheat = !invincible_cheat;}
	bool GetRecentLostSoulColor() { return m_recent_lost_soul_color; }

private:
	Sprite m_health_bar;
	float m_health_render_timer = 0.f;

	int m_max_health	= 0;
	int m_health		= 0;
	int m_damage		= 0;
	int m_attack_speed  = 0; // how many times can attack at 1 second
	float m_respawnTime = 0.f;
	Vector2i m_souls	= { 0, 0 };
	Bitset m_flowers;
	int m_isHoldingFlower = 0;
	//...armor, knock_back, speed
	bool air_uppercut_used = false;
	bool double_dash_used  = false;
    bool already_attacked = false;

	// For cheat code
    bool invincible_cheat = false;
	bool m_IsInvincible		 = false;
	float m_invincible_timer = 0.f;

	//false - purple
	//true - blue
	bool m_recent_lost_soul_color = false;
};

#endif