#version 400 core
in vec2 textureCoordinate;

uniform sampler2D textureT;
uniform float stack;
uniform vec4 color;
in float is_active;
void main()
{
	if(is_active == 1.0f)
		discard;
		
	gl_FragColor = texture(textureT, textureCoordinate);
	gl_FragColor.rgb = color.rgb;
	gl_FragColor.a -= 0.7f * stack;
}