/******************************************************************************/
/*!
\file   FragmentParticle.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/07

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "FragmentParticle.h"
#include "BaseWindow.h"

#include "Mathematics.h"

constexpr glm::ivec2 VELOCITY_RANGE = glm::ivec2(1000, 1000);

FragmentParticle::FragmentParticle(unsigned int input_amount_, glm::uvec2 const & size_,
	glm::vec2 const & destination_, glm::vec2 const & velocity_, glm::vec2 const & acceleration_)
	: Particle(input_amount_, size_)
{
	m_fading	  = 0.0f * 60;
	m_init_fading = m_fading;

	SetOffsets(velocity_, acceleration_, destination_);
	UpdateBuffers(m_amount);
}

void FragmentParticle::Update(float const & dt)
{
	unsigned int count = 0;
	if (!m_isActive)
		return;
	else
		for (auto & offset_ : m_offsetData)
		{
			offset_.m_transform.m_translation.x += offset_.m_velocity.x * dt;
			offset_.m_transform.m_translation.y += offset_.m_velocity.y * dt;

			offset_.m_velocity += offset_.m_acceleration * dt;
			offset_.m_acceleration += offset_.m_init_acceleration * dt;

			if (offset_.m_transform.m_translation.y < offset_.m_destination.y) count++;
		}

	if (count >= m_amount)
	{
		m_isActive = false;

		for (auto & offset_ : m_offsetData)
		{
			offset_.m_transform = offset_.m_init_transform;
			offset_.m_velocity  = offset_.m_init_velocity;
			offset_.m_velocity.x += (rand() % VELOCITY_RANGE.x) * (rand() % 2 == 0 ? 1.0f : -1.0f);
			offset_.m_velocity.y += (rand() % VELOCITY_RANGE.y) * (rand() % 2 == 0 ? 1.0f : -1.0f);
			offset_.m_acceleration = offset_.m_init_acceleration;
		}
	}


	m_fading = m_init_fading * dt;
}

void FragmentParticle::Draw(BaseWindow * base_window_ptr)
{
	if (!m_isActive) return;

	if (!GetShader())
	{
		auto fragmentshader = base_window_ptr->GetShaders().GetShader("Fragment");
		fragmentshader->CompileShader();

		SetShader(fragmentshader);
	}

	glBindVertexArray(GetMesh().GetVAO());
	glUseProgram(GetShader()->GetProgram());


	auto translation = GetTransform().m_translation;
	// Model
	glm::mat4 model(1.f);
	model = glm::translate(model, GetTransform().m_translation);
	model = glm::rotate(model, GetTransform().m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, GetTransform().m_scale);

	// View
	glm::mat4 view(1.f);
	view = glm::translate(view, -base_window_ptr->GetView()->GetPosition()); // Camera location
	view = glm::scale(view, glm::vec3(1.f, 1.f, 1.f));

	// Combine
	glm::mat4 combined = glm::mat4(1.f) * base_window_ptr->GetProjection() * view * model;


	unsigned int combinedLoc = glGetUniformLocation(GetShader()->GetProgram(), "combined");
	glUniformMatrix4fv(combinedLoc, 1, GL_FALSE, glm::value_ptr(combined));

	unsigned int fadingLoc = glGetUniformLocation(GetShader()->GetProgram(), "fading");
	glUniform1f(fadingLoc, GetFading());


	auto texture_chunk = GetAmount() / GetTextureRotation();
	for (unsigned int i = 0; i < GetTextureRotation(); i++)
	{
		SendData((i * texture_chunk), texture_chunk);
		glBindTexture(GL_TEXTURE_2D, GetTexutre(i)->GetTextureID());
		glDrawElementsInstanced(GL_TRIANGLES, GetMesh().Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr, texture_chunk);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


Direction const& FragmentParticle::GetDirection()
{
	return m_direction_;
}

float & FragmentParticle::GetFading() { return m_fading; }

void FragmentParticle::CopyTexture(std::deque<Texture *> const & input)
{
	m_texture			   = input;
	m_num_texture_rotation = static_cast<unsigned int>(m_texture.size());
}

void FragmentParticle::SetOffsets(glm::vec2 const & velocity_, glm::vec2 const & acceleration_, glm::vec2 const & destination_)
{
	glm::vec3 translationP = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaleP	   = { 0.5f, 0.5f, 1.0f };
	float rotationP		   = 0.0f;
	float timerP		   = 0.0f;
	Color colorP(1.0f, 1.0f, 0.0f, 0.1f);

	for (unsigned int i = 0; i < m_amount; i++)
	{
		m_offsetData.push_back(
			ParticleOffset(Transform(translationP, scaleP, rotationP), colorP, timerP));

		m_offsetData[i].m_velocity = velocity_;
		m_offsetData[i].m_velocity.x += (Mathematics::GetRandomInt(0, 1000)) * (Mathematics::GetRandomInt(0, 5) > 0 ? -1 : 1);
		m_offsetData[i].m_velocity.y += (Mathematics::GetRandomInt(0, 2000)) * (Mathematics::GetRandomInt(0, 1) > 0 ? -1 : 1);

		m_offsetData[i].m_acceleration = acceleration_;

		m_offsetData[i].m_init_velocity		= m_offsetData[i].m_velocity;
		m_offsetData[i].m_init_acceleration = acceleration_;
		m_offsetData[i].m_init_torque		= m_offsetData[i].m_torque;
		m_offsetData[i].m_init_color		= m_offsetData[i].m_color;
		m_offsetData[i].m_init_transform	= m_offsetData[i].m_transform;
		m_offsetData[i].m_destination		= destination_;
	}
}