#include "Color.h"

const Color Color::Red(1.0f, 0.0f, 0.0f, 1.0f);
const Color Color::Green(0.0f, 1.0f, 0.0f, 1.0f);
const Color Color::Blue(0.0f, 0.0f, 1.0f, 1.0f);
const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Gray(0.5f, 0.5f, 0.5f, 1.0f);
const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::TransparentBlack(0.0f, 0.0f, 0.0f, 0.0f);
const Color Color::CornflowerBlue(0.392f, 0.584f, 0.929f, 1.0f);
const Color Color::Gold(1.0f, 0.843f, 0.0f, 1.0f);


Color::Color()
{
}


Color::Color(float r, float g, float b, float a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{
}



