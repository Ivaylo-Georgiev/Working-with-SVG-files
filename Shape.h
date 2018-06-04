#pragma once
#include <iostream>

static int ID = 1;

class Shape
{
public:
	Shape();
	Shape(std::string tag, int line);
	Shape(const Shape & rhs) = default;
	Shape& operator=(const Shape & rhs) = default;
	virtual ~Shape() = default;

	int GetID()const { return m_ID; }
	int GetCorrespondingLine()const { return m_CorrespondingLine; }
	void DecreaseLine() { --m_CorrespondingLine; }
	virtual void Print() const = 0; //pure virtual function
	virtual Shape* Clone() const = 0; //pure virtual function

private:
	int m_ID;
	int m_CorrespondingLine;
};
