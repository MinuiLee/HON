#ifndef S_RENDERER_H
#define S_RENDERER_H
/******************************************************************************/
/*!
\headerfile   S_Renderer.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for renderer system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <vector>
#include <deque>

#include "S_Base.h"
#include "FragmentParticle.h"

#ifdef _DEBUG
class C_Collidable;
class C_Attackable;
#endif

class Window;
class C_Drawable;
class TextureManager;

class S_Renderer : public S_Base
{
public:
	// Ctor
	S_Renderer(SystemManager * sysMgr);

	// S_Base methods
	void Update(Window * window, float, bool editor_mode);

	// Observer method
	void Notify(Message & /*message*/) {}

	// S_Renderer method
	void Render(Window * wind);

	void DrawParticles(Window * wind);

	void UpdateFragmentParticleTexture(TextureManager* texMgr);

	void ActiveFragmentParticle(glm::vec3 const& position, Direction direction);

#ifdef _DEBUG
	void DebugRender(Window * wind, const C_Collidable * collision);
	void DebugRender(Window * wind, const C_Attackable * attack);
#endif

private:
	C_Drawable * GetDrawableComponent(int entityID);
	void DrawHealthBar(Window * wind, EntityId entity);

	std::deque<FragmentParticle> fragment_particles_left;
	std::deque<FragmentParticle> fragment_particles_up;
	std::deque<FragmentParticle> fragment_particles_down;
	std::deque<FragmentParticle> fragment_particles_right;
	std::deque<Texture *> fragment_texture_set;
};

#endif