#ifndef S_SPRITESHEET_H
#define S_SPRITESHEET_H
/******************************************************************************/
/*!
\headerfile   S_SpriteSheet.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for sprite sheet system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"
#include "ShaderContainer.h"

class S_SpriteSheet : public S_Base
{
public:
	// Ctor
	S_SpriteSheet(SystemManager * systemMgr);

	// S_Base methods
	void Initialize(ShaderContainer & input_);
	void Update(Window * /*window*/, float /*dt*/, bool /*editor_mode*/) {}

	// Observer method
	void Notify(Message & message);

	void RequestShader(ShaderContainer & input_);

private:
	ShaderContainer * m_shaders;
};

#endif