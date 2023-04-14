#ifndef C_SHADER_H
#define C_SHADER_H
/******************************************************************************/
/*!
\headerfile   C_Shader.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\breif
		Header file for shader component class.

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "C_Base.h"

class C_Shader : public C_Base
{
public:
	// Ctor
	C_Shader() : C_Base(Component::Shader) {}

	// C_Base method
	void ReadIn(std::stringstream & sstream)
	{ 
		sstream >> shaderName;
	}

	std::string GetName() { return shaderName; }

	
private:
	std::string shaderName;
};

#endif