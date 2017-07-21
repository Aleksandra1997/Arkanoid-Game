#include "Physics.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace MF;

Physics::Physics()
	: Figure(0.5, 0.5, 0.5)
{
	//obiekt staly
	m_g = 0.0;
	m_alfa_g = 0.0;
	m_v = 0.0;
	m_alfa_v = 0.0;
	//ustawia domyslna obwiednie obiektu
	m_border.bottom_left_corner_x = -1.0;
	m_border.bottom_left_corner_y = -1.0;

	m_border.top_right_x = 1.0;
	m_border.top_right_y = 1.0;
}

Physics::~Physics()
{
}

void Physics::Rebound(double alfa_n) //Rebound od sciany charakteryzowanej za pomoca normalnej alfa_n
{
	//prawo odbicia "kat padania rowny katowi odbicia (pod warunkiem, ze obiekt wnika do wnetrza)
	if (abs(alfa_n - m_alfa_v) > 90.0)
	{
		m_alfa_v = alfa_n - (180.0 + m_alfa_v - alfa_n);
	}
}

void Physics::Update(int current_time) //zmienia polozenie obiektu na podstawie aktualnego czasu
{
	int delta_t = current_time - m_previous_time;
	double v_x, v_y;
	v_x = m_v*cos(m_alfa_v / 180.0*M_PI);
	v_y = m_v*sin(m_alfa_v / 180.0*M_PI);
	m_position_x = m_position_x + v_x*delta_t + 0.5*m_g*cos(m_alfa_g / 180.0*M_PI)*delta_t*delta_t;
	m_position_y = m_position_y + v_y*delta_t + 0.5*m_g*sin(m_alfa_g / 180.0*M_PI)*delta_t*delta_t;

	//aktualizacja predkosci
	v_x = v_x + m_g*cos(m_alfa_g / 180.0*M_PI)*delta_t;
	v_y = v_y + m_g*sin(m_alfa_g / 180.0*M_PI)*delta_t;
	//wypadkowa predkosc
	m_v = sqrt(v_x*v_x + v_y*v_y);
	//kierunek predkosci
	m_alfa_v = atan2(v_y, v_x)*180.0 / M_PI;

	m_previous_time = current_time;
}

void Physics::Fast(double speed)
{
	this->m_v += speed;
}


void Physics::SetInitialParameters(int current_time, double v, double alfa_v, double g, double alfa_g)
{
	m_previous_time = current_time;

	m_v = v;
	m_alfa_v = alfa_v;

	m_g = g;
	m_alfa_g = alfa_g;
}

void Physics::SetBorders(double bottom_left_corner_x, double bottom_left_corner_y, double top_right_x, double top_right_y)
{
	m_border.bottom_left_corner_x = bottom_left_corner_x;
	m_border.bottom_left_corner_y = bottom_left_corner_y;
	m_border.top_right_x = top_right_x;
	m_border.top_right_y = top_right_y;
}

bool Physics::Collision(const Physics& X) //wykrywanie kolizji z innym obiektem i aktualizacja prêdkoœci (funkcja zwraca true gdy jest kolizja, a false w przeciwnym razie)
{
	//jesli wystepuje kolizja to przynajmniej jeden z wierzcholkow musi zawierac sie wewnatrz
	//sprawdzenie czy ktorys z wierzcholkow obiektu nie zawiera sie w obiekcie sprawdzanym
	bool flagCollision = false;
	if (IsInRectangle(m_position_x + m_border.bottom_left_corner_x,
		m_position_y + m_border.bottom_left_corner_y, X) == 1)
	{
		flagCollision = true;
	}
	else if (IsInRectangle(m_position_x + m_border.bottom_left_corner_x,
		m_position_y + m_border.top_right_y, X) == 1)
	{
		flagCollision = true;
	}
	else if (IsInRectangle(m_position_x + m_border.top_right_x,
		m_position_y + m_border.top_right_y, X) == 1)
	{
		flagCollision = true;
	}
	else if (IsInRectangle(m_position_x + m_border.top_right_x,
		m_position_y + m_border.bottom_left_corner_y, X) == 1)
	{
		flagCollision = true;
	}
	//odworcenie sprawdzania 
	else if (IsInRectangle(X.m_position_x + X.m_border.bottom_left_corner_x,
		X.m_position_y + X.m_border.bottom_left_corner_y, *this) == 1)
	{
		flagCollision = true;
	}
	else if (IsInRectangle(X.m_position_x + X.m_border.bottom_left_corner_x, 
		X.m_position_y + X.m_border.top_right_y, *this) == 1)
	{
		flagCollision = true;
	}
	else if (IsInRectangle(X.m_position_x + X.m_border.top_right_x,
		X.m_position_y + X.m_border.top_right_y, *this) == 1)
	{
		flagCollision = true;
	}
	else if (IsInRectangle(X.m_position_x + X.m_border.top_right_x,
		X.m_position_y + X.m_border.bottom_left_corner_y, *this) == 1)
	{
		flagCollision = true;
	}

	//obsluga kolizji
	if (flagCollision)
	{
		//znalezienie boku od ktorego nastapi odbicie
		double alfa_n = FindNormal(X);
		Rebound(alfa_n);
	}

	return flagCollision;
}

bool Physics::IsInRectangle(double _x, double _y, const Physics& X)//wykrywa czy dany punkt (_x,_y) znajduje sie wewnatrz pewnego kwadratu
{
	if (((_x < X.m_position_x + X.m_border.top_right_x) &&
		(_x > X.m_position_x + X.m_border.bottom_left_corner_x)) &&
		((_y < X.m_position_y + X.m_border.top_right_y) &&
		(_y > X.m_position_y + X.m_border.bottom_left_corner_y)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Physics::Distance(double _x, double _y, double _bottom_left_corner_x, double _bottom_left_corner_y, double _top_right_x, double _top_right_y)//wyznacza odleglosc od pewnej prostej przechodzacej przez 2 punkty
{
	//rownanie prostej pzrechodzacej pzrez 2 punkty: y=Ax+B
	double d;
	if (_top_right_x != _bottom_left_corner_x)
	{
		double A = (_top_right_y - _bottom_left_corner_y) / (_top_right_x - _bottom_left_corner_x);
		double B = _bottom_left_corner_y - A * _bottom_left_corner_x;
		//wyznaczenie odelglosci:
		d = fabs(A*_x - _y + B) / sqrt(A*A + 1.0);
	}
	else
	{
		d = fabs(_x - _top_right_x);
	}

	return d;
}

double Physics::FindNormal(const Physics& X)//znajduje normalna boku ktory jest najblizej srodka obiektu (wynikiem funkcji jest orientacja normalnej);
{
	double distances[4];//tablica zawierajaca odleglosc srodka obiektu od bokow, przyjmuje sie ze odbicie nastepuje od boku lezacego najblizej srodka obiektu
	int min_idx = 0;
	distances[0] = Distance(m_position_x, m_position_y, X.m_position_x + X.m_border.bottom_left_corner_x, X.m_position_y + X.m_border.bottom_left_corner_y, X.m_position_x + X.m_border.bottom_left_corner_x, X.m_position_y + X.m_border.top_right_y);
	distances[1] = Distance(m_position_x, m_position_y, X.m_position_x + X.m_border.bottom_left_corner_x, X.m_position_y + X.m_border.top_right_y, X.m_position_x + X.m_border.top_right_x, X.m_position_y + X.m_border.top_right_y);
	distances[2] = Distance(m_position_x, m_position_y, X.m_position_x + X.m_border.top_right_x, X.m_position_y + X.m_border.top_right_y, X.m_position_x + X.m_border.top_right_x, X.m_position_y + X.m_border.bottom_left_corner_y);
	distances[3] = Distance(m_position_x, m_position_y, X.m_position_x + X.m_border.top_right_x, X.m_position_y + X.m_border.bottom_left_corner_y, X.m_position_x + X.m_border.bottom_left_corner_x, X.m_position_y + X.m_border.bottom_left_corner_y);

	//poszukiwanie minimalnej wartosci Distancei
	for (int i = 1; i < 4; i++)
	{
		if (distances[i] < distances[min_idx])
		{
			min_idx = i;
		}
	}

	double result_angle = 0.0;
	//wyznaczenie normalnych najblizej lezacego boku, (dla aktualnej wersji zawsze leza wzdloz osi X lub Y)
	switch (min_idx)
	{
		case 0:
		{
			result_angle = 180.0;
		}
		break;
		case 1:
		{
			result_angle = 90.0;
		}
		break;
		case 2: 
		{
			result_angle = 0.0;
		}
		break;
		case 3: 
		{
			result_angle = -90.0;
		}
		break;
		default: 
		{
			result_angle = 0.0;
		}
		break;
	}

	return result_angle;
}

