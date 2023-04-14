/******************************************************************************/
/*!
\file   DustParticle.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/07

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "BaseWindow.h"
#include "DustParticle.h"

constexpr int VELOCITY_RANGE = 20;
constexpr float TIMER_RANDOM_MIN = 5.0f;
constexpr int TIMER_RANDOM_RANGE = 16;

#undef max
#undef min

DustParticle::DustParticle(unsigned int input_amount_, float depth_, glm::uvec2 const & size_, const glm::uvec2 screen_size)
	:Particle(input_amount_, size_)
{
	glm::vec3 translationP;
	glm::vec3 scaleP = { 1.0f, 1.0f, 1.0f };
	float rotationP = 0.0f;
	float timerP = 0.0f;
	Color colorP(1.0f, 1.0f, 0.0f, 0.1f);

	depth_ += 1.0f;

	m_init_random_value[0] = std::uniform_int<int>(-static_cast<int>((screen_size.x) / 2), ((screen_size.x)/ 2));
	m_init_random_value[1] = std::uniform_int<int>(-static_cast<int>((screen_size.y) / 2), ((screen_size.y)/ 2));


	for (unsigned int i = 0; i < m_amount; i++)
	{
		scaleP.x = static_cast<float>(m_init_random_value[0](m_random_number_generator)) / m_init_random_value[0].max();
		scaleP.y = scaleP.x;

		translationP = { m_init_random_value[0](m_random_number_generator), m_init_random_value[1](m_random_number_generator), 0.0f };


		m_offsetData.push_back(ParticleOffset(Transform(translationP, scaleP, rotationP), colorP, timerP));
		m_init_scale = scaleP;
		m_offsetData[i].m_velocity = glm::vec2(1.0f, 1.0f);
		m_offsetData[i].m_velocity.x += (rand() % VELOCITY_RANGE) * ((rand() % 2) > 0 ? 1 : -1);
		m_offsetData[i].m_velocity.y += (rand() % VELOCITY_RANGE) * ((rand() % 2) > 0 ? 1 : -1);

		m_offsetData[i].m_init_velocity = m_offsetData[i].m_velocity;
		m_offsetData[i].m_init_transform = m_offsetData[i].m_transform;
		m_offsetData[i].m_timer = TIMER_RANDOM_MIN + (rand() % TIMER_RANDOM_RANGE);


		m_offsetData[i].m_init_color = Color(0.0f, 0.0f, 0.0f, 3.0f / (m_offsetData[i].m_timer));
		m_offsetData[i].m_color = Color(0.0f, 0.0f, 0.0f, -1.0f);
	}
	UpdateBuffers(m_amount);
}

void DustParticle::Update(float const & dt)
{
	for (auto& offset_ : m_offsetData)
	{
		offset_.m_transform.m_translation.x += offset_.m_velocity.x * dt;
		offset_.m_transform.m_translation.y += offset_.m_velocity.y * dt;


		offset_.m_velocity += offset_.m_acceleration * dt;
		offset_.m_acceleration += offset_.m_init_acceleration * dt;
		offset_.m_color.Alpha += (offset_.m_init_color.Alpha * dt);
		offset_.m_timer -= dt;
		
		if (offset_.m_color.Alpha >= 0.5f && offset_.m_init_color.Alpha >= 0.0f)
			offset_.m_init_color.Alpha *= -1;
		else if (offset_.m_timer <= 0.0f)
		{
			offset_.m_transform.m_scale.x = static_cast<float>(m_init_random_value[0](m_random_number_generator)) / m_init_random_value[0].max();
			offset_.m_transform.m_scale.y = offset_.m_transform.m_scale.x;
			offset_.m_transform.m_translation = { m_init_random_value[0](m_random_number_generator), m_init_random_value[1](m_random_number_generator), 0.0f };
			
			offset_.m_velocity = offset_.m_init_velocity;
			offset_.m_velocity.x += (rand() % VELOCITY_RANGE) * ((rand() % 2) > 0 ? 1 : -1);
			offset_.m_velocity.y += (rand() % VELOCITY_RANGE) * ((rand() % 2) > 0 ? 1 : -1);
			offset_.m_timer = TIMER_RANDOM_MIN + (rand() % TIMER_RANDOM_RANGE);

			offset_.m_init_color = Color(0.0f, 0.0f, 0.0f, 3.0f / offset_.m_timer);
			offset_.m_color = Color(0.0f, 0.0f, 0.0f, -1.0f);
		}
	}

		
}

void DustParticle::Draw(BaseWindow * /*base_window_ptr*/)
{
	GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(1, draw_buffers);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(GetMesh().GetVAO());
	glUseProgram(GetShader()->GetProgram());


	unsigned int projectionLoc = glGetUniformLocation(GetShader()->GetProgram(), "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(*m_projection_ptr));


	auto texture_chunck = GetAmount() / GetTextureRotation();
	const unsigned int num_texture_rotation = GetTextureRotation();
	for (unsigned int i = 0; i < num_texture_rotation; i++)
	{
		SendData((i * texture_chunck), texture_chunck);
		glBindTexture(GL_TEXTURE_2D, GetTexutre(i)->GetTextureID());
		glDrawElementsInstanced(GL_TRIANGLES, GetMesh().Get_NumOfVertices(),
			GL_UNSIGNED_INT, nullptr, texture_chunck);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	draw_buffers[0] = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, draw_buffers);
	glActiveTexture(GL_TEXTURE0);
}

void DustParticle::SetProjection(glm::mat4 * input_) { m_projection_ptr = input_; }
