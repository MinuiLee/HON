/******************************************************************************/
/*!
\file   S_Renderer.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <algorithm>

#include "S_Renderer.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_Position.h"
#include "C_Drawable.h"
#include "C_SpriteSheet.h"
#include "C_Sprite.h"
#include "C_Status.h"
#include "Window.h"
#include "FloatRect.h"
#include "View.h"

namespace
{
	constexpr float HEALTH_RENDER_TIME = 3.f;
	constexpr int FRAGMENT_AMOUNT	  = 12;
	constexpr glm::uvec2 FRAGMENT_SIZE(100, 100);
	constexpr glm::vec2 FRAGMENT_POS(100.0f, 0.0f);
	constexpr glm::vec2 FRAGMENT_DESTINATION(0.0f, -1000.0f);
	constexpr glm::vec2 FRAGMENT_VELOCITY(0.0f, 0.0f);
	constexpr glm::vec2 FRAGMENT_ACCELERATION(10.0f, -5400.0f);

} // namespace

// Ctor
S_Renderer::S_Renderer(SystemManager * sysMgr) : S_Base(System::Renderer, sysMgr)
{
	Bitset bit;
	bit.set((size_t)Component::Position, 1);
	bit.set((size_t)Component::SpriteSheet, 1);
	m_requirement.emplace_back(bit);

	bit.reset();
	bit.set((size_t)Component::Position, 1);
	bit.set((size_t)Component::Sprite, 1);
	m_requirement.emplace_back(bit);
}

// S_Base methods
void S_Renderer::Update(Window * window, float dt, bool /*editor_mode*/)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	FloatRect view_space	  = window->GetBaseWindow()->GetUpdateCullingSpace();
	for (auto itr : m_entities)
	{
		C_Position * tran = entityMgr->GetC_Position(itr);
		C_Drawable * drawable = GetDrawableComponent(itr);

		drawable->UpdateSpritePosition(
			Vector3f(tran->GetPosition().x, tran->GetPosition().y, tran->GetPosition().z));

		if (C_SpriteSheet * sheet = dynamic_cast<C_SpriteSheet *>(drawable))
			sheet->GetSpriteSheet()->Update(dt);
		if (C_Status * status = entityMgr->GetC_Status(itr)) status->UpdateHRTimer(dt);
	}

	for (auto& current_particle : fragment_particles_up)
		current_particle.Update(dt);
	for (auto& current_particle : fragment_particles_left)
		current_particle.Update(dt);
	for (auto& current_particle : fragment_particles_down)
		current_particle.Update(dt);
	for (auto& current_particle : fragment_particles_right)
		current_particle.Update(dt);
}

// S_Renderer method
void S_Renderer::Render(Window * wind)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	for (auto itr : m_entities)
	{
		C_Position * tran	 = entityMgr->GetC_Position(itr);
		C_Drawable * drawable = GetDrawableComponent(itr);

		Vector2f spritePos;
		spritePos.x = tran->GetPosition().x;
		spritePos.y = tran->GetPosition().y;

		Vector2u spriteSize = drawable->GetSpriteSize();
		Vector2f topLeft = Vector2f(spritePos.x - spriteSize.x / 2, spritePos.y + spriteSize.y / 2);

		FloatRect rect(topLeft.x, topLeft.y, (float)spriteSize.x, (float)spriteSize.y);
		FloatRect viewSpace(wind->GetBaseWindow()->GetViewSpace());

		// Object culling
		if (viewSpace.Intersects(rect))
		{
			bool flicker = false;
			if (itr == m_playerID)
			{
				C_Status * player_status = entityMgr->GetC_Status(m_playerID);
				float invincible_timer   = player_status->GetInvincibleTimer();
				if ((static_cast<int>(invincible_timer / 0.1) % 2 != 0) && 
                    !player_status->IsInvincibleCheatEnabled()) flicker = true;
			}
			drawable->Draw(wind, flicker);

			if (entityMgr->GetEntityType(itr) == "Entity_Boss")
			{
				
			}
			else if (C_Status * status = entityMgr->GetC_Status(itr);
				status && status->GetHRTimer() < HEALTH_RENDER_TIME)
			{ 
                DrawHealthBar(wind, itr);
            } 
        }

	}
}

void S_Renderer::DrawParticles(Window * wind)
{
	for (auto & particle : fragment_particles_up)
		if (particle.IsActive())
			particle.Draw(wind->GetBaseWindow());
	for (auto & particle : fragment_particles_left)
		if (particle.IsActive())
			particle.Draw(wind->GetBaseWindow());
	for (auto & particle : fragment_particles_down)
		if (particle.IsActive())
			particle.Draw(wind->GetBaseWindow());
	for (auto & particle : fragment_particles_right)
		if (particle.IsActive())
			particle.Draw(wind->GetBaseWindow());
}

void S_Renderer::UpdateFragmentParticleTexture(TextureManager * texMgr)
{
	Texture * doll0 = texMgr->GetResource("doll0");
	Texture * doll1 = texMgr->GetResource("doll1");
	Texture * doll2 = texMgr->GetResource("doll2");
	Texture * doll3 = texMgr->GetResource("doll3");
	Texture * doll4 = texMgr->GetResource("doll4");
	fragment_texture_set.push_back(doll0);
	fragment_texture_set.push_back(doll1);
	fragment_texture_set.push_back(doll2);
	fragment_texture_set.push_back(doll3);
	fragment_texture_set.push_back(doll4);
}

void S_Renderer::ActiveFragmentParticle(glm::vec3 const & position, Direction direction)
{ 
	std::deque<FragmentParticle>* current_container = nullptr;
	
	switch(direction)
	{
	case Direction::Up:
		current_container = &fragment_particles_up;
		break;
	case Direction::Left:
		current_container = &fragment_particles_left;
		break;
	case Direction::Down:
		current_container = &fragment_particles_down;
		break;
	case Direction::Right:
		current_container = &fragment_particles_right;
		break;
	}

	for (auto& particle : *current_container)
	{
		if (!particle.IsActive())
		{ 
			particle.TranslateTo(position);
			particle.SetActive(true);
			return;
		}
	} 

	glm::vec2 velocity = FRAGMENT_VELOCITY;
	glm::vec2 acceleration = FRAGMENT_ACCELERATION;
	switch(direction)
	{
	case Direction::Up:
		velocity.x = 0.0f;
		acceleration.x = 0.0f;
		break;
	case Direction::Left:
		velocity.x *= -1.0f;
		velocity.y = 0.0f;
		break;
	case Direction::Down:
		velocity.x = 0.0f;
		velocity.y *= -1.0f;
		acceleration.x = 0.0f;
		break;
	case Direction::Right:
		velocity.y = 0.0f;
		acceleration.x *= -1.0f;
		break;
	}


	
	current_container->emplace_back(FRAGMENT_AMOUNT, FRAGMENT_SIZE,
																  FRAGMENT_DESTINATION, velocity,
																  FRAGMENT_ACCELERATION);


	FragmentParticle & current_particle = current_container->back();
	current_particle.TranslateTo(position);
	current_particle.SetActive(true);
	current_particle.CopyTexture(fragment_texture_set);
}

#ifdef _DEBUG
void S_Renderer::DebugRender(Window * wind, const C_Collidable * collision)
{
	for (auto itr : m_entities)
	{
		C_Position * tran =
			m_systemMgr->GetEntityManager()->GetComponent<C_Position>(itr, Component::Position);
		C_Drawable * drawable = GetDrawableComponent(itr);

		Vector2f spritePos;
		spritePos.x = tran->GetPosition().x;
		spritePos.y = tran->GetPosition().y;

		Vector2u spriteSize = drawable->GetSpriteSize();
		Vector2f topLeft = Vector2f(spritePos.x - spriteSize.x / 2, spritePos.y - spriteSize.y / 2);

		FloatRect rect(topLeft.x, topLeft.y, (float)spriteSize.x, (float)spriteSize.y);

		FloatRect viewSpace(wind->GetBaseWindow()->GetViewSpace());

		// Object culling
		drawable->DebugDraw(wind, collision);
	}
}

void S_Renderer::DebugRender(Window * wind, const C_Attackable * attack)
{
	for (auto itr : m_entities)
	{
		C_Position * tran =
			m_systemMgr->GetEntityManager()->GetComponent<C_Position>(itr, Component::Position);
		C_Drawable * drawable = GetDrawableComponent(itr);

		Vector2f spritePos;
		spritePos.x = tran->GetPosition().x;
		spritePos.y = tran->GetPosition().y;

		Vector2u spriteSize = drawable->GetSpriteSize();
		Vector2f topLeft = Vector2f(spritePos.x - spriteSize.x / 2, spritePos.y - spriteSize.y / 2);

		FloatRect rect(topLeft.x, topLeft.y, (float)spriteSize.x, (float)spriteSize.y);

		FloatRect viewSpace(wind->GetBaseWindow()->GetViewSpace());

		// Object culling
		drawable->DebugDraw(wind, attack);
	}
}
#endif

C_Drawable * S_Renderer::GetDrawableComponent(int entityID)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();

	C_Drawable * drawable = entityMgr->GetC_Sprite(entityID);
	if (!drawable) drawable = entityMgr->GetC_SpriteSheet(entityID);
	return drawable;
}

void S_Renderer::DrawHealthBar(Window * wind, EntityId entity)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	// Health bar
	if (C_Status * status = entityMgr->GetC_Status(entity);
		status && status->GetHealth() != status->GetMaxHealth())
	{
		C_Position * pos	  = entityMgr->GetC_Position(entity);
		C_Drawable * drawable = GetDrawableComponent(entity);

		Sprite & health = status->GetHealthBar();
		Vector2f health_pos(
			pos->GetPosition().x, pos->GetPosition().y + drawable->GetSpriteSize().y / 2);
		health.SetScale(Vector2f(
			static_cast<float>(status->GetHealth()) / static_cast<float>(status->GetMaxHealth()),
			1));
		health.SetPosition(health_pos);

		wind->GetBaseWindow()->Draw(health);
	}
}
