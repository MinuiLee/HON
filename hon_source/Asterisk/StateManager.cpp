/******************************************************************************/
/*!
\file   StateManager.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "StateManager.h"
#include "../Game/State_Game.h"

StateManager::StateManager(SharedContext * context)
    : m_context(context), m_currentState(StateType::GAME)
{	}

StateManager::~StateManager()
{
	for (auto & itr : m_states)
		delete itr.second;
}

void StateManager::Update(float dt)
{
	auto itr = m_states.find(m_currentState);
	if (itr != m_states.end())
		itr->second->Update(dt);
}

void StateManager::Draw()
{
	auto itr = m_states.find(m_currentState);
	if (itr != m_states.end())
		itr->second->Draw();
}

void StateManager::SwitchTo(const StateType & type)
{
    auto toDestroy = m_states.find(m_currentState);
    if (toDestroy != m_states.end()) toDestroy->second->Destroy();

	auto itr = m_states.find(type);
	if (itr != m_states.end())
	{
		m_currentState = type;
		m_context->m_window->GetBaseWindow()->
			SetView(itr->second->GetView());
		m_context->m_evMgr->SetState(type);
		itr->second->Create();
	}
}

void StateManager::ReloadMap()
{
    auto state_itr = m_states.find(StateType::GAME);
    if (state_itr == m_states.end()) return;

    State_Game * state_game = dynamic_cast<State_Game*>(state_itr->second);
	state_game->GetMap()->LoadMap("current");
}
