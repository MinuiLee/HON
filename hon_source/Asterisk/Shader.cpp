/******************************************************************************/
/*!
\file   Shader.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <GL/glew.h>
#include <cassert>
#include <iostream>

#include "Shader.h"

std::string Shader::PathToName(const std::string input_path)
{
	auto result = input_path;
	
	// Erase file path.
	auto end = 0;
	for(auto i : result)
	{
		if (i == '/')
		{
			++end;
			break;
		}

		++end;
	}
	result.erase(0, end);



	// Erase file extension.
	auto start = 0;
	for (auto i : result)
	{
		if (i == '.')
			break;


		++start;
	}
	result.erase(start, result.back());

	return  result;
}


size_t Shader::Size() const
{
	return m_size;
}


void Shader::CompileShader()
{
	// Create program.
	m_program = glCreateProgram();

	std::list<GLuint> shaders;

	for (const auto& current_shader : m_shader_list)
	{
		unsigned int shader = 0;

		switch (current_shader->type)
		{
		case ShaderSource::VertexShader:
			shader = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderSource::TessellationControl:
			shader = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case ShaderSource::TessellationEvaluation:
			shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case ShaderSource::Geometry:
			shader = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case ShaderSource::FragmentShader:
			shader = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		default:
			assert(!"Undefined shader type");
		}

		const GLchar * shader_source[] = { current_shader->shader_source_.c_str() };

		
		glShaderSource(shader, 1, shader_source, nullptr);
		glCompileShader(shader);


		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cout << "Error!! shader compile failed. : \n" << infoLog << std::endl;
		}


		// Attach shaders
		glAttachShader(m_program, shader);

		shaders.emplace_back(shader);
	}


	// Link program
	glLinkProgram(m_program);

	for (const auto current_shader : shaders)
		glDeleteShader(current_shader);
}



void Shader::SetUpShader(unsigned int& input_shader_program, const char* input_vertext_source, const char*  input_fragment_source)
{
	auto vertex_shader_source = input_vertext_source;
	auto fragment_shader_source = input_fragment_source;


	auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);



	int  success;
	char infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
		std::cout << "Error!! Vertex shader compile failed. : \n" << infoLog << std::endl;
	}




	auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);


	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
		std::cout << "Error!! Fragment shader compile failed. : \n" << infoLog << std::endl;
	}


	input_shader_program = glCreateProgram();
	glAttachShader(input_shader_program, vertex_shader);
	glAttachShader(input_shader_program, fragment_shader);
	glLinkProgram(input_shader_program);


	glUseProgram(input_shader_program);


	glDeleteShader(vertex_shader);   // delete shaders after make program.
	glDeleteShader(fragment_shader); // delete shaders after make program.
}


Shader::GLuint Shader::GetProgram() const
{
	return m_program; 
}