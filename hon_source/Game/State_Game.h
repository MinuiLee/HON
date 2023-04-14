#ifndef STATE_GAME_H
#define STATE_GAME_H
/******************************************************************************/
/*!
\headerfile   State_Game.h
\author Jooho Jeong
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for State_Game class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "../Asterisk/StateManager.h"
#include "../Asterisk/Map.h"
#include "../Asterisk/MapEditor.h"
#include "../Asterisk/Text.h"
#include "../Asterisk/DustParticle.h"
#include "Option.h"

namespace
{
    constexpr auto SOUL_UI_X = -620.f;
    constexpr auto SOUL_UI_Y = 350.f;
    constexpr auto SOUL_UI_GAP = 45.f;
}

class Item;
class State_Game : public BaseState
{
public:
    // Ctor
    State_Game(StateManager * stateMgr);
    ~State_Game();

    // State initializing
    void Create();
    void Destroy();

    // Game updating
    void Update(float dt);
    void Draw();

    // Callback
    void PlayerControl(EventDetails * details);

    void ReloadMap() { m_map->LoadMap("game"); }

	Map * GetMap() { return m_map; }
private: // Functions for callback
    void PlayerMovement(EventDetails * details);
    void ToggleInvincible(EventDetails * details);
    void SetHealth1(EventDetails * details);
    void ToggleWorldMap(EventDetails * details);
	void RevivalEffect(float dt, float in, float out);
	void BossUpdate(float dt);
private:
    Map * m_map; // game map
    bool IsEditingMode;
    bool IsDebugDraw = false;
	bool IsOption = true;
	bool IsStart = true;
    int Sound_num = 0;

    MapEditor * m_editor;
	Text text;
    Sprite boss_health_outer;
	Sprite boss_health_inner;
	DustParticle m_dust_particle;
	
    // Player Control
    float m_elapsed_time_from_attack = 0.f;
	std::pair<float, bool> m_fadeInOut;
	std::pair<float, bool> m_bossTimer;
	Option m_option;
};

#endif