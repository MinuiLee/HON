#ifndef SHADERCONTAINER_H
#define SHADERCONTAINER_H
/******************************************************************************/
/*!
\headerfile   ShaderContainer.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for shader container class.

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <unordered_map>

#include <../Asterisk/Shader.h>

class ShaderContainer 
{
	std::unordered_map<std::string, Shader> m_shader_collection;

	std::string m_config_name;

public:
	ShaderContainer(std::string file_path_);

	void ConfigContainer();

	Shader* GetShader(const std::string& key_);
};

#endif