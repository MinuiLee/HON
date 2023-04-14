/******************************************************************************/
/*!
\file   Game.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <iostream>

#include "Game.h"
#include "State_Game.h"
#include "State_Splash.h"
#include "State_Main.h"
#include "State_Credit.h"

#include "Asterisk/GameObject/DoorUI.h"

Game::Game()
    : m_context(
        &m_window,
        &m_textureMgr,
        m_window.GetEventManager(),
        &m_stateMgr,
        &m_systemMgr,
        &m_entityMgr,
        &m_audioMgr,
        &m_GUIMgr
        ),

    m_window(&m_stateMgr),
    m_textureMgr(),
    m_stateMgr(&m_context),
    m_systemMgr(),
    m_entityMgr(&m_systemMgr, &m_textureMgr),
    m_audioMgr(),
    m_GUIMgr(&m_context)
{
    m_systemMgr.SetEntityManager(&m_entityMgr);
    m_systemMgr.SetStateManager(&m_stateMgr);
    m_systemMgr.SetAudioManager(&m_audioMgr);

    m_stateMgr.RegisterState<State_Splash>(StateType::SPLASH);
    m_stateMgr.RegisterState<State_Game>(StateType::GAME);
    m_stateMgr.RegisterState<State_Main>(StateType::MAIN);
    m_stateMgr.RegisterState<State_Credit>(StateType::CREDIT);

	m_stateMgr.SwitchTo(StateType::SPLASH);
	
	DisplayLoading();
	
	LoadResource();

	m_stateMgr.GetEndingType() = EndingType::HYUNGSEOB;

	DoorUI::GetInstance()->Initialize(&m_textureMgr);
}

void Game::Update()
{
    //Event polling
    m_window.Update();

    //State updating
    if (m_window.IsWindowFocused())
        m_stateMgr.Update(m_clock.dt / 1000.f);
}

void Game::Draw()
{
    m_window.StartDraw(); // window.clear(sf::Color::Black);
    m_stateMgr.Draw(); // state drawing
    m_window.EndDraw(); // window.display();
}

void Game::RestartClock()
{
    m_clock.tick();
}

void Game::Clear()
{
    m_entityMgr.AddReservedEntities(m_clock.dt / 1000.f);
    if (m_entityMgr.ClearRemovedEntities())
        m_stateMgr.ReloadMap();
}

void Game::LoadResource()
{
    const PathContainer & texture_path = m_textureMgr.GetAllPath();
    for (auto & path : texture_path)
        m_textureMgr.RequireResource(path.first);

    const PathContainer & sound_path = m_audioMgr.GetAllPath();
    for (auto & path : sound_path)
        m_audioMgr.RequireResource(path.first);
}

void Game::UnloadResource()
{
    const PathContainer & texture_path = m_textureMgr.GetAllPath();
    for (auto & path : texture_path)
        m_textureMgr.ReleaseResource(path.first);

    const PathContainer & sound_path = m_audioMgr.GetAllPath();
    for (auto & path : sound_path)
        m_audioMgr.ReleaseResource(path.first);
}

void Game::DisplayLoading()
{
	m_textureMgr.RequireResource("Loading");
	Texture * txt = m_textureMgr.GetResource("Loading");
	m_lodingScreen.SetTexture(*txt);
	m_lodingScreen.BuildMesh(txt);
	m_lodingScreen.SetShader(m_window.GetBaseWindow()->GetShaders().GetShader("Basic"));

	m_window.StartDraw();
	m_window.GetBaseWindow()->Draw(m_lodingScreen);
	m_window.EndDraw();
}

