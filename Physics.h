#pragma once

#include "Figure.h" // #include "Figura.h"

namespace MF
{
	typedef struct Border
	{
		double bottom_left_corner_x; //minimalna wartosc x
		double bottom_left_corner_y; //minimalna wartosc y
		double top_right_x; //maskymalna wartosc x
		double top_right_y; //maskymalna wartosc y
	} Border;/*granice okreslaja lewy gorny naroznik (bottom_left_corner_x,bottom_left_corner_y) i prawy dolny (top_right_x,top_right_y) */

	class Physics : public Figure
	{
	protected:
		int m_previous_time; //czas ostatniej aktualizacji
		Border m_border; //granice obiektu
		//polozenie srodka masy - odziedziczone po Figure
		double m_v;//predkosc
		double m_alfa_v;//kierunek wektora predkosci w [stopniach]
		double m_g; //grawitacja
		double m_alfa_g;//kierunek wektora grawitacji

	public:
		Physics();
		~Physics();
		void Update(int current_time);//zmienia polozenie obiektu na podstawie aktualnego czasu
		//ustawia poczatkowy czas, predkosc oraz przyspieszenie
		void SetInitialParameters(int current_time, double v, double alfa_v, double g, double alfa_g);
		virtual bool Collision(const Physics& X); //wykrywanie kolizji z innym obiektem i aktualizacja prêdkoœci (funkcja zwraca true gdy jest kolizja, a false w przeciwnym razie)
		void Fast(double speed);

	protected:
		void SetBorders(double bottom_left_corner_x, double bottom_left_corner_y, double top_right_x, double top_right_y);

	private:
		void Rebound(double alfa_n); //odbicia od sciany charakteryzowanej za pomoca normalnej alfa_n
		bool IsInRectangle(double _x, double _y, const Physics& X);//wykrywa czy dany punkt (_x,_y) znajduje sie wewnatrz pewnego prostok¹ta
		double Distance(double _x, double _y, double _bottom_left_corner_x, double _bottom_left_corner_y, double _top_right_x, double _top_right_y);//wyznacza odleglosc od pewnej prostej przechodzacej przez 2 punkty
		double FindNormal(const Physics& X);//znajduje normalna boku ktory jest najblizej srodka obiektu (wynikiem funkcji jest orientacja normalnej);
	};
}
