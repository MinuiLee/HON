/******************************************************************************/
/*!
\file   ShaderSource.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/11/07

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <GL/glew.h>
#include <cassert>
#include <iostream>
#include <fstream>

#include "ShaderSource.h"

void ShaderSource::CleanInput(char* input_buffer, size_t length)
{
	// Replace invalide character with NULL.
	for (size_t i = 0; i < length; ++i)
		if (!(32 <= input_buffer[i] && input_buffer[i] <= 126))
			input_buffer[i] = 'A';
}

bool ShaderSource::CheckValidate(char input_char)
{
	bool invalid = 32 <= input_char && input_char <= 126;

	invalid = invalid || input_char == '\n';
	invalid = invalid || input_char == '\t';

	return !invalid;
}

ShaderSource::ShaderSource()
{
	type = VertexShader;
}

ShaderSource::~ShaderSource() 
{

}


ShaderSource::ShaderSource(ShaderType input_type_, std::string file_path_)
{
	std::string result;
	std::ifstream infile(file_path_, std::ios::in); // Open shaderfile.


	if (!infile.is_open()) assert(!"Error occerred during opening shader file.");


	std::string line = "";
	while (!infile.eof())
	{
		std::getline(infile, line);
		result.append(line + "\n");
	}

	infile.close();


	shader_source_ = move(result);
	type		   = input_type_;
}

ShaderSource ShaderSource::LoadShader(const ShaderType input_type, const std::string file_path)
{
	std::string shaderSource;
	std::string* result = new std::string;
	std::ifstream infile(file_path, std::ios::in); // Open shaderfile.


	if (!infile.is_open())
		assert(!"Error occerred during opening shader file.");


	std::string line = "";
	while(!infile.eof())
	{
		std::getline(infile, line);
		shaderSource.append(line + "\n");
	}

	infile.close();
	const char* a = new char[shaderSource.length() +1];
	a = shaderSource.c_str();

	*result = shaderSource;

	return ShaderSource(input_type, result->c_str());
}
