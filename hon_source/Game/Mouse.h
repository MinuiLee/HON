#ifndef MOUSE_H
#define MOUSE_H
/******************************************************************************/
/*!
\headerfile   Mouse.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14
\brief
		Header file for Mouse class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/ 
#include "../Asterisk/Sprite.h"
#include "../Asterisk/StateManager.h"

class Mouse
{
public:
	Mouse() {}
	void Initialize(Texture * txt, int w, int h);

	bool IsCollidingWith(Sprite sp, Vector2f scale);
	bool IsCollidingWith(Sprite sp, float zoom, float dx = 0, float dy = 0);
	bool IsCollidingWith(std::vector<std::pair <std::string, Sprite>> objects, std::string name);
	void SetPosition(Vector2i pos);
	Sprite & GetSprite() { return mouse; }
private:
	Sprite mouse;
	int width, height;
};

#endif