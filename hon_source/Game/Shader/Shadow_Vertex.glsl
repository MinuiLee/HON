#version 400 core 
layout(location = 0) in vec3 input_position;

layout(location = 1) in vec2 input_texturecoord;

out vec4 color;
out vec2 textureCoordinate;

uniform mat4 combined;

void main()
{
	mat2 shear;
	shear[0] = vec2(1.0, 0.0);
	shear[1] = vec2(-0.2, 0.5);

	vec2 pos2 = shear * input_position.xy;
	float yOff = input_position.y - pos2.y;

	gl_Position = combined * vec4(pos2.x, pos2.y - abs(yOff), input_position.z, 1.0);


	textureCoordinate = input_texturecoord;
}