#include "Circle.h"

#define NDEBUG
#include <GL/freeglut.h>

#define _USE_MATH_DEFINES
#include <cmath>

using namespace MF;

Circle::Circle(double radius, double red, double green, double blue)
	: Physics(),
	m_size_radius(radius)
{
	SetBorders(
		-m_size_radius, -m_size_radius,
		m_size_radius, m_size_radius
		);

	m_colour_red = red;
	m_colour_green = green;
	m_colour_blue = blue;
}

Circle::~Circle()
{
}

void Circle::Draw()
{
	if (m_flag_show)
	{
		glPushMatrix();
		{
			glTranslated(m_position_x, m_position_y, 0.0);
			glRotated(m_rotation_z, 0.0, 0.0, 1.0);

			glColor3d(m_colour_red, m_colour_green, m_colour_blue);

			glBegin(GL_TRIANGLE_FAN);
			{

				for (int i = 0; i <= 360; i++)
				{
					// 180 - pi
					// i - degInRad

					double degInRad = i*M_PI / 180;
					glVertex2f(cos(degInRad)*m_size_radius, sin(degInRad)*m_size_radius);
				}
			}
			glEnd();

		}
		glPopMatrix();
	}
}

