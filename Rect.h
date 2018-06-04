#pragma once
#include "Shape.h"

class Rect : public Shape
{
public:
	Rect();
	Rect(std::string tag, int line);

	void Print() const override;
	Shape* Clone() const { return new Rect(*this); }
	std::string GetX() const { return m_x; }
	std::string GetY() const { return m_y; }
	std::string GetHeight() const { return m_Height; }
	std::string GetWidth() const { return m_Width; }
private:
	std::string m_x, m_y;
	std::string m_Height, m_Width;
	std::string m_Stroke, m_Fill;
};
