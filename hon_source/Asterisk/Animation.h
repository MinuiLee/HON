#ifndef ANIMATION_H
#define ANIMATION_H
/******************************************************************************/
/*!
\headerfile   Animation.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\brief
		Header file for animation class

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <sstream>
#include "Vector.h"

class SpriteSheet;
class Animation
{
public:
	Animation(SpriteSheet * sheet, const std::string & part)
		: m_row(0), m_frameCurrent(0), m_frameStart(0), m_frameEnd(0), m_frameTime(0),
		  m_elasedTime(0), m_part(part), m_play(false), m_loop(false), m_spriteSheet(sheet)
	{
	}

	void Play() { m_play = true; }
	void Pause() { m_play = false; }
	void Stop()
	{
		m_play		   = false;
		m_frameCurrent = m_frameStart;
	}
	void SetPlay(bool play) { m_play = play; }
	bool IsPlaying() { return m_play; }
	void SetLoop(bool loop) { m_loop = loop; }
	void Reset()
	{
		m_frameCurrent = m_frameStart;
	}

	void Update(float dt);
	void FrameStep();

	friend std::stringstream & operator>>(std::stringstream & stream, Animation & anim)
	{
		stream >> anim.m_row >> anim.m_frameStart >> anim.m_frameEnd >> anim.m_frameTime;
		return stream;
	}

	const Vector2f GetCurrentFrame() const;
	unsigned int GetEndFrame() { return m_frameEnd; }
private:
	unsigned int m_row;
	unsigned int m_frameCurrent;
	unsigned int m_frameStart;
	unsigned int m_frameEnd;
	float m_frameTime;
	float m_elasedTime;
	std::string m_part;

	bool m_play;
	bool m_loop;

	SpriteSheet * m_spriteSheet;
};

#endif