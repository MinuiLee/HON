#ifndef C_BASE_H
#define C_BASE_H
/******************************************************************************/
/*!
\headerfile   C_Base.h
\author Jooho Jeong
\par    email: jooho556\@gmail.com
\date   2018/12/14
\breif  
		Base class for component classes.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <sstream>
#include "ECS_Types.h"

class C_Base
{
public:
    C_Base(const Component & id) : m_type(id) {}
    virtual ~C_Base() {}

    const Component & GetType() const { return m_type; }

    virtual void ReadIn(std::stringstream & sstream) = 0;

    friend std::stringstream & operator>>(
	std::stringstream & sstream, C_Base & base)
    {
		base.ReadIn(sstream);
		return sstream;
    }

private:
    Component m_type;
};

#endif