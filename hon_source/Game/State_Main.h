#ifndef STATE_MAIN_H
#define STATE_MAIN_H
/******************************************************************************/
/*!
\headerfile   State_Main.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for State_Main class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "Option.h"
#include "Asterisk/Map.h"
#include "Asterisk/MapEditor.h"
#include "Asterisk/Text.h"
#include "Asterisk/SpriteSheet.h"
#include "Asterisk/Animation.h"

constexpr float MENU_X = 460.f;
constexpr float MENU_Y = 15.f;
constexpr float MENU_GAP = -90.f;

enum BUTTON
{
	START,
	SETTING,
    CREDIT,
    QUIT,
    TOTAL
};

class State_Main : public BaseState
{
public:
    // Ctor
    State_Main(StateManager * stateMgr);
    ~State_Main();

    void Create();
    void Destroy();

    // Game updating
    void Update(float dt);
    void Draw();

private:
    void MakeSprite(Sprite & sp, TextureManager * txt, std::string name, float x, float y);
	void MakeSpriteSheet(SpriteSheet *& sp, TextureManager* texMgr, std::string name, std::string type, float x, float y);

	Sprite start, setting, credit, quit;
	Sprite main;

	SpriteSheet * soul, * bsoul1, * bsoul2, * player;

	Sprite background, creditText, copywright;
	Sprite areUsure, yesnoBlock;
	float scroll_speed = 100.f;

	Option option;

    int button = START;
	bool isOption = false, isCredit = false, isStart = false, isSwitching = false;
	bool areYouSure = false, yesOrNo = false;
	float elapsedTime = 0.f;
};

#endif