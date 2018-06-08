/*
	Defines an abstraction for rectangle.
*/

#pragma once
#include "Shape.h"

class Rect : public Shape
{
public:
	//default constructor
	Rect();
	//constructor with parameters
	Rect(std::string tag, int line);

	//ancillary functions
	void Print() const override;
	Shape* Clone() const override{ return new Rect(*this); }

	//selectors
	std::string GetX() const { return m_x; }
	std::string GetY() const { return m_y; }
	std::string GetHeight() const { return m_Height; }
	std::string GetWidth() const { return m_Width; }
	std::string GetStroke() const { return m_Stroke; }
	std::string GetFill() const { return m_Fill; }

	//mutators
	void ChangeX(std::string change) { m_x = change; }
	void ChangeY(std::string change) { m_y = change; }
private:
	std::string m_x, m_y;
	std::string m_Height, m_Width;
	std::string m_Stroke, m_Fill;
};
