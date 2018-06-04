#include "RawData.h"

#include "Shape.h"

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

void RawData::Print() const
{
	for (int i = 0; i < m_Lines; ++i)
	{
		std::cout << m_RawTags[i]<<std::endl;
	}
}
