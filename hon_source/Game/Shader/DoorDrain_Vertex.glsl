#version 400 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec2 input_texturecoord;

layout(location = 2) in vec3 input_translation;
layout(location = 3) in vec3 input_scale;
layout(location = 4) in float input_rotation;
layout(location = 5) in vec4 input_color;
layout(location = 6) in float input_timer;

uniform mat4 combined;


out vec2 textureCoordinate;
out float is_active;
void main()
{
	vec4 output_position = ((vec4(input_position, 0.0f) + vec4(input_translation, 1.0f)) * vec4(input_scale, 1.0f));
	gl_Position = combined * output_position;

	if(gl_Position.x < -0.6f && gl_Position.y > 0.7f)
		is_active = 1.0f;
	else
		is_active = 0.0f;
		
	gl_Position.z = 1.0f;
	textureCoordinate = input_texturecoord;
}