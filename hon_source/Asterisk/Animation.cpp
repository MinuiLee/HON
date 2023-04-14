/******************************************************************************/
/*!
\file   Animation.cpp
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <iostream>

#include "Animation.h"
#include "SpriteSheet.h"

void Animation::Update(float dt)
{
    if (!m_play)
	return;

    m_elasedTime += dt;
    if (m_elasedTime <= m_frameTime)
	return;

    FrameStep();
    m_elasedTime = 0;
}

void Animation::FrameStep()
{
    if (!m_play)
	return;
    else if (m_frameStart == m_frameEnd)
	return;
    else if (m_frameStart < m_frameEnd)
	++m_frameCurrent;

    if (m_frameCurrent > m_frameEnd)
    {
	if (m_loop)
	    m_frameCurrent = m_frameStart;
	else
	{
	    //Decrement current frame, if not, sprite disappear
	    --m_frameCurrent;
	    Pause();
	}
    }
}

const Vector2f Animation::GetCurrentFrame() const
{
	Direction currentDirection = m_spriteSheet->GetDirection();
	int offset = 0;
	if (currentDirection == Direction::Right)
		offset = 1;
	float animation_type = m_row + static_cast<float>(offset);//(unsigned int)m_spriteSheet->GetDirection();
	return Vector2f(m_frameCurrent, animation_type);
}