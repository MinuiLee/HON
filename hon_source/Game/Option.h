#ifndef OPTION_H
#define OPTION_H
/******************************************************************************/
/*!
\headerfile   Option.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\biref
		Header file for option class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <map>

#include "../Asterisk/Sprite.h"
#include "../Asterisk/TextureManager.h"
#include "../Asterisk/StateManager.h"
#include "../Asterisk/AudioManager.h"
#include "../Asterisk/Text.h"
#include "../Asterisk/SpriteSheet.h"

namespace
{
	constexpr float SCROLLER_GAP = 28.f;

	constexpr float BUTTON_X = 130.f;
	constexpr float SELECTED_X = 0.f;
	constexpr float BGM_Y = 220.f;
	constexpr float SFX_Y = 150.f;
	constexpr float MUTE_Y = 70.f;
	constexpr float FULLSCREEN_Y = -180.f;
	constexpr float FULLSCREEN_X = 125.f;
	constexpr float VSYNC_Y = -260.f;
	constexpr float GUIDEUI_Y = -340.f;
	constexpr float VOLUME_START_X = 5.f;
	constexpr float ICON_X_GAP = 100.f;
	constexpr float KEY_Y_GAP = 80.f;
	constexpr float KEY_Y = 235.f;
	constexpr float KEY_X = 135.f;
	constexpr float OPTION_Y = 210.f;
	constexpr float OPTION_X = -230.f;
	constexpr float OPTION_GAP = 130.f;
};
enum SETTING
{
	BGM,
	SFX,
	MUTE,
	FULLSCREEN,
	VSYNC,
	GUIDEUI
};

enum OPTION
{
	BACK,
	SETTINGS,
	KEYSETTING,
	MAINMENU
	
};

enum KEYS
{
	DASHL,
        DASHR,
	STOMP,
	UPPERCUT,
	JUMP,
	RIGHT,
	LEFT,
    MAP
};

class Option
{
public:
    Option() {}
    void Initialize(StateManager * txt, std::string state);
    bool Update(float dt);
    void Draw();

	void SetIsOption(bool b) { isOption = b; }
	void MoveAllToCenter(float x, float y);
	void ResetValues();
private:
	void MakeSprite(Sprite & sp, std::string name, float x, float y);
    void MakeSprite(unsigned int type, std::string name, float x, float y, unsigned kind = 0);

	std::map<int, Sprite> UI;
	std::map<KEYS, Text > KEYUI;
	Sprite black_screen, selected_setting, settings, key_screen, options;
	Sprite areUsure, yesnoBlock;
	SpriteSheet * soul;

	float sfx_volume = 0.f, bgm_volume = 0.f;
	bool isMute = false, isFullScreen = false, isOption = false, isMain = true, isKeySet = false, getInput = false;
	bool areYouSure = false, yesOrNo = false;

	int current_setting = SETTING::BGM;
	int current_option = OPTION::BACK;
	int current_keyset = KEYS::DASHL;

	TextureManager * m_txtMgr;
	StateManager * m_statMgr;
	AudioManager * m_audMgr;
};

#endif