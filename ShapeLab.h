#pragma once
#include "RawData.h"
#include "Shape.h"
#include "Line.h"
#include "Rect.h"
#include "Circle.h"

class ShapeLab
{
public:
	//ShapeLab();
	ShapeLab(const char * adress);
	ShapeLab(const ShapeLab & rhs);
	ShapeLab& operator=(const ShapeLab & rhs);
	~ShapeLab();

	RawData GetRawData()const { return m_Data; }
	Shape* GetShape(int ID);

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
