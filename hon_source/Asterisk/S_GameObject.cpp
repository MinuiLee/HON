/*!*******************************************************************
\file         S_GameObject.cpp
\author       Kim Hyungseob
\par          email: hn02415 \@ gmail.com
\date         2019/01/30

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
********************************************************************/
#include "S_GameObject.h"

#include "S_Renderer.h"

#include "SystemManager.h"
#include "EntityManager.h"
#include "FontManager.h"
#include "TextureManager.h"

#include "C_GameObject.h"
#include "C_Position.h"
#include "C_Controllable.h"

#include "GameObject/Door.h"
#include "GameObject/PlateTower.h"
#include "GameObject/FragileWall.h"

#include "BaseWindow.h"

// Ctor
S_GameObject::S_GameObject(SystemManager * systemMgr) : S_Base(System::GameObject, systemMgr)
{
	Bitset bits;
	bits.set((size_t)Component::GameObject, 1);
	m_requirement.push_back(bits);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::GetAttacked, this);
	handler->Subscribe(EntityMessage::Dead, this);
	handler->Subscribe(EntityMessage::Pay_Blue_Soul, this);
	handler->Subscribe(EntityMessage::Pay_Purple_Soul, this);
}

void S_GameObject::InitializeObject(EntityId entity, int condition)
{
	EntityManager * entityMgr  = m_systemMgr->GetEntityManager();
	std::string object_type	= entityMgr->GetEntityType(entity);
	C_GameObject * game_object = entityMgr->GetC_GameObject(entity);

	if (object_type == "Entity_Door")
		std::any_cast<Door>(game_object->Get_Object())->Initialize(condition);
	else if (object_type == "Entity_PlateTower")
		std::any_cast<PlateTower>(game_object->Get_Object())->Initialize(entity, entityMgr);
}

// S_Base methods
void S_GameObject::Update(Window * window, float dt, bool /*editor_mode*/)
{
	const auto entityMgr = m_systemMgr->GetEntityManager();
	const auto player	= m_systemMgr->GetPlayerID();

	const auto c_control = entityMgr->GetC_Controllable(player);
	c_control->SetPlayerCloseToDoor(-1);

	m_DrawText = false;

	for (auto entity : m_entities)
	{
		auto gameObject = entityMgr->GetC_GameObject(entity);

		switch (gameObject->GetType())
		{
		case ObjectType::Door:
		{
			if (std::any_cast<Door>(gameObject->Get_Object())
					->IsPlayerCloseToDoor(m_systemMgr, entity, dt))
			{
				c_control->SetPlayerCloseToDoor(entity);
				m_DrawText = true;
			}
			break;
		}
		case ObjectType::PlateTower:
		{
			C_Position * entity_pos = entityMgr->GetC_Position(entity);
			if (window->GetBaseWindow()->GetViewSpace().Intersects(entity_pos->GetPosition()))
				std::any_cast<PlateTower>(gameObject->Get_Object())->Update(dt);
			break;
		}
		}
	}

	if (m_is_shake)
	{ 
		window->GetBaseWindow()->m_is_shake = true;
		m_is_shake							= false;
	}
		
}

// Observer method
void S_GameObject::Notify(Message & message)
{
	auto entityMgr  = m_systemMgr->GetEntityManager();
	auto gameObject = entityMgr->GetC_GameObject(message.m_receiver);

	switch (message.m_type)
	{
	case EntityMessage::Dead:
	case EntityMessage::GetAttacked:
	{
		auto type = gameObject->GetType();

		if (type == ObjectType::PlateTower)
			std::any_cast<PlateTower>(gameObject->Get_Object())->HandleMessage(message);
		else if (type == ObjectType::FragileWall)
		{
			std::any_cast<FragileWall>(gameObject->Get_Object())->HandleMessage(m_systemMgr, &message);

			
			// Attack direction
			switch (message.m_direction)
			{
			case Direction::Left:
				if (std::any_cast<FragileWall>(gameObject->Get_Object())->GetDirection()
					== Direction::Right)
				{
					glm::vec3& pos = entityMgr->GetC_Position(message.m_receiver)->GetPosition();
					m_systemMgr->GetSystem<S_Renderer>(System::Renderer)->ActiveFragmentParticle(pos, Direction::Right);
					m_is_shake = true;
				}
				break;

			case Direction::Right:
				if (std::any_cast<FragileWall>(gameObject->Get_Object())->GetDirection()
					== Direction::Left) 
				{
					glm::vec3& pos = entityMgr->GetC_Position(message.m_receiver)->GetPosition();
					m_systemMgr->GetSystem<S_Renderer>(System::Renderer)->ActiveFragmentParticle(pos, Direction::Left);
					m_is_shake = true;
				}
				break;

			case Direction::Up:
				if (std::any_cast<FragileWall>(gameObject->Get_Object())->GetDirection()
					== Direction::Down) 
				{
					glm::vec3& pos = entityMgr->GetC_Position(message.m_receiver)->GetPosition();
					m_systemMgr->GetSystem<S_Renderer>(System::Renderer)->ActiveFragmentParticle(pos, Direction::Down);
					m_is_shake = true;
				}
				break;

			case Direction::Down:
				if (std::any_cast<FragileWall>(gameObject->Get_Object())->GetDirection()
					== Direction::Up) 
				{
					glm::vec3& pos = entityMgr->GetC_Position(message.m_receiver)->GetPosition();
					m_systemMgr->GetSystem<S_Renderer>(System::Renderer)->ActiveFragmentParticle(pos, Direction::Up);
					m_is_shake = true;
				}
				break;
			}
		}
		break;
	}
	case EntityMessage::Pay_Purple_Soul:
	{
		if (gameObject->GetType() == ObjectType::Door)
			if (std::any_cast<Door>(gameObject->Get_Object())
					->IsSoulEnoughToOpenDoor(m_systemMgr, &message))
				m_DrawText = false;
		break;
	}
	case EntityMessage::Pay_Blue_Soul:
	{
		if (gameObject->GetType() == ObjectType::Door)
			if (std::any_cast<Door>(gameObject->Get_Object())
					->IsSoulEnoughToOpenDoor(m_systemMgr, &message))
				m_DrawText = false;
		break;
	}
	}
}

void S_GameObject::Draw(Window * wind)
{
	for (auto entity : m_entities)
	{
		auto gameObject = m_systemMgr->GetEntityManager()->GetC_GameObject(entity);

		if (gameObject->GetType() == ObjectType::Door)
			std::any_cast<Door>(gameObject->Get_Object())->DrawUI(wind);
	}

	if (m_DrawText) m_doorUI->Draw(wind);
}