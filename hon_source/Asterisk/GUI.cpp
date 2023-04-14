/******************************************************************************/
/*!
\file   GUI.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>

#include "GUI.h"
#include "FontManager.h"
#include "C_Status.h"
#include "Window.h"

GUI::GUI(SharedContext * sharedContext, const Vector3f & position, const std::string & texture_name)
	: m_name(texture_name), m_position(position)
{
	m_entMgr	 = sharedContext->m_entityMgr;
	m_textureMgr = sharedContext->m_textureMgr;
	m_evtMgr	 = sharedContext->m_evMgr;

	if (m_name.find("Text") != std::string::npos)
	{
		m_ui_text.SetShader(
			sharedContext->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));
		m_ui_text.SetFont(*(FontManager::GetInstance()->GetPixelFont()));
		m_ui_text.SetPosition(m_position);
		if (m_name == "Text_M")
			m_ui_text.SetScale(0.8f, 0.8f, 1.f);
		else
			m_ui_text.SetScale(0.5f, 0.5f, 1.f);
		m_type = Type::Ttext;
	}
	else if (m_name.find("Animation") != std::string::npos)
	{
		m_ui_spritesheet = new SpriteSheet(m_name.substr(10), m_textureMgr);
		m_ui_spritesheet->SetCurrentAnimation("Body", "Idle", true, true);
		m_ui_spritesheet->SetShader(
			sharedContext->m_window->GetBaseWindow()->GetShaders(), "Animation");
		m_ui_spritesheet->SetSpritePosition(m_position);
		m_ui_spritesheet->SetDepth(GUI_TEXT_DEPTH);
		m_type = Type::Animation;
	}
	else // image
	{
		if (m_textureMgr->RequireResource(m_name))
		{
			Texture * txt = m_textureMgr->GetResource(m_name);
			m_ui_sprite.SetTexture(*txt);
			m_ui_sprite.BuildMesh(txt);
			m_ui_sprite.SetPosition(m_position);
			m_type = Type::Image;
		}
	}
}

GUI::~GUI()
{
	m_ui_sprite.DeleteMesh();
	m_textureMgr->ReleaseResource(m_name);
}

void GUI::Update(float dt, Map * map)
{
	if (m_type == Type::Image) { m_ui_sprite.SetPosition(m_position.x, m_position.y); }
	else if (m_type == Type::Animation)
	{
		float zoom   = map->GetWindow()->GetBaseWindow()->GetZoom();
		Vector2f pos = map->GetViewCenter();

		if (map->IsWorldMapOpen())
			m_ui_spritesheet->GetSprite("Body")->SetPosition(
				pos + Vector2f(m_position.x, m_position.y) * zoom);
		else
			m_ui_spritesheet->GetSprite("Body")->SetPosition(
				m_position.x + pos.x, m_position.y + pos.y);

		m_ui_spritesheet->GetSprite("Body")->SetScale(zoom, zoom);
		m_ui_spritesheet->Update(dt);
	}
	else // text
	{
		int pSoul = m_entMgr->GetC_Status(map->GetPlayerId())->GetSouls().x;
		int bSoul = m_entMgr->GetC_Status(map->GetPlayerId())->GetSouls().y;
		if (m_name == "Text_purple_soul")
			m_ui_text.SetString(std::to_string(pSoul < 0 ? 0 : pSoul));
		else if (m_name == "Text_blue_soul")
			m_ui_text.SetString(std::to_string(bSoul < 0 ? 0 : bSoul));
		else if (m_name == "Text_M")
			m_ui_text.SetString(SDL_GetScancodeName(m_evtMgr->GetBindingCode("Toggle_WorldMap")));
	}
}

void GUI::Draw(Window * window)
{
	if (m_type == Type::Image)
	{
		window->GetBaseWindow()->CalculateDirectProjectionDraw(m_ui_sprite, GUI_DEPTH);
		window->GetBaseWindow()->DirectProjectionDraw(m_ui_sprite);
	}
	else if (m_type == Type::Ttext)
	{
		m_ui_text.DirectProjectionDraw(window, GUI_TEXT_DEPTH);
	}
	else // animation
	{
		m_ui_spritesheet->Draw(window, false);
	}
}
