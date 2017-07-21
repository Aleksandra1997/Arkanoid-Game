#include "Figure.h"

using namespace MF;

Figure::Figure(double red, double green, double blue)
	: m_colour_red(red), m_colour_green(green), m_colour_blue(blue)
{
	m_position_x = 0.0;
	m_position_y = 0.0;
	m_rotation_z = 0.0;
	m_flag_show = true;
}

Figure::~Figure()
{
}

void Figure::Show()
{
	m_flag_show = true;
}

void Figure::Hide()
{
	m_flag_show = false;
}

void Figure::Move(double dx, double dy)
{
	m_position_x += dx;
	m_position_y += dy;
}

void Figure::SetPosition(double x, double y)
{
	m_position_x = x;
	m_position_y = y;
}

void Figure::Rotate(double drotation_z)
{
	m_rotation_z += drotation_z;
}

void Figure::ChangeColour(double r, double g, double b)
{
	m_colour_red = r;
	m_colour_green = g;
	m_colour_blue = b;
}

