#version 430 core 
layout(location = 0) in vec3 input_position;
layout(location = 1) in vec4 input_color;
layout(location = 2) in vec2 input_texturecoord;

out vec4 color;
out vec2 textureCoordinate;

uniform mat4 combined;

void main()
{
	vec4 vertices[4] = vec4[4](vec4( 1.0, -1.0, 1.0, 1.0),vec4(-1.0, -1.0, 1.0, 1.0),vec4( -1.0, 1.0, 1.0, 1.0),
							   vec4( 1.0, 1.0, 1.0, 1.0));


	gl_Position = vertices[gl_VertexID];
	color = input_color;
	textureCoordinate = input_texturecoord;
}