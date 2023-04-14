#ifndef SPRITESHEET_H
#define SPRITESHEET_H
/******************************************************************************/
/*!
\headerfile   SpriteSheet.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for sprite sheet class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <unordered_map>

#include "Sprite.h"
#include "Direction.h"
#include "ShaderContainer.h"

#ifdef _DEBUG
class C_Collidable;
class C_Attackable;
#endif

class Animation;
class Window;
using AnimationSet = std::unordered_map<std::string, std::unordered_map<std::string, Animation *>>;
// Animation part,	 <Animation name, pointer>

using CurrentAnimations = std::unordered_map<std::string, std::pair<Sprite, Animation *>>;
// Animation part, <Sprite, pointer>

class TextureManager;
class SpriteSheet
{
	friend class Animation;

public:
	SpriteSheet(const std::string & file, TextureManager * texMgr);
	~SpriteSheet();

	void SetCurrentAnimation(
		const std::string & part, const std::string & name, bool play, bool loop);

    void SetSpritePosition(const Vector3f & pos);
    Vector2f GetSpriteSize() const { return Vector2f(m_size.x * m_scale.x, m_size.y * m_scale.y); }
    void SetSpriteScale(const std::string & part, float scale);
	bool IsPlaying(const std::string & part);

    const Direction & GetDirection() { return m_dir; }
    void SetDirection(const Direction & dir) 
    {
	    if (dir == Direction::Up || dir == Direction::Down)
		    return;
	    m_dir = dir; 
    }

    void SetDepth(float depth_);
	
    Sprite * GetSprite(const std::string & part)
    {
	    auto itr = m_currentAnims.find(part);
	    return (itr != m_currentAnims.end()) ? &itr->second.first : nullptr;
    }

    bool HasState(const std::string & state_name);

	std::string GetCurrentAnimation() { return m_currentAnimi; }

	void CropSprite(const std::string & part);
	void Update(float dt);
	void Draw(Window * wind, bool flicker);
#ifdef _DEBUG
	void DebugDraw(Window * wind, const C_Collidable * collision);
	void DebugDraw(Window * wind, const C_Attackable * attack);

#endif
	void ReinitMeshes();

	void SetShader(ShaderContainer & shader_container_, std::string shader_name_);

    bool IsDeadAnimationEnd(unsigned int entity);

private:
    void LoadSheet(const std::string & file);
    void Purge();
    const Vector2u GetMaxRowAndCol();

	std::string m_name;
	std::string m_currentAnimi = "";
	Vector2u m_size;
	Vector2f m_scale;
	Direction m_dir;

	Shader * m_shader;

	AnimationSet m_animations;
	CurrentAnimations m_currentAnims;
	TextureManager * m_texMgr;

	float depth;
};

#endif