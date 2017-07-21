#pragma once

namespace MF
{
	class Figure
	{
	public:
		Figure(double red, double green, double blue);
		~Figure();

		void Show();
		void Hide();
		void Move(double dx, double dy);
		void SetPosition(double x, double y);
		void Rotate(double dgamma);
		void ChangeColour(double r, double g, double b);

		virtual void Draw() = 0;

	protected:
		double m_position_x;
		double m_position_y;

		double m_rotation_z;

		double m_colour_red;
		double m_colour_green;
		double m_colour_blue;

		bool m_flag_show;
	};
}
