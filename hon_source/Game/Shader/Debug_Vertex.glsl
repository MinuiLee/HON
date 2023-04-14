#version 400 core 
uniform vec4 translation;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out vec4 colorFrag;


void main()
{
	vec4 vert[4] = vec4[](vec4(translation.x, translation.y - translation.w, 0, 1), // left bottom
						  vec4(translation.x + translation.z, translation.y - translation.w, 0, 1), // right bottom
						  vec4(translation.x + translation.z, translation.y, 0, 1), // right top
						  vec4(translation.x, translation.y, 0, 1)); // left top

	gl_Position = projection * view * vert[gl_VertexID];
	colorFrag = color;
}