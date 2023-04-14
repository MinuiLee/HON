#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
/******************************************************************************/
/*!
\headerfile   EntityManager.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for entity manager class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

#include "ECS_Types.h"
#include "C_Base.h"
#include "Vector.h"

using EntityId		   = unsigned int;
using ComponentVector  = std::vector<C_Base *>;
using Entity		   = std::pair<std::pair<std::string, Bitset>, ComponentVector>;
using EntityContainer  = std::unordered_map<EntityId, Entity>;
using ComponentFactory = std::unordered_map<Component, std::function<C_Base *(void)>>;

using ComponentHash = std::unordered_map<std::string, Component>;

class SystemManager;
class TextureManager;
class EntityManager
{
public:
	EntityManager(SystemManager * systemMgr, TextureManager * texMgr);
	~EntityManager() { Purge(); }
	void Purge();
    bool ClearRemovedEntities();
    void AddReservedEntities(float dt);

	// Entity management
	int AddEntity(
		const std::string & file, const std::string img_name = "notused", bool collidable = false);
	void ReserveEntity(const std::string & entity_name, Vector3f pos, float time = 0.f);
	void RemoveEntity(const EntityId & id);
	bool HasEntity(const EntityId & id);

	// Component management
	bool HasComponent(const EntityId & id, const Component & type);
	bool AddComponent(const EntityId & id, const Component & type);
	bool RemoveComponent(const EntityId & id, const Component & type);

	template <class T>
	T * GetComponent(const EntityId & id, const Component & type)
	{
		if (!HasComponent(id, type)) return nullptr;
		auto component = std::find_if(m_entities[id].second.begin(), m_entities[id].second.end(),
			[&type](C_Base * base) { return (base->GetType() == type); });
		return (component != m_entities[id].second.end() ? dynamic_cast<T *>(*component) : nullptr);
	}

    #define GetC_Position(id) GetComponent<C_Position>(id, Component::Position)
    #define GetC_Attackable(id) GetComponent<C_Attackable>(id, Component::Attackable)
    #define GetC_Audio(id) GetComponent<C_Audio>(id, Component::Audio)
    #define GetC_Collidable(id) GetComponent<C_Collidable>(id, Component::Collidable)
    #define GetC_Controllable(id) GetComponent<C_Controllable>(id, Component::Controllable)
    #define GetC_Event(id) GetComponent<C_Event>(id, Component::Event)
    #define GetC_Rigidbody(id) GetComponent<C_RigidBody>(id, Component::RigidBody)
    #define GetC_Shader(id) GetComponent<C_Shader>(id, Component::Shader)
    #define GetC_Sprite(id) GetComponent<C_Sprite>(id, Component::Sprite)
    #define GetC_SpriteSheet(id) GetComponent<C_SpriteSheet>(id, Component::SpriteSheet)
    #define GetC_State(id) GetComponent<C_State>(id, Component::State)
    #define GetC_Status(id) GetComponent<C_Status>(id, Component::Status)
    #define GetC_AI(id) GetComponent<C_AI>(id, Component::AI)
    #define GetC_GameObject(id) GetComponent<C_GameObject>(id, Component::GameObject)

	ComponentFactory GetComponentFactory() { return m_factory; }
	ComponentHash GetComponentHash() { return m_hash; }
	size_t GetTotalEntityNum() { return m_entities.size(); }

	std::string GetEntityType(const int id) { return m_entities[id].first.first; }

	EntityId GetLastID() { return m_id; }

	const EntityContainer & GetEntityContainer() const { return m_entities; }
    unsigned int GetPlayerID();

    SystemManager * GetSystemManager() { return m_systemMgr; }
	TextureManager * GetTextureManager() { return m_textureMgr; }

private:
	template <class T>
	void RegisterComponent(Component type)
	{
		m_factory[type] = []() -> C_Base * { return new T; };
	}

	int GenerateEntity(const std::string & file);
	void MappingComponent();

	EntityId m_id;
	EntityContainer m_entities;
	ComponentFactory m_factory;
	ComponentHash m_hash;

    std::vector<EntityId> m_entities_to_delete;
    std::vector<std::pair<float, std::pair<std::string, Vector3f>>> m_entities_to_add;
	// <time, <name, pos>>

	SystemManager * m_systemMgr;
	TextureManager * m_textureMgr;
};

#endif