/*
	Defines an abstraction of a lab where a shape and it's 
	corresponding svg tag are manipulated.
*/

#pragma once
#include "RawData.h"
#include "Shape.h"
#include "Line.h"
#include "Rect.h"
#include "Circle.h"

class ShapeLab
{
public:
	//---canonical form---
	//ShapeLab(); //default state is not well defined
	//constructor with parameters
	ShapeLab(std::string adress);
	//copy constructor
	ShapeLab(const ShapeLab & rhs);
	//operator overload
	ShapeLab& operator=(const ShapeLab & rhs);
	//destructor
	~ShapeLab();

	//selecors
	RawData GetRawData()const { return m_Data; }
	Shape* GetShape(int ID);
	int GetShapesNum() const { return m_ShapesNum; }

	//shape-manipulating functions
	void Print() const;
	ShapeLab& Create(std::string tagDetails);
	ShapeLab& Erase(int id);
	ShapeLab& Translate(std::string info);
	void Within(std::string info) const;
private:
	int m_ShapesNum;
	RawData m_Data;
	Shape** m_Shapes;
};
