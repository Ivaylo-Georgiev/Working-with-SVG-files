#pragma once
#include "Shape.h"

class Line : public Shape
{
public:
	Line();
	Line(std::string tag, int line);

	void Print() const override;
	Shape* Clone() const { return new Line(*this); }
	std::string GetX1() const { return m_x1; }
	std::string GetX2() const { return m_x2; }
	std::string GetY1() const { return m_y1; }
	std::string GetY2() const { return m_y2; }
	std::string GetStroke() const { return m_Stroke; }

	void ChangeX1(std::string change) { m_x1 = change; }
	void ChangeX2(std::string change) {	m_x2 = change; }
	void ChangeY1(std::string change) { m_y1 = change; }
	void ChangeY2(std::string change) { m_y2 = change; }

private:
	std::string m_x1, m_y1, m_x2, m_y2;
	std::string m_Stroke;
};
