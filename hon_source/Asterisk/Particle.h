#ifndef PARTICLE_H
#define PARTICLE_H
/******************************************************************************/
/*!
\headerfile   Particle.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/01/10
\brief
		Header file for particle class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <random>
#include <deque>

#include "Transform.h" // Transform
#include "Color.h"	 // Color
#include "Texture.h"   // Mesh, texture

// Vertex attribute offset for particle.
struct ParticleOffset
{
	ParticleOffset() {}
	ParticleOffset(Transform transform_, Color color_, float timer_)
		: m_transform(transform_), m_color(color_), m_timer(timer_), m_init_transform(transform_),
		  m_init_color(color_)
	{
	}

	Transform m_transform;
	Color m_color;
	float m_timer;

	bool m_switch = false;

	Color m_init_color;

	Transform m_init_transform;
	glm::vec2 m_init_velocity	 = glm::vec2(0.0f);
	glm::vec2 m_init_acceleration = glm::vec2(0.0f);
	glm::vec2 m_init_torque		  = glm::vec2(0.0f, 0.0f);

	glm::vec2 m_destination;
	glm::vec2 m_velocity	 = glm::vec2(0.0f);
	glm::vec2 m_acceleration = glm::vec2(0.0f);
	glm::vec2 m_torque		 = glm::vec2(0.0f, 0.0f);
};

class Particle
{
	friend class FragmentParticle;
	friend class DustParticle;
	friend class ScatteringParticle;
	friend class DrainParticle;
	friend class DoorDrainParticle;
	Mesh m_mesh;
	std::deque<Texture *> m_texture;
	unsigned int m_num_texture_rotation = 0;
	Shader * m_shader = nullptr;
	Transform m_transform;

	std::vector<ParticleOffset> m_offsetData;

	GLuint m_offset = 0;

	unsigned int m_amount = 0;
	bool m_isActive		  = false;
	std::random_device m_random_seed;
	std::mt19937 m_random_number_generator;

public:
	Particle() {}
	Particle(const Particle & input);
	Particle(unsigned int input_amount_, glm::uvec2 const & size_);
	~Particle();
	void CopyTexture(std::deque<Texture *> const & input);

	void BuildMesh(glm::uvec2 const & size_);

	bool IsActive();
	void SetActive(bool input);

	void BuildMeshFromTexture(Texture * texture_);
	void SetShader(Shader * shader_);
	Shader const * GetShader();
	void UpdateBuffers(unsigned int amount_);
	void SendData(unsigned int amount_);
	void SendData(unsigned int begin_, unsigned int amount_);
	const Mesh & GetMesh();
	Transform const & GetTransform();
	void PushBack(Texture * input_texture_);
	Texture * GetTexutre(unsigned int index_);
	Texture * GetRandomTexture();

	unsigned int const & GetAmount();
	void SetAmount(unsigned int input_amount_);

	virtual void Update(float const & dt) = 0;

	ParticleOffset * GetParticleOffsetPtr(unsigned int index_);
	unsigned int const & GetTextureRotation();
	unsigned int GetNumOfTextures();
	void TranslateAbout(float x, float y, float z);
	void TranslateTo(glm::vec3 const & translate);
	void ClearTexture();
};

#endif