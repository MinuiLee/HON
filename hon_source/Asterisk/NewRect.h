#pragma once

template <typename T>
class NewRect
{
    T left;
    T top;
    T width;
    T height;

public:
    NewRect(T input_left, T input_top, T input_width, T input_height) : 
	left(input_left),top(input_top),
	width(input_width),	height(input_height)
    {
    }
};

typedef NewRect<float> FloatNewRect;
