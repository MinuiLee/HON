#ifndef STATEMANAGER_H
#define STATEMANAGER_H
/******************************************************************************/
/*!
\headerfile   StateManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for StateManager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <unordered_map>
#include "BaseState.h"
#include "Window.h"
#include "SharedContext.h"

enum class StateType
{
	SPLASH,
	GAME,
	MAIN,
	CREDIT
};

enum EndingType
{
	JAEJUN,
	HYUNGSEOB,
	JOOHO,
	MINUI
};

using States = std::unordered_map<StateType, BaseState *>;

class View;
class StateManager
{
public:
	StateManager(SharedContext * context);
	~StateManager();

	void Update(float dt);
	void Draw();
	void SwitchTo(const StateType & type);

	SharedContext * GetContext() { return m_context; }
	StateType GetCurrentStateType() const { return m_currentState; }
	View * GetCurrentView() const
	{
		auto current = m_states.find(m_currentState);
		if (current != m_states.end()) { return (*current).second->GetView(); }
		return nullptr;
	}

	template <typename T>
	void RegisterState(const StateType & type)
	{
		auto itr = m_states.find(type);
		if (itr == m_states.end())
		{
			BaseState * base = new T(this);
			m_states.emplace(type, base);
		}
	}

    void ReloadMap();

	EndingType & GetEndingType() {return m_ending;}

private:
	SharedContext * m_context;
	States m_states;
	StateType m_currentState;
	EndingType m_ending = EndingType::JAEJUN;
};

#endif