#version 400 core
in vec2 textureCoordinate;
in vec4 frag_color;

uniform sampler2D textureT;
in float is_active;
void main()
{
	if(is_active == 1.0f)
		discard;
		
	gl_FragColor = texture(textureT, textureCoordinate);
	gl_FragColor.rgb = frag_color.rgb;
}