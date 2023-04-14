#ifndef SHADER_H
#define SHADER_H
/******************************************************************************/
/*!
\headerfile   Shader.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for Shader class.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>
#include <list>
#include <memory>

#include "ShaderSource.h"

class Shader
{
	friend class Graphics;

	using GLuint = unsigned int;

	GLuint m_program;
	std::list<std::unique_ptr<ShaderSource>> m_shader_list;
	size_t m_size;

	static std::string PathToName(std::string input_path);
public:
	Shader() : m_program(0), m_size(0) {}

	size_t Size() const;
	
	std::list<std::unique_ptr<ShaderSource>> & GetList() { return m_shader_list; }

	GLuint GetProgram() const;
	void CompileShader();
	static void SetUpShader(unsigned int& input_shader_program, const char* input_vertext_source, const char*  input_fragment_source);
};

#endif