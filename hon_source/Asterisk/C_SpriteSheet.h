#ifndef C_SPRITESHEET_H
#define C_SPRITESHEET_H
/******************************************************************************/
/*!
\headerfile   C_SpriteSheet.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for sprite sheet component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Drawable.h"
#include "SpriteSheet.h"
#include "TextureManager.h"

#ifdef _DEBUG
class C_Collidable;
class C_Attackable;
#endif
class C_SpriteSheet : public C_Drawable
{
public:
	// Ctor & Dtor
	C_SpriteSheet() : C_Drawable(Component::SpriteSheet), m_sheet(nullptr) {}

	~C_SpriteSheet()
	{
		if (m_sheet) delete m_sheet;
		m_sheet = nullptr;
	}

	// C_Base method
	void ReadIn(std::stringstream & sstream) override { sstream >> m_texture_name; }

	// C_Drawable methods
	void UpdateSpritePosition(const Vector3f & pos)
	{
		if (m_sheet) m_sheet->SetSpritePosition(pos);
	}

	const Vector2f GetSpriteSize() override { return m_sheet->GetSpriteSize(); }

	void Draw(Window * wind, bool flicker)
	{
		if (m_sheet) m_sheet->Draw(wind, flicker);
	}
#ifdef _DEBUG
	void DebugDraw(Window * wind, const C_Collidable * collision)
	{
		if (m_sheet) 
				m_sheet->DebugDraw(wind, collision);
	}


	void DebugDraw(Window * wind, const C_Attackable * attack)
	{
		if (m_sheet)
				m_sheet->DebugDraw(wind, attack);
	}
#endif
	// C_SpriteSheet methods
	SpriteSheet * GetSpriteSheet() { return m_sheet; }
	void SetTextureManager(TextureManager * texMgr) { m_textureMgr = texMgr; }
	void SetSpriteSheet()
	{
		if (!m_sheet && m_texture_name != "" && m_textureMgr)
			m_sheet = new SpriteSheet(m_texture_name, m_textureMgr);
	}

	void SetName(const std::string name) { m_sheetName = name; }
	Transform & GetTransform()
	{
		return m_sheet->GetSprite("Body")->GetTransform();
	}

	void SetShader(ShaderContainer & shader_container_, std::string shader_name_) 
	{ 
		m_sheet->SetShader(shader_container_,shader_name_);
	}

private:
	std::string m_sheetName;
	SpriteSheet * m_sheet;
};

#endif