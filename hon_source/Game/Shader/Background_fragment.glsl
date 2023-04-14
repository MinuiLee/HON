#version 430 core 

in vec4 color;
in vec2 textureCoordinate;

uniform sampler2D textureT;

void main()
{
		gl_FragColor  = texture(textureT, textureCoordinate);
}