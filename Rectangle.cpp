#include "Rectangle.h"

#define NDEBUG
#include <GL/freeglut.h>

using namespace MF;

Rectangle::Rectangle(double width, double height, double red, double green, double blue)
	: Physics(),
	m_size_width(width), m_size_height(height)
{
	SetBorders(
		-m_size_width / 2, -m_size_height / 2,
		m_size_width / 2, m_size_height / 2
		);

	m_colour_red = red;
	m_colour_green = green;
	m_colour_blue = blue;
}

Rectangle::~Rectangle()
{
}

void Rectangle::Draw()
{
	if (m_flag_show)
	{
		glPushMatrix();
		{
			glTranslated(m_position_x, m_position_y, 0.0);
			glRotated(m_rotation_z, 0.0, 0.0, 1.0);

			glColor3d(m_colour_red, m_colour_green, m_colour_blue);

			glBegin(GL_POLYGON);
			{
				glVertex3d(-m_size_width / 2, m_size_height / 2, 0);
				glVertex3d(m_size_width / 2, m_size_height / 2, 0);
				glVertex3d(m_size_width / 2, -m_size_height / 2, 0);
				glVertex3d(-m_size_width / 2, -m_size_height / 2, 0);
			}
			glEnd();
		}
		glPopMatrix();
	}
}