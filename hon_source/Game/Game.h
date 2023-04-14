#ifndef GAME_H
#define GAME_H
/******************************************************************************/
/*!
\headerfile   Game.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for Game class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include <chrono>

#include "Asterisk/StateManager.h"
#include "Asterisk/TextureManager.h"
#include "Asterisk/EntityManager.h"
#include "Asterisk/SystemManager.h"
#include "Asterisk/SharedContext.h"
#include "Asterisk/AudioManager.h"
#include "Asterisk/GUIManager.h"
#include "Asterisk/FontManager.h"

class Game
{
public:
	//Ctor
	Game();
	~Game() { UnloadResource(); }

	//Main game looping methods
	void Update();
	void Draw();
    void RestartClock();
    void Clear();

    void LoadResource();
    void UnloadResource();

	//Data getting methods
	Window & GetWindow() { return m_window; }

private:
	void DisplayLoading();
	Sprite m_lodingScreen;

private:
	SharedContext m_context;
	Window m_window;

	TextureManager m_textureMgr;
	StateManager m_stateMgr;
	SystemManager m_systemMgr;
	EntityManager m_entityMgr;
	AudioManager m_audioMgr;
	GUIManager m_GUIMgr;

    struct Clock
    {
        uint32_t last_tick_time = 0;
        uint32_t dt = 0;

        void tick()
        {
            uint32_t tick_time = SDL_GetTicks();
            dt = tick_time - last_tick_time;
            last_tick_time = tick_time;
        }
    };
    Clock m_clock;
};

#endif