#include "Circle.h"
#include <string>
#include <algorithm>

//ancillary function
std::string ExtractTagValueCircle(std::string tag, std::string attribute)
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

Circle::Circle()
	:Shape()
{
	m_cx = "0";
	m_cy = "0";
	m_r = "0";
	m_Stroke = "none";
	m_Fill = "none";
}

Circle::Circle(std::string tag, int line)
	:Shape(tag, line)
{
	//snipped borrowed by Antoine, Stack Overflow user 
	//cannonical form for erasing whitespace in a string
	tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());

	m_cx = (tag.find("cx=") != std::string::npos) ? ExtractTagValueCircle(tag, "cx=") : "0";
	m_cy = (tag.find("cy=") != std::string::npos) ? ExtractTagValueCircle(tag, "cy=") : "0";
	m_r = (tag.find("r=") != std::string::npos) ? ExtractTagValueCircle(tag, "r=") : "0";
	m_Stroke = (tag.find("stroke=") != std::string::npos) ? ExtractTagValueCircle(tag, "stroke=") : "none";
	m_Fill = (tag.find("fill=") != std::string::npos) ? ExtractTagValueCircle(tag, "fill=") : "none";
}

void Circle::Print() const
{
	std::cout << GetID() << " circle " << m_cx << ' ' << m_cy << ' ' <<m_r<<' '<< m_Stroke << ' ' << m_Fill << std::endl;
}

