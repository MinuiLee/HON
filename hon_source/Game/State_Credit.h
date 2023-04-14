#ifndef STATE_CREDIT_H
#define STATE_CREDIT_H
/******************************************************************************/
/*!
\headerfile   State_Credit.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for State_Credit class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Asterisk\Sprite.h"
#include "Asterisk\StateManager.h"
#include "Asterisk\TextureManager.h"
#include "Asterisk/SpriteSheet.h"

class State_Credit : public BaseState
{
public:
	//Ctor & Dtor
    State_Credit(StateManager * stateMgr);
	~State_Credit();

	//State Initializing - This is called when entering current state
	void Create();
	void Destroy();

	//Game updating
	void Update(float dt);
	void Draw();
private:
	void MakeSprite(Sprite & sp, TextureManager * texMgr, std::string name, float x, float y);
	void MakeSpriteSheet(SpriteSheet *& sp, TextureManager* texMgr, std::string name, std::string type, float x, float y);
	void InitializeGhost(EndingType type);

	Sprite background, credit, human;
	SpriteSheet * bsoul, * psoul;
	float scroll_speed = 100.f;

	bool isStart = false;
	float elapsedTime = 0.f;
	float moveStart = 0.f;
};

#endif