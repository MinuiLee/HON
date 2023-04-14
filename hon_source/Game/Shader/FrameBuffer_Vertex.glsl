#version 400 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec2 input_texturecoord;

out vec2 textureCoordinate;

uniform bool is_shake;
uniform float shake_timer;

void main()
{
	gl_Position = vec4(input_position, 1.0);

	if (is_shake)
    {
        float strength = 0.05;
        gl_Position.x += cos(shake_timer * 10) * strength;        
        gl_Position.y += cos(shake_timer * 15) * strength;        
    }
	textureCoordinate = input_texturecoord;
}