#version 400 core
in vec2 textureCoordinate;
in float frag_fading;
uniform sampler2D textureT;

void main()
{
	gl_FragColor = texture(textureT, textureCoordinate);
	gl_FragColor.a -= frag_fading;
	if(gl_FragColor.a == 0.0)
		discard;
	//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);// additive
}