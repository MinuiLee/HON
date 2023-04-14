#version 400 core


in vec2 textureCoordinate;

uniform sampler2D textureT;

void main()
{
	vec4 color = texture(textureT, textureCoordinate);

	if(color.a == 0.0)
		discard;
	else
		color = vec4(0.0, 0.0, 0.0, 0.5);

	gl_FragColor = color;
}