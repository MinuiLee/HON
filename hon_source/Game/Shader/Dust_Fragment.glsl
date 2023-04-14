#version 400 core
in vec2 textureCoordinate;

uniform sampler2D textureT;
in vec4 frag_alpha;
void main()
{
	gl_FragColor = texture(textureT, textureCoordinate);
	gl_FragColor.a -= 0.8f;
	gl_FragColor += frag_alpha;
}