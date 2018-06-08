#include "Line.h"
#include <string>
#include <algorithm> //needed for the snippet

//ancillary function
std::string ExtractTagValueLine(std::string tag, std::string attribute)
{
	//flag when an opening quote is reached
	bool firstQuoteReached = false;
	//set tag initially to be the empty string
	std::string tagValue = "";

	//traverse tag starting from the given attribute
	for (int i = tag.find(attribute); i < tag.size(); ++i)
	{
		//closing quote is reached
		if (firstQuoteReached && tag[i] == '\"')
			return tagValue;

		//extract value between quotes
		if (firstQuoteReached)
			tagValue += tag[i];

		//opening quote is reached
		if (tag[i] == '\"')
			firstQuoteReached = true;
	}
	return tagValue;
}

//default constructor
Line::Line()
	:Shape()
{
	//0 is default for coordinate attributes, while
	//none is default for colour attributes
	m_x1 = "0";
	m_y1 = "0";
	m_x2 = "0";
	m_y2 = "0";
	m_Stroke = "none";
}

//constructor with parameters
Line::Line(std::string tag, int line)
	:Shape(tag, line)
{
	//THE FOLLOWING 1 LINE OF CODE IS A SNIPPET BORROWED BY Antoine, A STACK OVERFLOW USER 
	//cannonical form for erasing whitespace in a string
	tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());

	//if x1 is defined in the tag, assign m_x1 with the value; if not, set default value
	m_x1 = (tag.find("x1=") != std::string::npos) ? ExtractTagValueLine(tag, "x1=") : "0";
	//if y1 is defined in the tag, assign m_y1 with the value; if not, set default value
	m_y1 = (tag.find("y1=") != std::string::npos) ? ExtractTagValueLine(tag, "y1=") : "0";
	//if x2 is defined in the tag, assign m_x2 with the value; if not, set default value
	m_x2 = (tag.find("x2=") != std::string::npos) ? ExtractTagValueLine(tag, "x2=") : "0";
	//if y2 is defined in the tag, assign m_y2 with the value; if not, set default value
	m_y2 = (tag.find("y2=") != std::string::npos) ? ExtractTagValueLine(tag, "y2=") : "0";
	//if stroke is defined in the tag, assign m_Stroke with the value; if not, set default value
	m_Stroke = (tag.find("stroke=") != std::string::npos) ? ExtractTagValueLine(tag, "stroke=") : "none";
}

//print shape features
void Line::Print() const
{
	std::cout <<GetID()<<" line "<< m_x1<<' '<<m_y1<<' '<<m_x2<<' '<<m_y2<<' '<<m_Stroke<<std::endl;
}
