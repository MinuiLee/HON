#version 400 core
in vec2 textureCoordinate;

uniform bool is_fadeIn;
uniform float fadeIn_timer;
uniform bool is_fadeOut;
uniform float fadeOut_timer;
uniform vec2 view_position;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	gl_FragColor = texture(texture0, textureCoordinate) + texture(texture1, textureCoordinate + view_position);
	

	
	//if(gl_FragColor == color1)
	//	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

 	const vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	if(is_fadeIn)
	{
		gl_FragColor = mix(gl_FragColor, black, fadeIn_timer);
	}
	else if(is_fadeOut)
	{
		gl_FragColor = mix(gl_FragColor, black, fadeOut_timer);
	}
}