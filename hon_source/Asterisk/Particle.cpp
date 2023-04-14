/******************************************************************************/
/*!
\file   Particle.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/01/10

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Particle.h"

Particle::Particle(const Particle & input)
	: m_mesh(input.m_mesh), m_texture(input.m_texture),
	  m_num_texture_rotation(input.m_num_texture_rotation), m_shader(input.m_shader),
	  m_transform(input.m_transform), m_offsetData(input.m_offsetData), m_offset(input.m_offset),
	  m_amount(input.m_amount), m_random_seed(), m_random_number_generator(m_random_seed())
{
}
Particle::Particle(unsigned int input_amount_, glm::uvec2 const & size_)
	: m_random_number_generator(m_random_seed())
{
	BuildMesh(size_);
	SetAmount(input_amount_);
}
Particle::~Particle()
{
	glDeleteBuffers(1, &m_offset); }
void Particle::CopyTexture(std::deque<Texture *> const & input) 
{
	m_texture			   = input;
	m_num_texture_rotation = static_cast<unsigned int>(m_texture.size());
}

void Particle::BuildMesh(glm::uvec2 const & size_)
{
	m_mesh.Square(static_cast<float>(size_.x), static_cast<float>(size_.y));
}

bool Particle::IsActive()
{
	return m_isActive; }

void Particle::SetActive(bool input)
{
	m_isActive = input;
}

void Particle::BuildMeshFromTexture(Texture * texture_)
{
	m_texture.push_back(texture_);
	m_mesh.Square(static_cast<float>(texture_->GetSize().x), static_cast<float>(texture_->GetSize().y));
}

void Particle::SetShader(Shader * shader_)
{
	m_shader = shader_;
}

Shader const* Particle::GetShader()
{
	return m_shader;
}

void Particle::UpdateBuffers(unsigned int amount_)
{
	m_mesh.Update_Buffers();

	glDeleteBuffers(1, &m_offset);
	glGenBuffers(1, &m_offset);
	glBindBuffer(GL_ARRAY_BUFFER, m_offset);
	glBufferData(GL_ARRAY_BUFFER, amount_ * sizeof(ParticleOffset), &m_offsetData[0], GL_STATIC_DRAW);
	
	m_amount = amount_;


	size_t attribute_index = 0;
	

	// Transform::Translation
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleOffset), reinterpret_cast<void *>(attribute_index));
	
	attribute_index += sizeof(glm::vec3);

	// Transform::Scale
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleOffset), reinterpret_cast<void *>(attribute_index));

	attribute_index += sizeof(glm::vec3);

	// Transform::Rotation
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleOffset), reinterpret_cast<void *>(attribute_index));

	attribute_index += sizeof(float);

	// Color
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleOffset), reinterpret_cast<void *>(attribute_index));

	attribute_index += sizeof(Color);

	// Timer
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleOffset), reinterpret_cast<void *>(attribute_index));/**/
	

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1); /**/
}

void Particle::SendData(unsigned int amount_)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_offset);
	glBufferData(GL_ARRAY_BUFFER, amount_ * sizeof(ParticleOffset), &m_offsetData[0], GL_STATIC_DRAW);
}

void Particle::SendData(unsigned int begin_, unsigned int amount_)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_offset);
	glBufferData(GL_ARRAY_BUFFER, amount_ * sizeof(ParticleOffset), &m_offsetData[begin_], GL_STATIC_DRAW);
}

ParticleOffset * Particle::GetParticleOffsetPtr(unsigned int index_)
{
	return &m_offsetData[index_];
}

const Mesh & Particle::GetMesh()
{
	return m_mesh; 
}

Transform const & Particle::GetTransform()
{
	return m_transform;
}

void Particle::PushBack(Texture* input_texture_)
{
	m_texture.push_back(input_texture_);
	m_num_texture_rotation++;
}

Texture * Particle::GetTexutre(unsigned int index_)
{
	return m_texture[index_];
}

Texture* Particle::GetRandomTexture()
{
	return m_texture[rand() % m_texture.size()];
}

unsigned int const& Particle::GetAmount()
{
	return m_amount;
}

void Particle::SetAmount(unsigned int input_amount_)
{
	m_amount = input_amount_;
}

unsigned int const& Particle::GetTextureRotation()
{
	return m_num_texture_rotation;
}

unsigned int Particle::GetNumOfTextures()
{
	return static_cast<unsigned int>(m_texture.size());
}

void Particle::TranslateAbout(float x, float y, float z)
{
	m_transform.m_translation.x += x;
	m_transform.m_translation.y += y;
	m_transform.m_translation.z += z;
}

void Particle::TranslateTo(glm::vec3 const & translate) 
{ m_transform.m_translation = translate; }

void Particle::ClearTexture() 
{ 
	m_num_texture_rotation = 0;
	m_texture.clear();
}
