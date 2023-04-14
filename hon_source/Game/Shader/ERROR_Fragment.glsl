#version 400 core


in vec2 textureCoordinate;

uniform sampler2D textureT;

void main()
{
	vec4 color = vec4(0,0,1,1);
	int indexX = int(gl_FragCoord.x);
	int indexY = int(gl_FragCoord.y);
	int div = 16;
	if((indexX % div) > 8.0
	 &&(indexY % div) > 8.0 )
		color = vec4(1,0,0,1);
	else if((indexX % div) < 8.0
	 	  &&(indexY % div) < 8.0 )
		color = vec4(1,0,0,1);

	gl_FragColor = color;
}