#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
/******************************************************************************/
/*!
\headerfile   SystemManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for SystemManager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <unordered_map>

#include "C_Base.h"
#include "S_Base.h"
#include "MessageHandler.h"

#include "ShaderContainer.h"

using EntityId	= unsigned int;
using SystemContainer = std::unordered_map<System, S_Base *>;

class Window;
class EntityManager;
class StateManager;
class AudioManager;
class FontManager;
#ifdef _DEBUG
class C_Collidable;
class C_Attackable;
#endif
class SystemManager
{
public:
	// Ctor & Dtor
	SystemManager();
	~SystemManager() { PurgeSystems(); }

	// Getter & Setter
	void SetEntityManager(EntityManager * entityMgr) { m_entityMgr = entityMgr; }
	void SetStateManager(StateManager * stateMgr);
	void SetAudioManager(AudioManager * audioMgr) { m_audioMgr = audioMgr; }

	EntityManager * GetEntityManager() { return m_entityMgr; }
	MessageHandler * GetMessageHandler() { return &m_messageHandler; }
	AudioManager * GetAudioManager() { return m_audioMgr; }

	void SetPlayerID(unsigned int playerID);
	unsigned int GetPlayerID() const { return m_playerID; }

    template <class T>
    T * GetSystem(System system)
    {
	auto itr = m_systems.find(system);
	return (itr != m_systems.end() ? static_cast<T *>(itr->second) : nullptr);
    }
	//void Initialize();
	void InitializeShader(ShaderContainer* input_);

	// Game loop methods
	void Update(Window * window, float dt, bool IsEditorMode);
	void Draw(Window * wind);
#ifdef _DEBUG
	void DebugDraw(Window * wind, const C_Collidable * collision);
	void DebugDraw(Window * wind, const C_Attackable * attack);
#endif

	// Entity registering to system
	void EntityModified(const EntityId & entity, const Bitset & bits);
	void RemoveEntity(const EntityId & entity);
	void AddEntity(const EntityId & entity, const Bitset & bits);

	void PurgeSystems();
	void PurgeEntities();

private:
	SystemContainer m_systems;
	MessageHandler m_messageHandler;

	EntityManager * m_entityMgr;
	AudioManager * m_audioMgr;

        unsigned int m_playerID;
};

#endif