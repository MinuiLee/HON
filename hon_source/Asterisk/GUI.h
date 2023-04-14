#ifndef GUI_H
#define GUI_H
/******************************************************************************/
/*!
\headerfile   GUI.h
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Sprite.h"
#include "Text.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "SharedContext.h"
#include "SpriteSheet.h"
#include "Map.h"

enum Type
{
	Image,
	Ttext,
	Animation
};

class Window;
class GUI
{
public:
	GUI(SharedContext * sharedContext, const Vector3f & position, const std::string & texture_name);
	~GUI();

	void Update(float dt, Map * map);
	void Draw(Window * window);

protected:
	Sprite m_ui_sprite;
	SpriteSheet * m_ui_spritesheet;
	Text m_ui_text;

private:
	TextureManager * m_textureMgr;
	EntityManager * m_entMgr;
	EventManager * m_evtMgr;

	std::string m_name;
	Vector3f m_position;
	Type m_type;
};

#endif