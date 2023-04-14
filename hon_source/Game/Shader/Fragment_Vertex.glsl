#version 400 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec2 input_texturecoord;

layout(location = 2) in vec3 input_translation;
layout(location = 3) in vec3 input_scale;
layout(location = 4) in float input_rotation;
layout(location = 5) in vec4 input_color;
layout(location = 6) in float input_timer;


uniform mat4 combined;
uniform float fading;
out vec2 textureCoordinate;
out float frag_fading;
void main()
{
	vec3 output_position = (input_position + input_translation)*input_scale;
	
	mat3 rotation;
	rotation[0] = vec3(cos(input_rotation), -sin(input_rotation), 0.0);
	rotation[1] = vec3(sin(input_rotation), cos(input_rotation), 0.0);
	rotation[2] = vec3(0.0, 0.0, 1.0);
	
	output_position *= rotation;

	gl_Position = combined * vec4(output_position, 1.0);

	textureCoordinate = input_texturecoord;
	frag_fading = fading;
}