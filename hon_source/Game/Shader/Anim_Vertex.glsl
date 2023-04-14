#version 400 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec4 input_color;
layout(location = 2) in vec2 input_texturecoord;

out vec4 color;
out vec2 textureCoordinate;

uniform mat4 combined;
uniform vec2 uv;
uniform vec2 coordinate;

float single_width = (1 / uv.y);
float single_height = (1 / uv.x);
vec2 animUV[4] = vec2[](vec2(single_width * coordinate.x,       single_height * (uv.x - (coordinate.y +1))),
						vec2(single_width * (coordinate.x + 1), single_height * (uv.x - (coordinate.y +1))),
						vec2(single_width * (coordinate.x + 1), single_height * (uv.x - (coordinate.y))),
						vec2(single_width * coordinate.x,       single_height * (uv.x - (coordinate.y))));

void main()
{
	gl_Position = combined * vec4(input_position, 1.0);
	color = input_color;
	textureCoordinate = animUV[gl_VertexID];
}