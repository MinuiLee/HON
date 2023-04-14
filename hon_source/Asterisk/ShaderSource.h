#ifndef SHADERSOURCE_H
#define SHADERSOURCE_H
/******************************************************************************/
/*!
\headerfile   ShaderSource.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include <string>

class ShaderSource
{
	friend class Shader;

	std::string shader_source_;

	static void CleanInput(char * input_buffer, size_t length);
	static bool CheckValidate(char input_char);

public:
	enum ShaderType
	{
		VertexShader = 1,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		FragmentShader,
	};

	ShaderType type = VertexShader;

	ShaderSource();
	ShaderSource(const ShaderSource& /*input_*/){};
	~ShaderSource();
	ShaderSource(ShaderType input_type, std::string file_path);
	static ShaderSource LoadShader(ShaderType input_type, std::string file_path);
};

#endif