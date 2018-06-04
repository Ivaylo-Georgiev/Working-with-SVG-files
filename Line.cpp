#include "Line.h"
#include <string>
#include <algorithm>

//ancillary function
std::string ExtractTagValueLine(std::string tag, std::string attribute)
{
	bool firstQuoteReached = false;
	std::string tagValue = "";

	for (int i = tag.find(attribute); i < tag.size(); ++i)
	{
		if (firstQuoteReached && tag[i] == '\"')
			return tagValue;

		if (firstQuoteReached)
			tagValue += tag[i];

		if (tag[i] == '\"')
			firstQuoteReached = true;
	}
	return tagValue;
}

Line::Line()
	:Shape()
{
	m_x1 = "0";
	m_y1 = "0";
	m_x2 = "0";
	m_y2 = "0";
	m_Stroke = "none";
	//m_Tag = "<line x1=\"0\" y1=\"0\" x2=\"0\" y2=\"0\" stroke=\"none\"";
}

Line::Line(std::string tag, int line)
	:Shape(tag, line)
{
	//m_Tag = tag;

	//snipped borrowed by Antoine, Stack Overflow user 
	//cannonical form for erasing whitespace in a string
	tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());

	m_x1 = (tag.find("x1=") != std::string::npos) ? ExtractTagValueLine(tag, "x1=") : "0";
	m_y1 = (tag.find("y1=") != std::string::npos) ? ExtractTagValueLine(tag, "y1=") : "0";
	m_x2 = (tag.find("x2=") != std::string::npos) ? ExtractTagValueLine(tag, "x2=") : "0";
	m_y2 = (tag.find("y2=") != std::string::npos) ? ExtractTagValueLine(tag, "y2=") : "0";
	m_Stroke = (tag.find("stroke=") != std::string::npos) ? ExtractTagValueLine(tag, "stroke=") : "none";
}

void Line::Print() const
{
	std::cout <<GetID()<<" line "<< m_x1<<' '<<m_y1<<' '<<m_x2<<' '<<m_y2<<' '<<m_Stroke<<" line: " << GetCorrespondingLine()<<std::endl;
}
