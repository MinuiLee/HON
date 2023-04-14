/*!*******************************************************************
\file         DoorUI.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/03/03

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "DoorUI.h"

#include "../FontManager.h"
#include "../TextureManager.h"

#include "../Texture.h"

#include "../BaseWindow.h"

#define KEY_TEXT_SCALE 0.4f
#define SOUL_SCALE 1.5f

#define SPRITE_XOFFSET 30.f
#define SPRITE_YOFFSET 20.f

#define TEXT_XOFFSET 8.7f
#define TEXT_YOFFSET 1.5f

DoorUI * DoorUI::GetInstance()
{
	static DoorUI instance;

	return &instance;
}

DoorUI::DoorUI()
{
	m_purple_key.SetFont(*(FontManager::GetInstance()->GetPixelFont()));
	m_blue_key.SetFont(*(FontManager::GetInstance()->GetPixelFont()));

	m_purple_key.SetScale(KEY_TEXT_SCALE, KEY_TEXT_SCALE, 1.f);
	m_blue_key.SetScale(KEY_TEXT_SCALE, KEY_TEXT_SCALE, 1.f);

	m_purple_key.SetString("A");
	m_blue_key.SetString("S");
}

void DoorUI::Draw(Window * window)
{
	if (!m_draw) return;

	auto baseWind = window->GetBaseWindow();

	baseWind->Draw(m_purple_soul);
	baseWind->Draw(m_blue_soul);

	m_purple_key.Draw(window, ENTITIES_TEXT_DEPTH);
	m_blue_key.Draw(window, ENTITIES_TEXT_DEPTH);
}

void DoorUI::Initialize(TextureManager* textureMgr)
{
	Texture* purple = textureMgr->GetResource("Object_Soul_purple");
	Texture* blue = textureMgr->GetResource("Object_Soul_blue");

	m_purple_soul.SetTexture(*purple);
	m_blue_soul.SetTexture(*blue);

	m_purple_soul.BuildMesh(purple);
	m_blue_soul.BuildMesh(blue);
}

void DoorUI::SetPosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;

	AdjustPosition();
}

void DoorUI::SetPosition(Vector2f position)
{
	m_position = position;

	AdjustPosition();
}

void DoorUI::SetText(SDL_Scancode purple, SDL_Scancode blue)
{
	m_purple_key.SetString(SDL_GetScancodeName(purple));
	m_blue_key.SetString(SDL_GetScancodeName(blue));
}

void DoorUI::ToggleDraw()
{
	m_draw = !m_draw;
}

bool DoorUI::IsDrawing()
{
	return m_draw;
}

void DoorUI::AdjustPosition()
{
	Vector2f condition_pos = Vector2f(m_position.x, m_position.y + 33.f);

	condition_pos.y -= SPRITE_YOFFSET;

	m_purple_soul.SetPosition(condition_pos.x - SPRITE_XOFFSET, condition_pos.y);
	m_blue_soul.SetPosition(condition_pos.x + SPRITE_XOFFSET, condition_pos.y);

	condition_pos.x -= TEXT_XOFFSET;
	condition_pos.y += TEXT_YOFFSET;

	m_purple_key.SetPosition(condition_pos.x - SPRITE_XOFFSET, condition_pos.y);
	m_blue_key.SetPosition(condition_pos.x + SPRITE_XOFFSET, condition_pos.y);
}