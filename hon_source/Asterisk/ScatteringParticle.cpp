/******************************************************************************/
/*!
\file   ScatteringParticle.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/07

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ScatteringParticle.h"
#include "BaseWindow.h"

namespace
{
	enum
	{
		BLUE = 0,
		PURPLE
	};
}

ScatteringParticle::ScatteringParticle(unsigned int input_amount_, glm::uvec2 const & size_)
	:Particle(input_amount_, size_)
{
	glm::vec3 translationP = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaleP = { 1.0f, 1.0f, 1.0f };
	float rotationP = 0.0f;
	float timerP = 0.0f;
	Color colorP(1.0f, 1.0f, 0.0f, 0.1f);

	for (unsigned int i = 0; i < m_amount; i++)
	{
		m_offsetData.push_back(ParticleOffset(Transform(translationP, scaleP, rotationP), colorP, timerP));

		m_offsetData[i].m_velocity = glm::vec2(0.0f, 0.0f);
		m_offsetData[i].m_velocity.x += (rand() % 800) * ((rand() % 2) > 0 ? 1 : -1);
		m_offsetData[i].m_velocity.y += (rand() % 800) * ((rand() % 2) > 0 ? 1 : -1);


		m_offsetData[i].m_init_velocity = m_offsetData[i].m_velocity;
		m_offsetData[i].m_init_torque = m_offsetData[i].m_torque;
		m_offsetData[i].m_init_color = m_offsetData[i].m_color;
		m_offsetData[i].m_init_transform = m_offsetData[i].m_transform;
	}
	UpdateBuffers(m_amount);
}

void ScatteringParticle::Update(float const & dt)
{
	if (m_timer > 1.0f)
	{
		m_isActive = false;
		m_timer = 0.0f;
		return;
	}

	for (auto& offset_ : m_offsetData)
	{
		offset_.m_transform.m_translation.x += offset_.m_velocity.x * dt;
		offset_.m_transform.m_translation.y += offset_.m_velocity.y * dt;
	}
	m_transform.m_scale -= dt;

	m_timer += dt*2;
}

void ScatteringParticle::Draw(BaseWindow * base_window_ptr)
{
	if (!m_isActive) return;

	if (!GetShader())
	{
		auto fragmentshader = base_window_ptr->GetShaders().GetShader("Scattering");
		fragmentshader->CompileShader();

		SetShader(fragmentshader);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

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

	glUniform1f(glGetUniformLocation(GetShader()->GetProgram(), "timer"), m_timer);
	glUniform4fv(glGetUniformLocation(GetShader()->GetProgram(), "color"), 1, glm::value_ptr(color));

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

void ScatteringParticle::Initialize()
{
	m_isActive = true;

	glm::vec3 translationP = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scaleP = { 1.0f, 1.0f, 1.0f };
	Color colorP(1.0f, 1.0f, 0.0f, 0.1f);

	m_transform.m_scale = glm::vec3(1, 1, 1);

	for (auto & offset_ : m_offsetData)
	{
		offset_.m_transform = offset_.m_init_transform;

		offset_.m_velocity = glm::vec2(0.0f, 0.0f);
		offset_.m_velocity.x += (rand() % 800) * ((rand() % 2) > 0 ? 1 : -1);
		offset_.m_velocity.y += (rand() % 800) * ((rand() % 2) > 0 ? 1 : -1);
	}
}

void ScatteringParticle::SetColor(int input_color)
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
