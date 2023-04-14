/******************************************************************************/
/*!
\file   ShaderContainer.cpp
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content 2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <fstream>
#include <cassert>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <iostream>

#include "ShaderContainer.h"
#include "ShaderSource.h"


#define ROW_LIMIT 65536
#define ARRAY_NAME "SHADERS"

ShaderContainer::ShaderContainer(std::string file_name_) : m_config_name(file_name_)
{
}

void ShaderContainer::ConfigContainer() 
{
	rapidjson::Document shaderConfig;

	FILE * fp			 = nullptr;
	std::string filename = "Shader/" + m_config_name + ".json";

	errno_t err = fopen_s(&fp, filename.c_str(), "rb");
	if (err != 0) return;

	char readBuffer[ROW_LIMIT];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	shaderConfig.ParseStream(is);

	const rapidjson::Value & Entities = shaderConfig[ARRAY_NAME];
	assert(Entities.IsArray());


	for (rapidjson::SizeType i = 0; i < Entities.Size(); i++)
	{
		auto shaderSource = Entities[i].GetObject();
		Shader shader;
		std::string name;
		std::string vertexSourcePath;
		std::string tessControlSourcePath;
		std::string tessEvalSourcePath;
		std::string geometrySourcePath;
		std::string fragmentSourcePath;

		if (shaderSource.HasMember("Name"))
			name = shaderSource.FindMember("Name")->value.GetString();
		if (shaderSource.HasMember("Vertex")) 
			vertexSourcePath = shaderSource.FindMember("Vertex")->value.GetString();
		if (shaderSource.HasMember("TessControl"))
			tessControlSourcePath = shaderSource.FindMember("TessControl")->value.GetString();
		if (shaderSource.HasMember("TessEval"))
			tessEvalSourcePath = shaderSource.FindMember("TessEval")->value.GetString();
		if (shaderSource.HasMember("Geometry"))
			geometrySourcePath = shaderSource.FindMember("Geometry")->value.GetString();
		if (shaderSource.HasMember("Fragment"))
			fragmentSourcePath = shaderSource.FindMember("Fragment")->value.GetString();

		
		if (strcmp(vertexSourcePath.c_str(), ARRAY_NAME))
		{
			std::unique_ptr<ShaderSource> vertexSource(
				new ShaderSource(ShaderSource::VertexShader, vertexSourcePath));
			shader.GetList().emplace_back(move(vertexSource));
		}

		if (strcmp(fragmentSourcePath.c_str(), ARRAY_NAME))
		{
			std::unique_ptr<ShaderSource> fragSource(
				new ShaderSource(ShaderSource::FragmentShader, fragmentSourcePath));
			shader.GetList().emplace_back(move(fragSource));
		}


		shader.CompileShader();
		m_shader_collection[name] = std::move(shader);
	}

	fclose(fp);
}

Shader * ShaderContainer::GetShader(const std::string & key_)
{
	return &m_shader_collection[key_];
}
