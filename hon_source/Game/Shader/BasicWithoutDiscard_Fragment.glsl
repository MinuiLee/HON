#version 400 core


in vec2 textureCoordinate;

uniform sampler2D textureT;

void main()
{
	vec4 color = texture(textureT, textureCoordinate);

	gl_FragColor = color;
}