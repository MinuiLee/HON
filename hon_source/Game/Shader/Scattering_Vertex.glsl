#version 400 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec2 input_texturecoord;

layout(location = 2) in vec3 input_translation;
layout(location = 3) in vec3 input_scale;
layout(location = 4) in float input_rotation;
layout(location = 5) in vec4 input_color;
layout(location = 6) in float input_timer;


uniform mat4 combined;
uniform float timer;
uniform vec4 color;
out vec4 frag_color;
out vec2 textureCoordinate;
out float frag_timer;
void main()
{
	vec3 output_position = input_position * input_scale;
	output_position += input_translation;
	gl_Position = combined * vec4(output_position, 1.0);

	textureCoordinate = input_texturecoord;
	frag_timer = timer;
	frag_color = color;
}