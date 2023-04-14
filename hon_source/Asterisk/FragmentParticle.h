#ifndef FRAGMENTPARTICLE_H
#define FRAGMENTPARTICLE_H
/******************************************************************************/
/*!
\headerfile   FragmentParticle.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/07
\brief
		Header file for fragment particle class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Particle.h"
#include "Direction.h"

class BaseWindow;
class FragmentParticle : public Particle
{
	float m_fading		= 0.0f;
	float m_init_fading = 0.0f;
	Direction m_direction_ = Direction::None;

public:
	FragmentParticle() {}
	FragmentParticle(unsigned int input_amount_, glm::uvec2 const & size_,
		 glm::vec2 const & destination_,
		glm::vec2 const & velocity_, glm::vec2 const & acceleration_);
	void Update(float const & dt);
	void Draw(BaseWindow * base_window_ptr);

	void CopyTexture(std::deque<Texture *> const & input);
	

	Direction const& GetDirection();

	float & GetFading();
	void SetOffsets(glm::vec2 const & velocity_, glm::vec2 const & acceleration_,glm::vec2 const & destination_);
};

#endif