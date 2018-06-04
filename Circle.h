#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	Circle();
	Circle(std::string tag, int line);

	void Print() const override;
	Shape* Clone() const { return new Circle(*this); }
	std::string GetCX() const { return m_cx; }
	std::string GetCY() const { return m_cy; }
	std::string GetR() const { return m_r; }
	std::string GetStroke() const { return m_Stroke; }
	std::string GetFill() const { return m_Fill; }

	void ChangeCX(std::string change) { m_cx = change; }
	void ChangeCY(std::string change) { m_cy = change; }
private:
	std::string m_cx, m_cy;
	std::string m_r;
	std::string m_Stroke, m_Fill;
};
