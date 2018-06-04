#pragma once
#include <string>
#include "Shape.h"

/*
	Stores the content of the .svg file line by line.
*/

class RawData
{
public:
	RawData();
	RawData(const char * adress);
	RawData(const RawData & rhs);
	RawData& operator=(const RawData & rhs);
	~RawData();

	std::string * GetRawTags() const { return m_RawTags; }
	int GetLines()const { return m_Lines; }
	
	void AddTag(std::string token);
	void RemoveTag(int line);
	int GetLastShapeIndex() const;
	void ReplaceTag(int line, Shape* shape);

	void Print()const;

private:
	int m_Lines;
	std::string * m_RawTags;
};
