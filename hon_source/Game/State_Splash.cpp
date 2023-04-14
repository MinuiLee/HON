/******************************************************************************/
/*!
\file   State_Splash.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "State_Splash.h"
#include "Asterisk/TextureManager.h"
#include "../Asterisk/EventManager.h"
#include "SDL2/include/SDL.h"
#include "Asterisk/FontManager.h"

State_Splash::State_Splash(StateManager * stateMgr)
	: BaseState(stateMgr) {}

State_Splash::~State_Splash()
{
	Destroy();
}

void State_Splash::Create()
{
	TextureManager * texMgr = m_stateMgr->GetContext()->m_textureMgr;

	texMgr->RequireResource("DigiPen");
	Texture * txt = texMgr->GetResource("DigiPen");
	m_digipen.BuildMesh(txt);
	m_digipen.SetTexture(*txt);
	m_digipen.SetScale(0.9f, 0.9f);
	m_digipen.SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));


	texMgr->RequireResource("AsteriskLogo");
	txt = texMgr->GetResource("AsteriskLogo");
	m_asterisk.BuildMesh(txt);
	m_asterisk.SetTexture(*txt);
	m_asterisk.SetScale(1.2f, 1.2f);
	m_asterisk.Move(60.f, 0.f);
	m_asterisk.SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));

	texMgr->RequireResource("fmod");
	txt = texMgr->GetResource("fmod");
	m_fmod.BuildMesh(txt);
	m_fmod.SetTexture(*txt);
	m_fmod.SetScale(0.5f, 0.5f);
	m_fmod.SetShader(m_stateMgr->GetContext()->m_window->GetBaseWindow()->GetShaders().GetShader("Basic"));
}

void State_Splash::Destroy()
{
}

void State_Splash::ReinitMeshes() {}

void State_Splash::Update(float dt)
{
	if (m_stateMgr->GetEndingType() == EndingType::JAEJUN)
		return;

	if (!m_bool)
	{
		if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn)
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer -= dt;
		else if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut)
			m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer += dt;
	}

	if (m_timer < 0.f && m_bool)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = true;
		m_bool = false;
	}

	if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer > 1.f)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = true;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.0f;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.0f;

		if (logo == LOGO::ASTERISK)
		{
			m_stateMgr->SwitchTo(StateType::MAIN);
		}

		if (logo == LOGO::FMODD)
			logo = LOGO::ASTERISK;
		if (logo == LOGO::DIGIPEN)
			logo = LOGO::FMODD;
	}
	else if (m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer < 0.0f)
	{
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = true;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.0f;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.0f;
		m_timer = 2.f;
		m_bool = true;
	}

	else if (m_stateMgr->GetContext()->m_evMgr->IsKeyTriggered(SDL_SCANCODE_RETURN) && logo != LOGO::DIGIPEN)
	{
		m_stateMgr->SwitchTo(StateType::MAIN);
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeIn = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_is_fadeOut = false;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeIn_timer = 1.0f;
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->m_fadeOut_timer = 0.0f;
	}

	m_timer -= dt > 1.f ? 0 : dt;
}

void State_Splash::Draw()
{
	if (logo == LOGO::DIGIPEN)
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(m_digipen);
	else if (logo == LOGO::FMODD)
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(m_fmod);
	else
		m_stateMgr->GetContext()->m_window->GetBaseWindow()->Draw(m_asterisk);

}