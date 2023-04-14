#ifndef SPRITE_H
#define SPRITE_H
/******************************************************************************/
/*!
\headerfile   Sprite.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for sprite class.

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Transform.h"
#include "Shader.h"
#include "Texture.h"
#include "FloatRect.h"

class Sprite
{
public:
	Sprite();

	void BuildMesh(Texture * texture_);
	void DeleteMesh()
	{
		if (m_mesh)
		{
			delete m_mesh;
			m_mesh = nullptr;
		}
	}

	void Move(float input_x, float input_y)
	{
		m_transform.m_translation.x += input_x;
		m_transform.m_translation.y += input_y;
	}

	void Move(Vector2f input)
	{
		m_transform.m_translation.x += input.x;
		m_transform.m_translation.y += input.y;
	}

	void SetPosition(float input_x, float input_y, float input_z)
	{
		m_transform.m_translation.x = input_x;
		m_transform.m_translation.y = input_y;
		m_transform.m_translation.z = input_z;
		m_depth						= input_z;
	}

	void SetPosition(float input_x, float input_y)
	{
		m_transform.m_translation.x = input_x;
		m_transform.m_translation.y = input_y;
	}

	void SetPosition(const Vector3f & pos) { m_transform.m_translation = pos; }
	void SetPosition(const Vector2f & pos) { m_transform.m_translation.x = pos.x;
											 m_transform.m_translation.y = pos.y;}

	const Transform & GetTransform() const { return m_transform; }
	Transform & GetTransform() { return m_transform; }

	
	const Vector3f & GetPosition() const { return m_transform.m_translation; }

	void SetScale(const Vector3f & input) { m_transform.m_scale = input; }
	void SetScale(const Vector2f & input)
	{
		m_transform.m_scale.x = input.x;
		m_transform.m_scale.y = input.y;
	}
	const Vector3f & GetScale() const { return m_transform.m_scale; }
	glm::vec3 & GetScale() { return m_transform.m_scale; }
	void SetScale(float x, float y)
	{
		m_transform.m_scale.x = x;
		m_transform.m_scale.y = y;
	}

	void SetSize(const Vector2u & size) { m_size = size; }
	void SetSize(int x, int y)
	{
		m_size.x = x;
		m_size.y = y;
	}
	Vector2f GetSize() const  { return Vector2f(m_size.x / m_transform.m_scale.x, m_size.y / m_transform.m_scale.y); }

	void SetRotation(float angle) { m_transform.m_rotation = glm::radians(angle); }
	float GetRotation() const { return m_transform.m_rotation; }


	Shader * GetShader() const { return m_pShader; }
	void SetShader(Shader * input_shader) 
	{ 
		m_pShader = input_shader; 
	}


	Texture * GetTexture() const { return m_texture; }
	void SetTexture(Texture & texture);
	void SetTextureRect(const FloatRect & rect);

	Mesh * GetMesh() { return m_mesh; }
	const Mesh * GetMesh() const { return m_mesh; }
	void SetMesh(Mesh* mesh) { m_mesh = mesh; }

	
	bool const& IsShadow() const {return m_isShadow;}
	void SetShadowVisibility(const bool input_) {m_isShadow = input_;}

	float const& GetDepth() { return m_depth; }
	void SetDepth(float input_) { m_depth = input_; }

	glm::mat4& GetMatrix() { return m_combined; }
	float const* GetMatrixValue() const { return value_ptr(m_combined); }
	void SetMatrix(glm::mat4 const& input_) { m_combined = input_; }
	glm::mat4& GetModel() { return m_model; }
	void SetModel(glm::mat4& input_) { m_model = input_; }
	glm::mat4& GetView() { return m_view; }
	void SetView(glm::mat4& input_) { m_view = input_; }


private:
	
	Transform m_transform;
	Shader * m_pShader;
	Texture * m_texture;
	Vector2i m_size;
	Mesh * m_mesh;

	float m_depth = 0.f;


	bool m_isShadow = false;
	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_combined;
};

#endif