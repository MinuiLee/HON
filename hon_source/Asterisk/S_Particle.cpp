/******************************************************************************/
/*!
\file   S_Particle.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/30

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Particle.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "C_Position.h"
#include "C_Status.h"

constexpr unsigned int DRAIN_PARTICLE_AMOUNT = 10;
constexpr glm::uvec2 DRAIN_PARTICLE_SIZE	 = glm::uvec2(96, 96);
constexpr unsigned int SCATTERING_PARTICLE_AMOUNT = 20;
constexpr glm::uvec2 SCATTERING_PARTICLE_SIZE	 = glm::uvec2(96, 96);

S_Particle::S_Particle(SystemManager * systemMgr)
	: S_Base(System::Particle, systemMgr),
	  soul_texture("Media\\Textures\\SoulFragment.png", false)
{
	Bitset bits;
	bits.set((size_t)Component::Position, 1);
	m_requirement.emplace_back(bits);

	MessageHandler * handler = m_systemMgr->GetMessageHandler();
	handler->Subscribe(EntityMessage::Drain_Soul, this);
	handler->Subscribe(EntityMessage::GetAttacked, this);
	handler->Subscribe(EntityMessage::Knockdown, this);
	handler->Subscribe(EntityMessage::Pay_Purple_Soul, this);
	handler->Subscribe(EntityMessage::Pay_Blue_Soul, this);
}

void S_Particle::Update(Window* /*window*/, float dt, bool /*aa*/)
{
	for (DrainParticle & particle : drain_particles)
		if (particle.IsActive()) particle.Update(dt);
	for (ScatteringParticle & particle : scattering_particles)
		if (particle.IsActive()) particle.Update(dt);
	for (DoorDrainParticle & particle : doordrain_particles)
		if (particle.IsActive()) particle.Update(dt);
}

void S_Particle::Notify(Message & message)
{
	EntityManager * entityMgr = m_systemMgr->GetEntityManager();
	
	switch (message.m_type)
	{
	case EntityMessage::Drain_Soul:
	{
		for (DrainParticle & particle : drain_particles)
			if (!particle.IsActive())
			{
				particle.TranslateTo(glm::vec3(message.m_f2D.m_x, message.m_f2D.m_y, 0.0f));
				particle.SetColor(message.int_value);
				particle.SetActive(true);
				particle.Initialize();
				return;
			}

		drain_particles.emplace_back(DRAIN_PARTICLE_AMOUNT, DRAIN_PARTICLE_SIZE);


		DrainParticle & current_particle = drain_particles.back();
		current_particle.TranslateTo(glm::vec3(message.m_f2D.m_x, message.m_f2D.m_y, 0.0f));
		current_particle.SetActive(true);
		current_particle.SetColor(message.int_value);
		current_particle.CopyTexture(soul_texture_set);


		break;
	}
	case EntityMessage::Knockdown:
	case EntityMessage::GetAttacked:
	{
		int color = 0;
		if (C_Status * status = entityMgr->GetC_Status(message.m_receiver))
			color = status->GetRecentLostSoulColor();

		if (message.m_receiver != static_cast<int>(m_playerID))
			return;
		Vector3f position = entityMgr->GetC_Position(message.m_receiver)->GetPosition();
		

		for (ScatteringParticle & particle : scattering_particles)
			if (!particle.IsActive())
			{
				particle.TranslateTo(glm::vec3(position.x, position.y, 0.0f));
				particle.SetColor(color);
				particle.SetActive(true);
				particle.Initialize();
				return;
			}

		scattering_particles.emplace_back(SCATTERING_PARTICLE_AMOUNT, SCATTERING_PARTICLE_SIZE);


		ScatteringParticle & current_particle = scattering_particles.back();
		current_particle.TranslateTo(glm::vec3(position.x, position.y, 0.0f));
		current_particle.SetActive(true);
		current_particle.SetColor(color);
		current_particle.CopyTexture(soul_texture_set);


		break;
	}
	case EntityMessage::Pay_Purple_Soul:
	case EntityMessage::Pay_Blue_Soul:
	{
		int color = 0;
		if (message.m_type == EntityMessage::Pay_Purple_Soul)
			color = 1;

		Vector3f position = entityMgr->GetC_Position(message.m_receiver)->GetPosition();
		Vector3f player_pos = entityMgr->GetC_Position(m_playerID)->GetPosition();


		for (DoorDrainParticle & particle : doordrain_particles)
			if (!particle.IsActive())
			{
				particle.TranslateTo(glm::vec3(player_pos.x, player_pos.y, 0.0f));
				particle.SetColor(color);
				particle.SetActive(true);
				particle.Initialize();
				particle.SetTarget(position.x, position.y);
				return;
			}

		doordrain_particles.emplace_back(SCATTERING_PARTICLE_AMOUNT, SCATTERING_PARTICLE_SIZE);


		DoorDrainParticle & current_particle = doordrain_particles.back();
		current_particle.TranslateTo(glm::vec3(player_pos.x, player_pos.y, 0.0f));
		current_particle.SetActive(true);
		current_particle.SetColor(color);
		current_particle.CopyTexture(soul_texture_set);
		current_particle.SetTarget(position.x, position.y);

		break;
	}
	}
}

void S_Particle::Draw(Window * window) 
{ 
	for (DrainParticle & particle : drain_particles)
		if (particle.IsActive()) 
			particle.Draw(window->GetBaseWindow());

	for (ScatteringParticle & particle : scattering_particles)
		if (particle.IsActive())
			particle.Draw(window->GetBaseWindow());

	for (DoorDrainParticle & particle : doordrain_particles)
		if (particle.IsActive())
			particle.Draw(window->GetBaseWindow());
}

void S_Particle::UpdateSoulTexture(TextureManager * /*texMgr*/)
{
	soul_texture_set.push_back(&soul_texture);

	for (DrainParticle & particle : drain_particles)
		particle.CopyTexture(soul_texture_set);

	for (ScatteringParticle & particle : scattering_particles)
		particle.CopyTexture(soul_texture_set);

	for (DoorDrainParticle & particle : doordrain_particles)
		particle.CopyTexture(soul_texture_set);
}
