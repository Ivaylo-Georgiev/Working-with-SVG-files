#include "RawData.h"

#include "Shape.h"
#include "Line.h"
#include "Circle.h"
#include "Rect.h"

#include <iostream>
#include <fstream>

//default constructor
RawData::RawData()
{
	m_Lines = 1;
	m_RawTags = new std::string[m_Lines];
	m_RawTags[0] = "";
	m_Adress = "";
}

//constructor with 1 parameter
RawData::RawData(std::string adress)
{
	//init member variables
	m_Adress = adress;
	m_Lines = 0;

	//corresponds to a line in the file
	std::string line;

	//open file for input and output without truncating its content
	//if file not found, create one
	std::fstream svg;
	svg.open(adress, std::fstream::out | std::fstream::in | std::fstream::app);
	
	//if file fails to open, throw exception, terminate program
	if (svg.fail())
	{
		std::cerr << "A problem while opening your file has occured.\n";
		throw std::exception();
	}

	if (svg.is_open())
	{
		std::cout << "Successfully opened "<<adress<<"\n";
		//read line by line
		while (std::getline(svg, line))
		{
			++m_Lines;
		}   //eof flag is set

		svg.clear();   //clear eof flag
		svg.seekg(0, svg.beg);   //set pointer to beginning of file

		//allocate memory and init member variable
		m_RawTags = new std::string[m_Lines];
		for (int i = 0; i < m_Lines; ++i)
		{
			std::getline(svg, line);
			m_RawTags[i] = line;
		}
		//close file
		svg.close();
	}
}

//copy constructor
RawData::RawData(const RawData & rhs)
{
	m_Lines = rhs.m_Lines;
	m_RawTags = new std::string[m_Lines];
	for (int i = 0; i < m_Lines; ++i)
	{
		m_RawTags[i] = rhs.m_RawTags[i];
	}
	m_Adress = rhs.m_Adress;
}

//operator overload
RawData& RawData::operator=(const RawData & rhs)
{
	//check for self-initialisation
	if (this!=&rhs)
	{
		//free old memory
		delete[] m_RawTags;
		m_Lines = rhs.m_Lines;
		//allocate new memory
		m_RawTags = new std::string[m_Lines];
		for (int i = 0; i < m_Lines; ++i)
		{
			m_RawTags[i] = rhs.m_RawTags[i];
		}
		m_Adress = rhs.m_Adress;
	}
	return *this;
}

//destructor
RawData::~RawData()
{
	delete[] m_RawTags;
}

void RawData::Save() const
{
	//open
	std::ofstream svg(m_Adress, std::fstream::trunc);

	//if file fails to open, throw exception, terminate program
	if (svg.fail())
	{
		std::cerr << "A problem while opening your file has occured.\n";
		throw std::exception();
	}

	if (svg.is_open())
	{
		//write
		for (int i = 0; i < m_Lines; ++i)
			svg << m_RawTags[i] + '\n';
		//close
		svg.close();

		std::cout << "Saved.\n";
	}
}

void RawData::SaveAs(std::string path) const
{
	std::ofstream svg(path); 

	//if file fails to open, throw exception, terminate program
	if (svg.fail())
	{
		std::cerr << "A problem while opening your file has occured.\n";
		throw std::exception();
	}

	if (svg.is_open())
	{
		//write
		for (int i = 0; i < m_Lines; ++i)
			svg << m_RawTags[i] + '\n';
		//close
		svg.close();
		std::cout << "Saved as "<<path<<"\n";
	}
}

//create new svg tag by shape features
void RawData::AddTag(std::string token)
{
	//last shape line index
	int lastShape = GetLastShapeIndex();

	//handle files without initial shape tags 
	if (m_Lines>0 && lastShape == 0)
		lastShape = m_Lines - 2; //the line before <\svg>

	//create buffer 
	std::string * temp = new std::string[m_Lines + 1];
	//copy tags preceding the new tag
	for (int i = 0; i <= lastShape; i++)
	{
		temp[i] = m_RawTags[i];
	}

	//handle empty files
	if (m_Lines==0)
		temp[0] = token;
	//place the new tag at its proper position
	else
		temp[lastShape + 1] = token; 
		

	//copy tags following the new tag
	for (int i = lastShape+1, j = lastShape+2; i < m_Lines; ++i, ++j)
	{
		temp[j] = m_RawTags[i];
	}
	
	delete[] m_RawTags; //free old memory
	++m_Lines; //increase lines

	//m_RawTags now points to the buffer with the new tag
	m_RawTags = temp;
}

//remove tag specified by its line
void RawData::RemoveTag(int line)
{
	//create buffer
	std::string * temp = new std::string[m_Lines - 1];
	for (int i = 0, j=0; i < m_Lines; ++i)
	{
		//skip tag corresponding to the specified line
		if (i == line)
			continue;
		temp[j] = m_RawTags[i];
		++j;
	}
	--m_Lines; //decrease lines
	delete[] m_RawTags; //free old memory

	//m_RawTags now points to the buffer without the selected tag
	m_RawTags = temp;
}

//find the last supported shape and return the number of the line it corresponds to
int RawData::GetLastShapeIndex() const
{
	int index = 0;
	//traverse all tags
	for (int i = 0; i < m_Lines; ++i)
	{
		//if a supported shape is found
		if (m_RawTags[i].find("<line") != std::string::npos ||
			m_RawTags[i].find("<circle") != std::string::npos ||
			m_RawTags[i].find("<rect") != std::string::npos)
		{
			index = i; //update index with its corresponding line
		}
	}
	return index;
}

//select a tag and change its attribute values
void RawData::ReplaceTag(int line, Shape* shape)
{
	//line
	if (dynamic_cast<Line*>(shape) != nullptr)
	{
		//update values
		m_RawTags[line] = "<line x1=\"" + dynamic_cast<Line*>(shape)->GetX1() + "\" y1=\"" + 
			dynamic_cast<Line*>(shape)->GetY1() + "\" x2=\"" + dynamic_cast<Line*>(shape)->GetX2() + 
			"\" y2=\"" + dynamic_cast<Line*>(shape)->GetY2() + "\" stroke=\"" + 
			dynamic_cast<Line*>(shape)->GetStroke() + "\" />";
	}

	//circle
	if (dynamic_cast<Circle*>(shape) != nullptr)
	{
		//update values
		m_RawTags[line] = "<circle cx=\"" + dynamic_cast<Circle*>(shape)->GetCX() + "\" cy=\"" + 
			dynamic_cast<Circle*>(shape)->GetCY() + "\" r=\"" + dynamic_cast<Circle*>(shape)->GetR() + 
			"\" stroke=\"" + dynamic_cast<Circle*>(shape)->GetStroke() + "\" fill=\"" + 
			dynamic_cast<Circle*>(shape)->GetFill() + "\" />";
	}

	//rect
	if (dynamic_cast<Rect*>(shape) != nullptr)
	{
		//update values
		m_RawTags[line] = "<rect x=\"" + dynamic_cast<Rect*>(shape)->GetX() + "\" y=\"" + 
			dynamic_cast<Rect*>(shape)->GetY() + "\" height=\"" + dynamic_cast<Rect*>(shape)->GetHeight() +
			"\" width=\"" + dynamic_cast<Rect*>(shape)->GetWidth() + "\" stroke=\"" + 
			dynamic_cast<Rect*>(shape)->GetStroke() + "\" fill=\"" + 
			dynamic_cast<Rect*>(shape)->GetFill() + "\" />";
	}
}

//print every line on the console
void RawData::Print() const
{
	//traverse all tags
	for (int i = 0; i < m_Lines; ++i)
	{
		std::cout << m_RawTags[i]<<std::endl;
	}
}
