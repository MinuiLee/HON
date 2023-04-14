#ifndef DUSTPARTICLE_H
#define DUSTPARTICLE_H
/******************************************************************************/
/*!
\headerfile   DustParticle.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/07
\brief
		Header file for dust particle class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Particle.h"

class BaseWindow;
class DustParticle : public Particle
{
	std::uniform_int<int> m_init_random_value[2];
	glm::vec2 m_init_scale;
	glm::mat4* m_projection_ptr;

public:
	DustParticle(unsigned int input_amount_, float depth_, glm::uvec2 const & size_, const glm::uvec2 screen_size);
	void Update(float const& dt);
	void Draw(BaseWindow* base_window_ptr);

	void SetProjection(glm::mat4 * input_);
};

#endif