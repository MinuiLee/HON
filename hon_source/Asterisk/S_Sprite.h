#ifndef S_SPRITE_H
#define S_SPRITE_H
/******************************************************************************/
/*!
\headerfile   S_Sprite.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for sprite system class.

\copyright
All content 2018 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "S_Base.h"
#include "ShaderContainer.h"

class S_Sprite : public S_Base
{
public:
    // Ctor
    S_Sprite(SystemManager * systemMgr);

    // S_Base methods
    void Initialize(ShaderContainer & input_);
    void Update(Window * /*window*/, float /*dt*/, bool /*editor_mode*/) {}
    void ReinitMeshes();

    // Observer method
    void Notify(Message & message);

    void RequestShader(ShaderContainer & input_);

private:
    ShaderContainer * m_shaders;
};

#endif