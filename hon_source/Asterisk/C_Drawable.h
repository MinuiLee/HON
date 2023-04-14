#ifndef C_DRAWABLE_H
#define C_DRAWABLE_H
/******************************************************************************/
/*!
\headerfile   C_Drawable.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif
		Header file for drawable component class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"
#include "Vector.h"
#include "Transform.h"

class Window;
class C_Collidable;
class C_Attackable;
class C_Drawable : public C_Base
{
public:
    C_Drawable(const Component & com) : C_Base(com) {}

    //C_Base method
    virtual void ReadIn(std::stringstream & /*sstream*/) override {}

    //C_Drawable methods
    virtual void UpdateSpritePosition(const Vector3f & /*pos*/) {}
    void SetTextureManager(TextureManager * texMgr) { m_textureMgr = texMgr; }

    virtual const Vector2f GetSpriteSize() = 0;
    virtual void Draw(Window * wind, bool flicker) = 0;
#ifdef _DEBUG
    virtual void DebugDraw(Window * wind, const C_Collidable * col) = 0;
    virtual void DebugDraw(Window * wind, const C_Attackable * attack) = 0;
#endif
    virtual Transform & GetTransform() = 0;

protected:
	std::string m_texture_name;
	TextureManager * m_textureMgr;
};

#endif