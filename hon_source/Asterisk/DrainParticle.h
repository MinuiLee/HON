/******************************************************************************/
/*!
\headerfile   DrainParticle.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2019/04/29
\brief
		Header file for drainig particle class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "Particle.h"

class BaseWindow;
class DrainParticle : public Particle
{
	float stack = 0.0f;
	glm::vec4 color;
	glm::vec3 view_pos;
	glm::vec2 view_size;

public:
	DrainParticle(unsigned int input_amount_, glm::uvec2 const & size_);
	void Update(float const& dt);
	void Draw(BaseWindow * base_window_ptr);
	void Initialize();
	void SetColor(int input_color);
};