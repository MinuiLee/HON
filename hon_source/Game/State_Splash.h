#ifndef STATE_SPLASH_H
#define STATE_SPLASH_H
/******************************************************************************/
/*!
\headerfile   State_Splash.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for State_Splash class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Asterisk/StateManager.h"
#include "Asterisk/Text.h"

enum LOGO
{
	DIGIPEN,
	FMODD,
	ASTERISK
};

class State_Splash : public BaseState
{
public:
	//Ctor & Dtor
	State_Splash(StateManager * stateMgr);
	~State_Splash();

	//State Initializing - This is called when entering current state
	void Create();
	void Destroy();
	void ReinitMeshes();

	//Game updating
	void Update(float dt);
	void Draw();
private:
	Sprite m_digipen, m_asterisk, m_fmod;
	bool m_bool = true;
	float m_timer = 3.f;

	LOGO logo = DIGIPEN;
};

#endif