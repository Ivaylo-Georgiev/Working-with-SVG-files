#include "Rect.h"
#include <string>
#include <algorithm> //needed for the snippet

//ancillary function
std::string ExtractTagValueRect(std::string tag, std::string attribute)
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
Rect::Rect()
	:Shape()
{
	//0 is default for coordinate attributes, while
	//none is default for colour attributes
	m_x = "0";
	m_y = "0";
	m_Height = "0";
	m_Width = "0";
	m_Stroke = "none";
	m_Fill = "none";
}

//constructor with parameters
Rect::Rect(std::string tag, int line)
	:Shape(tag, line)
{
	//THE FOLLOWING 1 LINE OF CODE IS A SNIPPET BORROWED BY Antoine, A STACK OVERFLOW USER 
	//cannonical form for erasing whitespace in a string
	tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());

	//if x is defined in the tag, assign m_x with the value; if not, set default value
	m_x = (tag.find("x=") != std::string::npos) ? ExtractTagValueRect(tag, "x=") : "0";
	//if y is defined in the tag, assign m_y with the value; if not, set default value
	m_y = (tag.find("y=") != std::string::npos) ? ExtractTagValueRect(tag, "y=") : "0";
	//if height is defined in the tag, assign m_Height with the value; if not, set default value
	m_Height = (tag.find("height=") != std::string::npos) ? ExtractTagValueRect(tag, "height=") : "0";
	//if width is defined in the tag, assign m_Width with the value; if not, set default value
	m_Width = (tag.find("width=") != std::string::npos) ? ExtractTagValueRect(tag, "width=") : "0";
	//if stroke is defined in the tag, assign m_Stroke with the value; if not, set default value
	m_Stroke = (tag.find("stroke=") != std::string::npos) ? ExtractTagValueRect(tag, "stroke=") : "none";
	//if fill is defined in the tag, assign m_Fill with the value; if not, set default value
	m_Fill = (tag.find("fill=") != std::string::npos) ? ExtractTagValueRect(tag, "fill=") : "none";
}

//print shape features
void Rect::Print() const
{
	std::cout << GetID() << " rect " << m_x << ' ' << m_y << ' ' << m_Height << ' ' << m_Width << ' '<<m_Stroke<<' ' << m_Fill << std::endl;
}

