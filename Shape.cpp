#include "Shape.h"


Shape::Shape()
{
	m_ID = 0;
	m_CorrespondingLine = 0;
}

Shape::Shape(std::string tag, int line)
{
	m_ID = ID;
	++ID;
	m_CorrespondingLine = line;
}
