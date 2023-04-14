/******************************************************************************/
/*!
\file   DoorDrainParticle.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/29

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "DoorDrainParticle.h"
#include "BaseWindow.h"

#include "Mathematics.h"

namespace
{
	enum
	{
		BLUE = 0,
		PURPLE
	};
}
constexpr glm::vec2 VELROCITY_RANGE = glm::vec2(50.0f, 50.0f);

constexpr float ACCELERATION_AMOUNT		   = 2.0f;
constexpr float SCATTERING_DURATION		   = 1.0f / ACCELERATION_AMOUNT;
constexpr float STACK_ACCELRATION_MODIFIER = 0.035f;

DoorDrainParticle::DoorDrainParticle(unsigned int input_amount_, glm::uvec2 const & size_)
	: Particle(input_amount_, size_)
{
	glm::vec3 translationP = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaleP	   = { 1.0f, 1.0f, 1.0f };
	float rotationP		   = 0.0f;
	float timerP		   = 0.0f;
	Color colorP(1.0f, 1.0f, 0.0f, 0.1f);

	stack = 0.0f;

	for (unsigned int i = 0; i < m_amount; i++)
	{
		m_offsetData.push_back(
			ParticleOffset(Transform(translationP, scaleP, rotationP), colorP, timerP));
		m_offsetData[i].m_switch = false;

		m_offsetData[i].m_velocity = glm::vec2(0.0f, 0.0f);
		m_offsetData[i].m_velocity.x += static_cast<float>(cos(i) * (rand() % 800));
		m_offsetData[i].m_velocity.y += static_cast<float>(sin(i) * (rand() % 800));

		m_offsetData[i].m_color.Red = static_cast<float>(Mathematics::GetRandomInt(2, 7)); // Use red channel for random interval.
		m_offsetData[i].m_color.Blue = 0.0f; // Use blue channel as timer.

		m_offsetData[i].m_acceleration = m_offsetData[i].m_velocity * m_offsetData[i].m_color.Red;

		m_offsetData[i].m_init_velocity		= m_offsetData[i].m_velocity;
		m_offsetData[i].m_init_acceleration = m_offsetData[i].m_acceleration;
		m_offsetData[i].m_init_color		= m_offsetData[i].m_color;
		m_offsetData[i].m_init_transform	= m_offsetData[i].m_transform;
	}
	UpdateBuffers(m_amount);
}

void DoorDrainParticle::Update(float const & dt)
{
	m_isActive = false;
	stack += dt;
	for (auto & offset_ : m_offsetData)
	{
		if (offset_.m_color.Blue >= 0.7)
			offset_.m_switch = false;
		else if (offset_.m_switch)
		{
			m_isActive = true;
			glm::vec2 destination(target_pos.x, target_pos.y);


			destination.x -= m_transform.m_translation.x;
			destination.y -= m_transform.m_translation.y;
			

			offset_.m_transform.m_translation.x +=
				(destination.x - offset_.m_transform.m_translation.x) * offset_.m_color.Blue;
			offset_.m_transform.m_translation.y +=
				(destination.y - offset_.m_transform.m_translation.y) * offset_.m_color.Blue;


			offset_.m_color.Blue += dt;
		}
		else if (offset_.m_timer >= (1.0f / offset_.m_color.Red))
		{
			offset_.m_switch = true;
		}
		else
		{
			m_isActive = true;
			offset_.m_transform.m_translation.x += offset_.m_velocity.x * dt;
			offset_.m_transform.m_translation.y += offset_.m_velocity.y * dt;

			offset_.m_velocity -= offset_.m_acceleration * dt;


			offset_.m_timer += dt;
		}
	}
}

void DoorDrainParticle::Draw(BaseWindow * base_window_ptr)
{
	view_pos  = base_window_ptr->GetView()->GetCenter();
	view_size = base_window_ptr->GetView()->GetSize();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


	if (!GetShader())
	{
		auto drain_shader = base_window_ptr->GetShaders().GetShader("DoorDrain");
		drain_shader->CompileShader();

		SetShader(drain_shader);
	}

	glBindVertexArray(GetMesh().GetVAO());
	glUseProgram(GetShader()->GetProgram());


	glm::vec3 translation = GetTransform().m_translation;


	// Model
	glm::mat4 model(1.f);
	model = glm::translate(model, translation);
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

	unsigned int colorLoc = glGetUniformLocation(GetShader()->GetProgram(), "color");
	glUniform4fv(colorLoc, 1, glm::value_ptr(color));

	glUniform1f(glGetUniformLocation(GetShader()->GetProgram(), "stack"), stack);

	auto texture_chunk = GetAmount() / GetTextureRotation();
	for (unsigned int i = 0; i < GetTextureRotation(); i++)
	{
		SendData((i * texture_chunk), texture_chunk);
		glBindTexture(GL_TEXTURE_2D, GetTexutre(i)->GetTextureID());
		glDrawElementsInstanced(
			GL_TRIANGLES, GetMesh().Get_NumOfVertices(), GL_UNSIGNED_INT, nullptr, texture_chunk);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(0.0f);
	glDepthFunc(GL_GEQUAL);

	glEnable(GL_CULL_FACE);
}

void DoorDrainParticle::Initialize()
{ 
	glm::vec3 translationP = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaleP	   = { 1.0f, 1.0f, 1.0f };
	float rotationP		   = 0.0f;

	stack = 0.0f;
	int i = 0;
	for (auto & offset_ : m_offsetData)
	{
		offset_.m_switch	   = false;
		offset_.m_timer		   = 0.0f;


		offset_.m_transform = Transform(translationP, scaleP, rotationP);

		offset_.m_velocity = glm::vec2(0.0f, 0.0f);
		offset_.m_velocity.x += static_cast<float>(cos(i++) * (rand() % 800));
		offset_.m_velocity.y += static_cast<float>(sin(i++) * (rand() % 800));

		offset_.m_color.Red = static_cast<float>(Mathematics::GetRandomInt(2, 7)); // Use red channel for random interval.
		offset_.m_color.Blue   = 0.0f;
		offset_.m_acceleration = offset_.m_velocity * offset_.m_color.Red;
	}
}

void DoorDrainParticle::SetColor(int input_color)
{
	if (input_color == BLUE)
	{
		color.r = 0.3f;
		color.g = 1.0f;
		color.b = 0.7f;
		color.a = 1.0f;
	}
	else
	{
		color.r = 0.7f;
		color.g = 0.3f;
		color.b = 1.0f;
		color.a = 1.0f;
	}
}

void DoorDrainParticle::SetTarget(float x, float y)
{
	target_pos.x = x-1.5f; target_pos.y = y+16.5f;
}
