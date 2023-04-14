#version 400 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec2 input_texturecoord;

out vec4 color;
out vec2 textureCoordinate;

uniform mat4 combined;

void main()
{
	gl_Position = combined * vec4(input_position, 1.0);

	textureCoordinate = input_texturecoord;
}