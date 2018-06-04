#include "Rect.h"
#include <string>
#include <algorithm>

//ancillary function
std::string ExtractTagValueRect(std::string tag, std::string attribute)
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

Rect::Rect()
	:Shape()
{
	m_x = "0";
	m_y = "0";
	m_Height = "0";
	m_Width = "0";
	m_Stroke = "none";
	m_Fill = "none";
}

Rect::Rect(std::string tag, int line)
	:Shape(tag, line)
{
	//snipped borrowed by Antoine, Stack Overflow user 
	//cannonical form for erasing whitespace in a string
	tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());

	m_x = (tag.find("x=") != std::string::npos) ? ExtractTagValueRect(tag, "x=") : "0";
	m_y = (tag.find("y=") != std::string::npos) ? ExtractTagValueRect(tag, "y=") : "0";
	m_Height = (tag.find("height=") != std::string::npos) ? ExtractTagValueRect(tag, "height=") : "0";
	m_Width = (tag.find("width=") != std::string::npos) ? ExtractTagValueRect(tag, "width=") : "0";
	m_Stroke = (tag.find("stroke=") != std::string::npos) ? ExtractTagValueRect(tag, "stroke=") : "none";
	m_Fill = (tag.find("fill=") != std::string::npos) ? ExtractTagValueRect(tag, "fill=") : "none";
}

void Rect::Print() const
{
	std::cout << GetID() << " rect " << m_x << ' ' << m_y << ' ' << m_Height << ' ' << m_Width << ' '<<m_Stroke<<' ' << m_Fill << std::endl;
}



