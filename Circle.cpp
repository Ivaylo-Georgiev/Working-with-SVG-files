#include "Circle.h"
#include <string>
#include <algorithm> //needed for the snippet

//ancillary function
std::string ExtractTagValueCircle(std::string tag, std::string attribute)
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
Circle::Circle()
	:Shape()
{
	//0 is default for coordinate attributes, while
	//none is default for colour attributes
	m_cx = "0";
	m_cy = "0";
	m_r = "0";
	m_Stroke = "none";
	m_Fill = "none";
}

//constructor with parameters
Circle::Circle(std::string tag, int line)
	:Shape(tag, line)
{
	//THE FOLLOWING 1 LINE OF CODE IS A SNIPPET BORROWED BY Antoine, A STACK OVERFLOW USER 
	//cannonical form for erasing whitespace in a string
	tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());

	//if cx is defined in the tag, assign m_cx with the value; if not, set default value
	m_cx = (tag.find("cx=") != std::string::npos) ? ExtractTagValueCircle(tag, "cx=") : "0";
	//if cy is defined in the tag, assign m_cy with the value; if not, set default value
	m_cy = (tag.find("cy=") != std::string::npos) ? ExtractTagValueCircle(tag, "cy=") : "0";
	//if r is defined in the tag, assign m_r with the value; if not, set default value
	m_r = (tag.find("r=") != std::string::npos) ? ExtractTagValueCircle(tag, "r=") : "0";
	//if stroke is defined in the tag, assign m_Stroke with the value; if not, set default value
	m_Stroke = (tag.find("stroke=") != std::string::npos) ? ExtractTagValueCircle(tag, "stroke=") : "none";
	//if fill is defined in the tag, assign m_Fill with the value; if not, set default value
	m_Fill = (tag.find("fill=") != std::string::npos) ? ExtractTagValueCircle(tag, "fill=") : "none";
}

//print shape features
void Circle::Print() const
{
	std::cout << GetID() << " circle " << m_cx << ' ' << m_cy << ' ' <<m_r<<' '<< m_Stroke << ' ' << m_Fill << std::endl;
}

