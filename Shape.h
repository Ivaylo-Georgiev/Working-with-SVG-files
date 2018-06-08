/*
	Defines an abstraction for a shape.
*/

#pragma once
#include <iostream>

//each shape has it's unique ID
static int ID = 1;

/*abstract*/ class Shape
{
public:
	//default constructor
	Shape();
	//constructor with parameters
	Shape(std::string tag, int line);
	//copy constructor
	Shape(const Shape & rhs) = default;
	//operator overload
	Shape& operator=(const Shape & rhs) = default;
	//VIRTUAL destructor to avoid undefined behavior
	virtual ~Shape() = default;

	//selectors
	int GetID()const { return m_ID; }
	int GetCorrespondingLine()const { return m_CorrespondingLine; }

	//ancillary functions
	void DecreaseLine() { --m_CorrespondingLine; }
	virtual void Print() const = 0; //pure virtual function
	virtual Shape* Clone() const = 0; //pure virtual function

private:
	int m_ID;
	int m_CorrespondingLine;
};
