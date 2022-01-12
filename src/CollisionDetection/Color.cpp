#include "Color.h"

Color::Color() :
	r(0), g(0), b(0)
{
}

Color::Color(float r, float g, float b) :
	r(r / 255), g(g / 255), b(b / 255)
{
}
