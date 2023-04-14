#version 400 core 
layout(location = 0) in vec3 input_position;

layout(location = 1) in vec2 input_texturecoord;

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
	/*mat2 shear = {vec2(1.0, 0.0),
				  vec2(0.7, 0.5)};*/
	mat2 shear;
	shear[0] = vec2(1.0, 0.0);
	shear[1] = vec2(-0.2, 0.5);

	vec2 pos2 = shear * input_position.xy;
	float yOff = input_position.y - pos2.y;

	gl_Position = combined * vec4(pos2.x, pos2.y - abs(yOff), input_position.z, 1.0);


	textureCoordinate = animUV[gl_VertexID];
}