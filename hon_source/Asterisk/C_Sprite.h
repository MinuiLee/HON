#ifndef C_SPRITE_H
#define C_SPRITE_H
/******************************************************************************/
/*!
\headerfile   C_Sprite.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for sprite component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Drawable.h"
#include "Vector.h"
#include "Window.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "MessageHandler.h"

class C_Sprite : public C_Drawable
{
public:
	C_Sprite() : C_Drawable(Component::Sprite) {}
	~C_Sprite()
	{
		m_sprite.DeleteMesh();
		if (m_texture_name != "") m_textureMgr->ReleaseResource(m_texture_name);
	}

	// C_Base method
	void ReadIn(std::stringstream & sstream) override { sstream >> m_texture_name; }

	// C_Drawable methods
	void UpdateSpritePosition(const Vector3f & pos) override
	{
		m_sprite.SetPosition(Vector3f(pos.x, pos.y, pos.z));
	}
	void Draw(Window * wind, bool /*flicker*/) override { wind->GetBaseWindow()->Draw(m_sprite); }
#ifdef _DEBUG
	void DebugDraw(Window * wind, const C_Collidable * col)
	{
		wind->GetBaseWindow()->DebugDraw(m_sprite, col);
	}
	void DebugDraw(Window * wind, const C_Attackable * attack)
	{
		wind->GetBaseWindow()->DebugDraw(m_sprite, attack);
	}
#endif
	void TextureSetting()
	{
		if (!m_textureMgr || m_texture_name == "") return;

		m_textureMgr->RequireResource(m_texture_name);
		Texture * tex = m_textureMgr->GetResource(m_texture_name);
		m_sprite.SetTexture(*tex);
		m_sprite.BuildMesh(tex);
	}

	const Vector2f GetSpriteSize() override { return m_sprite.GetSize(); }

	Transform & GetTransform() { return m_sprite.GetTransform(); }

    void SetShader(ShaderContainer & shader_container_, std::string shader_name_)
    {
        m_sprite.SetShader(shader_container_.GetShader(shader_name_));
    }

	Sprite const& GetSprite() const { return m_sprite; }
    Sprite& GetSprite() { return m_sprite; }

private:
	Sprite m_sprite;
};

#endif