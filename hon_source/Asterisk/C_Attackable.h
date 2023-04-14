#ifndef C_ATTACKABLE_H
#define C_ATTACKABLE_H
/******************************************************************************/
/*!
\headerfile   C_Attackable.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"
#include "FloatRect.h"

class C_Attackable : public C_Base
{
public:
	C_Attackable() : C_Base(Component::Attackable), m_xOffset(0.f) {}
	void ReadIn(std::stringstream & sstream)
	{
		sstream >> m_attackRange.m_top >> m_attackRange.m_left >> m_attackRange.m_height >>
			m_attackRange.m_width;
	}

	void SetTopleft(float top, float left)
	{
		m_attackRange.m_top  = top;
		m_attackRange.m_left = left;
	}

	const FloatRect & GetAttackRange() const {return m_attackRange;}

	float GetOffset() const { return m_xOffset;}
	void SetOffset(float offset) { m_xOffset = offset; }


private:
	FloatRect m_attackRange;
	float m_xOffset;

    unsigned int m_uppercut_num = 0;
};

#endif