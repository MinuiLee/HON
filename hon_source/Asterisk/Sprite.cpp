/******************************************************************************/
/*!
\file   Sprite.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Sprite.h"


Sprite::Sprite() : m_pShader(nullptr), m_texture(nullptr),m_size(16.f, 16.f), m_mesh(nullptr)
{
    m_transform.SetScale(glm::vec3(1.f));
}

void Sprite::BuildMesh(Texture * texture_)
{
	if(!m_mesh)
		m_mesh = new Mesh(texture_->GetSize());
}

void Sprite::SetTextureRect(const FloatRect & rect)
{
	Vector2f texture_size = m_texture->GetSize();
	m_mesh->SetTextureRect(rect, texture_size);

    m_transform.m_scale.x = rect.m_width / texture_size.x; // 1 / 10
    m_transform.m_scale.y = rect.m_height / texture_size.y; 
    m_size.x = static_cast<int>(rect.m_width);
    m_size.y = static_cast<int>(rect.m_height);
}

void Sprite::SetTexture(Texture & texture)
{
    m_texture = &texture;
    m_size = texture.GetSize();
}
