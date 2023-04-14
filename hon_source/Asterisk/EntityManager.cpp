/******************************************************************************/
/*!
\file   EntityManger.cpp
\author Minui Lee
\par    email: alsdml0727@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <sstream>
#include <fstream>
#include <iostream>

#include "EntityManager.h"
#include "SystemManager.h"

#include "C_Position.h"
#include "C_SpriteSheet.h"
#include "C_Controllable.h"
#include "C_State.h"
#include "C_Collidable.h"
#include "C_RigidBody.h"
#include "C_AI.h"
#include "C_Audio.h"
#include "C_Status.h"
#include "C_Attackable.h"
#include "C_Sprite.h"
#include "C_Shader.h"
#include "C_GameObject.h"
#include "S_GameObject.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

using namespace rapidjson;

EntityManager::EntityManager(SystemManager * systemMgr, TextureManager * texMgr)
	: m_id(0), m_systemMgr(systemMgr), m_textureMgr(texMgr)
{
	RegisterComponent<C_Position>(Component::Position);
	RegisterComponent<C_Sprite>(Component::Sprite);
	RegisterComponent<C_SpriteSheet>(Component::SpriteSheet);
	RegisterComponent<C_State>(Component::State);
	RegisterComponent<C_RigidBody>(Component::RigidBody);
	RegisterComponent<C_Controllable>(Component::Controllable);
	RegisterComponent<C_Collidable>(Component::Collidable);
	RegisterComponent<C_AI>(Component::AI);
	RegisterComponent<C_Audio>(Component::Audio);
	RegisterComponent<C_Status>(Component::Status);
	RegisterComponent<C_Attackable>(Component::Attackable);
	RegisterComponent<C_Shader>(Component::Shader);
	RegisterComponent<C_GameObject>(Component::GameObject);
	MappingComponent();
}

void EntityManager::MappingComponent()
{
	m_hash["Position"]	 = Component::Position;
	m_hash["Sprite"]	   = Component::Sprite;
	m_hash["SpriteSheet"]  = Component::SpriteSheet;
	m_hash["Shader"]	   = Component::Shader;
	m_hash["State"]		   = Component::State;
	m_hash["RigidBody"]	= Component::RigidBody;
	m_hash["Controllable"] = Component::Controllable;
	m_hash["Collidable"]   = Component::Collidable;
	m_hash["AI"]		   = Component::AI;
	m_hash["Audio"]		   = Component::Audio;
	m_hash["Status"]	   = Component::Status;
	m_hash["Attackable"]   = Component::Attackable;
	m_hash["Event"]		   = Component::Event;
	m_hash["GameObject"]   = Component::GameObject;
}

unsigned int EntityManager::GetPlayerID()
{
   return m_systemMgr->GetPlayerID();
}

int EntityManager::GenerateEntity(const std::string & file)
{
	int id = m_id;
	if (!m_entities.emplace(id, std::make_pair(std::make_pair(file, Bitset()), ComponentVector()))
			 .second)
		return -1;

	++m_id;
	return id;
}

int EntityManager::AddEntity(
	const std::string & file, const std::string img_name, bool /*collidable*/)
{
	int id = -1;

	FILE * fp			 = nullptr;
	std::string filename = "Media/Entities/" + file + ".json";

	errno_t err = fopen_s(&fp, filename.c_str(), "rb");
	if (err != 0)
	{
		std::cout << "Cannot open the file : " << file << ".json\n";
		return -1;
	}

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document doc;

	doc.ParseStream(is);

	id = GenerateEntity(file);
	if (id == -1)
	{
		std::cout << "Failed to add entity\n";
		return -1;
	}
	Bitset bits;

	for (auto com : m_hash)
	{
		if (!doc.HasMember(com.first.c_str())) continue;

		AddComponent(id, com.second);
		bits.set((size_t)com.second, 1);

		C_Base * base = GetComponent<C_Base>(id, com.second);
		if (!base) return -1;

		std::string keystream;

		if (doc[com.first.c_str()].IsArray())
		{
			const Value & arr = doc[com.first.c_str()];
			for (SizeType i = 0; i < arr.Size(); i++)
			{
				if (arr[i].IsString())
					keystream += arr[i].GetString();
				else
					keystream += std::to_string(arr[i].GetDouble());

				keystream += " ";
			}
		}
		else if (doc[com.first.c_str()].IsString())
			keystream += doc[com.first.c_str()].GetString();

		if (img_name != "notused" && (com.second == Component::Sprite))
		{
			std::stringstream input(img_name);
			input >> *base;
		}
		else
		{
			std::stringstream input(keystream);
			input >> *base;
		}

		if (com.second == Component::SpriteSheet)
		{
			C_SpriteSheet * sheet = static_cast<C_SpriteSheet *>(base);
			sheet->SetTextureManager(m_textureMgr);
			sheet->SetSpriteSheet();

			C_Position * tran =
				m_systemMgr->GetEntityManager()->GetComponent<C_Position>(id, Component::Position);
			
			if (tran)
			{
				sheet->GetSpriteSheet()->SetDepth(tran->GetPosition().z);
			}
		}
		else if (com.second == Component::Sprite)
		{
			C_Sprite * sprite = static_cast<C_Sprite *>(base);
			sprite->SetTextureManager(m_textureMgr);
			sprite->TextureSetting();

			C_Position * tran =
				m_systemMgr->GetEntityManager()->GetComponent<C_Position>(id, Component::Position);

			sprite->GetSprite().SetPosition(tran->GetPosition().x, tran->GetPosition().y);
			sprite->GetSprite().SetDepth(tran->GetPosition().z);

			if (tran)
			{
				sprite->UpdateSpritePosition(Vector3f(tran->GetPosition().x,
					tran->GetPosition().y + sprite->GetSpriteSize().y / 2, tran->GetPosition().z));
			}
		}
        else if (com.second == Component::Status)
        {
            C_Status * status = static_cast<C_Status*>(base);
            Sprite & health_bar = status->GetHealthBar();
            Texture * health = m_textureMgr->GetResource("Health");
            health_bar.SetTexture(*health);
            health_bar.BuildMesh(health);
        }
	}

	if (auto AI = GetC_AI(id); AI && AI->GetType() == EnemyType::Flying)
		GetC_Rigidbody(id)->SetFly(true);

	// Register entity to systems
	m_systemMgr->AddEntity(id, bits);

	fclose(fp);
	return id;
}

void EntityManager::ReserveEntity(const std::string & entity_name, Vector3f pos, float time)
{
	m_entities_to_add.push_back(std::make_pair(time, std::make_pair(entity_name, pos)));
}

void EntityManager::RemoveEntity(const EntityId & id) { m_entities_to_delete.push_back(id); }

bool EntityManager::HasEntity(const EntityId & id)
{
	auto entity = m_entities.find(id);
	return entity != m_entities.end();
}

bool EntityManager::HasComponent(const EntityId & id, const Component & type)
{
	if (!HasEntity(id)) return false;

	return m_entities[id].first.second.test((size_t)type);
}

bool EntityManager::AddComponent(const EntityId & id, const Component & type)
{
	if (!HasEntity(id)) return false;
	// Entity exists

	if (HasComponent(id, type)) return false;
	// Component does not exist

	auto itr2 = m_factory.find(type);
	if (itr2 == m_factory.end()) return false;
	// Component type exist

	// Add component to entity
	m_entities[id].second.emplace_back(itr2->second());
	m_entities[id].first.second.set((size_t)type, true);

	m_systemMgr->EntityModified(id, m_entities[id].first.second);
	return true;
}

bool EntityManager::RemoveComponent(const EntityId & id, const Component & type)
{
	if (!HasEntity(id)) return false;

	// Entity exist

	if (!HasComponent(id, type)) return false;
	// Component exist

	C_Base * base = GetComponent<C_Base>(id, type);
	delete base;

	m_entities[id].first.second.set((size_t)type, false);

	m_systemMgr->EntityModified(id, m_entities[id].first.second);

	return true;
}

void EntityManager::Purge()
{
	if (m_entities.empty()) return;

	for (auto & itr : m_entities)
	{
		for (auto & itr2 : itr.second.second)
		{
			delete itr2;
			itr2 = nullptr;
		}
	}
	m_entities.clear();
    m_entities_to_add.clear();
	m_id = 0;
}

bool EntityManager::ClearRemovedEntities()
{
	bool player_deleted = false;

	for (unsigned int entity : m_entities_to_delete)
	{
		auto to_delete = m_entities.find(entity);
		if (to_delete == m_entities.end()) continue;

		for (auto & component : to_delete->second.second)
		{
			delete component;
			component = nullptr;
		}
		m_entities.erase(to_delete);
		m_systemMgr->RemoveEntity(entity);

		if (entity == m_systemMgr->GetPlayerID()) player_deleted = true;
	}
	m_entities_to_delete.clear();

	return player_deleted;
}

void EntityManager::AddReservedEntities(float dt)
{
	int num = -1;
	std::vector<int> to_delete;
	for (auto & new_entity_info : m_entities_to_add)
	{
		num++;
		if (new_entity_info.first > 0.f)
		{
			new_entity_info.first -= dt;
			continue;
		}

		int id = AddEntity(new_entity_info.second.first);

		to_delete.push_back(num);

		C_Position * new_entity_pos = GetC_Position(id);

		// Position initialize
        new_entity_pos->SetPosition(new_entity_info.second.second);
        new_entity_pos->SetInitialPosition(new_entity_info.second.second);
        if (C_Collidable * col = GetC_Collidable(id))
            col->SetPosition(new_entity_info.second.second);

		// Shader initialize
		MessageHandler * handler = m_systemMgr->GetMessageHandler();
		Message msg_shader(EntityMessage::Request_Shader, id);
		handler->Dispatch(msg_shader);

        //Rigidbody initialize (projectile)
        if (new_entity_info.second.first == "Object_Plate")
        {
            Vector3f player_pos = GetC_Position(GetPlayerID())->GetPosition();
            Vector3f tower_pos = new_entity_info.second.second;
            
            C_RigidBody * rigid = GetC_Rigidbody(id);
            rigid->SetVelocity(Vector3f((player_pos.x - tower_pos.x), 256 + (player_pos.y - tower_pos.y), 0));
        }
        else if (new_entity_info.second.first == "Entity_PlateTower")
        {
            S_GameObject * game_object_system =
                m_systemMgr->GetSystem<S_GameObject>(System::GameObject);
            game_object_system->InitializeObject(id);
        }

		if (new_entity_info.second.first != "Object_Soul_purple" &&
			new_entity_info.second.first != "Object_Soul_blue")
			continue;
		// Pop effect of soul

		if (C_Collidable * col = GetC_Collidable(id))
		{
			col->SetGhost(true);
			Message msg_pop(EntityMessage::Pop, id);
			handler->Dispatch(msg_pop);
		}
	}

	for (size_t i = to_delete.size(); i > 0; i--)
		m_entities_to_add.erase(m_entities_to_add.begin() + to_delete[i-1]);
}
