#ifndef S_PARTICLE_H
#define S_PARTICLE_H
/******************************************************************************/
/*!
\headerfile   S_Particle.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/4/29
\brief
		Header file for particle system class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <deque>

#include "S_Base.h"
#include "DrainParticle.h"
#include "ScatteringParticle.h"
#include "DoorDrainParticle.h"
#include "TextureManager.h"


class S_Particle : public S_Base
{
	std::deque<DrainParticle> drain_particles;
	std::deque<ScatteringParticle> scattering_particles;
	std::deque<DoorDrainParticle> doordrain_particles;
	Texture soul_texture;

	std::deque<Texture *> soul_texture_set;

public:
	// Ctor
	S_Particle(SystemManager * systemMgr);

	// S_Base method
	void Update(Window * window, float, bool);

	// Observer method
	void Notify(Message &);

	void Draw(Window * window);

	void UpdateSoulTexture(TextureManager * texMgr);
};

#endif