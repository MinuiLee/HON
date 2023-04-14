/******************************************************************************/
/*!
\file   SystemManager.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "SystemManager.h"
#include "Window.h"

#include "S_Base.h"
#include "S_Renderer.h"
#include "S_Movement.h"
#include "S_State.h"
#include "S_Control.h"
#include "S_SpriteSheet.h"
#include "S_Collision.h"
#include "S_AI.h"
#include "S_Audio.h"
#include "S_Attackable.h"
#include "S_Sprite.h"
#include "S_GameObject.h"
#include "C_Position.h"
#include "S_Particle.h"

SystemManager::SystemManager()
{
	// Allocating all systems
	m_systems.emplace(System::Renderer, new S_Renderer(this));
	m_systems.emplace(System::Movement, new S_Movement(this));
	m_systems.emplace(System::State, new S_State(this));
	m_systems.emplace(System::Control, new S_Control(this));
	m_systems.emplace(System::SheetAnimation, new S_SpriteSheet(this));
	m_systems.emplace(System::Sprite, new S_Sprite(this));
	m_systems.emplace(System::Collision, new S_Collision(this));
	m_systems.emplace(System::AI, new S_AI(this));
	m_systems.emplace(System::Audio, new S_Audio(this));
	m_systems.emplace(System::Attackable, new S_Attackable(this));
	m_systems.emplace(System::GameObject, new S_GameObject(this));
	m_systems.emplace(System::Particle, new S_Particle(this));
}

void SystemManager::InitializeShader(ShaderContainer * input_)
{
	auto itr = m_systems.find(System::SheetAnimation);
	if (itr == m_systems.end()) return;
	static_cast<S_SpriteSheet *>(itr->second)->Initialize(*input_);

	itr = m_systems.find(System::Sprite);
	if (itr == m_systems.end()) return;
	static_cast<S_Sprite *>(itr->second)->Initialize(*input_);
}

void SystemManager::Update(Window * window, float dt, bool IsEditorMode)
{
    for (auto & itr : m_systems)
        itr.second->Update(window, dt, IsEditorMode);
}

void SystemManager::Draw(Window * wind)
{
	auto itr = m_systems.find(System::Renderer);
	if (itr == m_systems.end()) return;
	static_cast<S_Renderer *>(itr->second)->Render(wind);

	auto gameObject = m_systems.find(System::GameObject);
	if (gameObject == m_systems.end()) return;
	static_cast<S_GameObject *>(gameObject->second)->Draw(wind);

	auto particles = m_systems.find(System::Particle);
	if (particles == m_systems.end()) return;
	static_cast<S_Particle *>(particles->second)->Draw(wind);
}
#ifdef _DEBUG
void SystemManager::DebugDraw(Window * wind, const C_Collidable * collision)
{
	auto itr = m_systems.find(System::Renderer);
	if (itr == m_systems.end()) return;
	static_cast<S_Renderer *>(itr->second)->DebugRender(wind, collision);
}

void SystemManager::DebugDraw(Window * wind, const C_Attackable * attack)
{
	auto itr = m_systems.find(System::Renderer);
	if (itr == m_systems.end()) return;
	static_cast<S_Renderer *>(itr->second)->DebugRender(wind, attack);
}

#endif
void SystemManager::EntityModified(const EntityId & entity, const Bitset & bits)
{
	for (auto & system : m_systems)
	{
		if (system.second->FitsRequitrment(bits))
		{
			if (system.second->HasEntity(entity))
				continue;
			else
				system.second->AddEntity(entity);
		}
		else
		{
			if (system.second->HasEntity(entity)) system.second->RemoveEntity(entity);
		}
	}
}

void SystemManager::RemoveEntity(const EntityId & entity)
{
	for (auto & itr : m_systems)
		itr.second->RemoveEntity(entity);
}

void SystemManager::AddEntity(const EntityId & entity, const Bitset & bits)
{
	for (auto & system : m_systems)
	{
		if (system.second->HasEntity(entity))
			continue;
		else if (system.second->FitsRequitrment(bits))
			system.second->AddEntity(entity);
	}
}

void SystemManager::PurgeSystems()
{
	if (m_systems.empty()) return;

	// De-allocate all systems
	for (auto & itr : m_systems)
		delete itr.second;
}

void SystemManager::PurgeEntities()
{
	if (m_systems.empty()) return;

	// De-allocate all systems
	for (auto & itr : m_systems)
		itr.second->PurgeEntities();
}

void SystemManager::SetPlayerID(unsigned int playerID)
{
	m_playerID = playerID;
	for (auto & system : m_systems)
		system.second->SetPlayerID(playerID);
}

void SystemManager::SetStateManager(StateManager * stateMgr)
{
	auto s_attackable = m_systems.find(System::Attackable);
	if (s_attackable == m_systems.end()) return;

	static_cast<S_Attackable *>(s_attackable->second)->SetStateManager(stateMgr);
}