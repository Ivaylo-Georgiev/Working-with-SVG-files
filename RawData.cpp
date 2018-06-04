#include "RawData.h"

#include "Shape.h"
#include "Line.h"
#include "Circle.h"
#include "Rect.h"

#include <iostream>
#include <fstream>

RawData::RawData()
{
	m_Lines = 1;
	m_RawTags = new std::string[m_Lines];
	m_RawTags[0] = "";
}

//constructor with 1 parameter
RawData::RawData(const char * adress)
{
	m_Lines = 0;
	std::ifstream svg(adress);
	std::string line;

	if (svg.is_open())
	{
		//read line by line
		while (std::getline(svg, line))
		{
			++m_Lines;
		}   //eof flag is set
		
		svg.clear();   //clear flag
		svg.seekg(0, svg.beg);   //set pointer to beginning of file

		//allocate memory and initialise
		m_RawTags = new std::string[m_Lines];
		for (int i = 0; i < m_Lines; ++i)
		{
			std::getline(svg, line);
			m_RawTags[i] = line;
			//std::cout << m_RawTags[i] << std::endl;
		}
	}
	//close file
	svg.close();
}

RawData::RawData(const RawData & rhs)
{
	m_Lines = rhs.m_Lines;
	m_RawTags = new std::string[m_Lines];
	for (int i = 0; i < m_Lines; ++i)
	{
		m_RawTags[i] = rhs.m_RawTags[i];
	}
}

RawData& RawData::operator=(const RawData & rhs)
{
	if (this!=&rhs)
	{
		delete[] m_RawTags;
		m_Lines = rhs.m_Lines;
		m_RawTags = new std::string[m_Lines];
		for (int i = 0; i < m_Lines; ++i)
		{
			m_RawTags[i] = rhs.m_RawTags[i];
		}
	}
	return *this;
}

RawData::~RawData()
{
	delete[] m_RawTags;
}

void RawData::AddTag(std::string token)
{
	//last shape line
	int lastShape = GetLastShapeIndex();
	

	std::string * temp = new std::string[m_Lines + 1];
	for (int i = 0; i <= lastShape; i++)
	{
		temp[i] = m_RawTags[i];
	}
	temp[lastShape+1] = token;
	for (int i = lastShape+1, j = lastShape+2; i < m_Lines; ++i, ++j)
	{
		temp[j] = m_RawTags[i];
	}

	delete[] m_RawTags;
	++m_Lines;
	//m_RawTags = new std::string[m_Lines];
	m_RawTags = temp;
}

void RawData::RemoveTag(int line)
{
	std::string * temp = new std::string[m_Lines - 1];
	for (int i = 0, j=0; i < m_Lines; ++i)
	{
		if (i == line)
			continue;
		temp[j] = m_RawTags[i];
		++j;
	}
	--m_Lines;
	delete[] m_RawTags;
	m_RawTags = temp;
}

int RawData::GetLastShapeIndex() const
{
	int index = 0;
	for (int i = 0; i < m_Lines; ++i)
	{
		if (m_RawTags[i].find("<line") != std::string::npos ||
			m_RawTags[i].find("<circle") != std::string::npos ||
			m_RawTags[i].find("<rect") != std::string::npos)
		{
			index = i;
		}
	}
	return index;
}

void RawData::ReplaceTag(int line, Shape* shape)
{
	if (dynamic_cast<Line*>(shape) != nullptr)
	{
		m_RawTags[line] = "<line x1=\"" + dynamic_cast<Line*>(shape)->GetX1() + "\" y1=\"" + 
			dynamic_cast<Line*>(shape)->GetY1() + "\" x2=\"" + dynamic_cast<Line*>(shape)->GetX2() + 
			"\" y2=\"" + dynamic_cast<Line*>(shape)->GetY2() + "\" stroke=\"" + 
			dynamic_cast<Line*>(shape)->GetStroke() + "\" \>";
	}
	if (dynamic_cast<Circle*>(shape) != nullptr)
	{
		m_RawTags[line] = "<circle cx=\"" + dynamic_cast<Circle*>(shape)->GetCX() + "\" cy=\"" + 
			dynamic_cast<Circle*>(shape)->GetCY() + "\" r=\"" + dynamic_cast<Circle*>(shape)->GetR() + 
			"\" stroke=\"" + dynamic_cast<Circle*>(shape)->GetStroke() + "\" fill=\"" + 
			dynamic_cast<Circle*>(shape)->GetFill() + "\" \>";
	}
	if (dynamic_cast<Rect*>(shape) != nullptr)
	{
		m_RawTags[line] = "<rect x=\"" + dynamic_cast<Rect*>(shape)->GetX() + "\" y=\"" + 
			dynamic_cast<Rect*>(shape)->GetY() + "\" height=\"" + dynamic_cast<Rect*>(shape)->GetHeight() +
			"\" width=\"" + dynamic_cast<Rect*>(shape)->GetWidth() + "\" stroke=\"" + 
			dynamic_cast<Rect*>(shape)->GetStroke() + "\" fill=\"" + 
			dynamic_cast<Rect*>(shape)->GetFill() + "\" \>";
	}
}


void RawData::Print() const
{
	for (int i = 0; i < m_Lines; ++i)
	{
		std::cout << m_RawTags[i]<<std::endl;
	}
}
