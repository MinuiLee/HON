#ifndef COLOR_H
#define COLOR_H
/******************************************************************************/
/*!
\headerfile   Color.h
\author Jaejun Jang
\par    email: jjj404001\@gmail.com
\date   2018/12/14
\brief
		Header file for Color struct.

\copyright
All content  2019 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#define ColorChannel float

struct Color
{
	// total 3 * 4 bytes wasted.
	ColorChannel Red;
	ColorChannel Green;
	ColorChannel Blue;
	ColorChannel Alpha;
	Color();
	Color(ColorChannel input_R, ColorChannel input_G, ColorChannel input_B,
		ColorChannel input_A = 1.0f)
		: Red(input_R), Green(input_G), Blue(input_B), Alpha(input_A)
	{
	}

public:
	ColorChannel * GetColorPtr() { return &Red; }
};

#endif