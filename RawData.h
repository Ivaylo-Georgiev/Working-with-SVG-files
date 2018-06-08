/*
	Defines a container abstraction.
	Stores the content of the .svg file line by line.
	NB: I assume that every line in the .svg file is a tag.
*/

#pragma once
#include <string>
#include "Shape.h"

class RawData
{
public:
	//---canonical form---
	//default constructor
	RawData();
	//constructor with parameters
	RawData(std::string adress);
	//copy constructor
	RawData(const RawData & rhs);
	//operator overload
	RawData& operator=(const RawData & rhs);
	//destructor
	~RawData();

	//selectors
	int GetLines()const { return m_Lines; }
	std::string * GetRawTags() const { return m_RawTags; }
	std::string GetAdress() const { return m_Adress; }

	void Save() const;
	void SaveAs(std::string path) const;
	
	//ancillary functions
	void AddTag(std::string token);
	void RemoveTag(int line);
	int GetLastShapeIndex() const;
	void ReplaceTag(int line, Shape* shape);
	void Print()const;
private:
	int m_Lines;
	std::string * m_RawTags;
	std::string m_Adress;
};
