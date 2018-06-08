/*
	Ancillary functions for the ShapeLab class.
*/

#pragma once
#include "Shape.h"
#include "Line.h"
#include "Circle.h"
#include "Rect.h"
//#include <iostream>
//#include <string>

/*
//convert int to string
std::string ToStr(int value)
{
std::string strValue = "";
char ch;
while (value!=0)
{
ch = value % 10 + 48;
strValue = ch + strValue;
value /= 10;
}
return strValue;
}
*/

//perform translation on shapes
void Trans(Shape* sh, int vertical, int horizontal)
{
	//line
	if (dynamic_cast<Line*>(sh) != nullptr)
	{
		//translate x1
		dynamic_cast<Line*>(sh)->ChangeX1(std::to_string(horizontal + stoi(dynamic_cast<Line*>(sh)->GetX1())));
		//translate y1
		dynamic_cast<Line*>(sh)->ChangeY1(std::to_string(vertical + stoi(dynamic_cast<Line*>(sh)->GetY1())));
		//translate x2
		dynamic_cast<Line*>(sh)->ChangeX2(std::to_string(horizontal + stoi(dynamic_cast<Line*>(sh)->GetX2())));
		//translate y2
		dynamic_cast<Line*>(sh)->ChangeY2(std::to_string(vertical + stoi(dynamic_cast<Line*>(sh)->GetY2())));
	}

	//circle
	if (dynamic_cast<Circle*>(sh) != nullptr)
	{
		//translate cx
		dynamic_cast<Circle*>(sh)->ChangeCX(std::to_string(horizontal + stoi(dynamic_cast<Circle*>(sh)->GetCX())));
		//translate cy
		dynamic_cast<Circle*>(sh)->ChangeCY(std::to_string(vertical + stoi(dynamic_cast<Circle*>(sh)->GetCY())));
	}

	//rect
	if (dynamic_cast<Rect*>(sh) != nullptr)
	{
		//translate x
		dynamic_cast<Rect*>(sh)->ChangeX(std::to_string(horizontal + stoi(dynamic_cast<Rect*>(sh)->GetX())));
		//translate y
		dynamic_cast<Rect*>(sh)->ChangeY(std::to_string(vertical + stoi(dynamic_cast<Rect*>(sh)->GetY())));
	}
}

//generate tag by shape details
//e.g. rect 1 1 10 20 green none
std::string GenerateTag(std::string token)
{
	//set tag initially to be the empty string
	std::string tag = "";

	if (token.find("line") != std::string::npos) //line
	{
		//5 attributes defined in line
		const int ATTR_COUNT = 5; 
		//list of supported attributes for line
		std::string ATTRIBUTES[ATTR_COUNT]{ "x1", "y1", "x2", "y2", "stroke" };

		//append at begin
		tag += "<line ";
		//build tag with attribute values
		for (int i = 0; i < ATTR_COUNT; ++i)
		{
			//crop off the type of the shape from its description
			token = token.substr(token.find_first_of(" ") + 1, token.length());
			//append attribute values
			tag += ATTRIBUTES[i] + "=\"" + token.substr(0, token.find_first_of(" ")) + "\" ";
		}
	}

	if (token.find("rect") != std::string::npos) //rect
	{
		//6 attributes defined in rect
		const int ATTR_COUNT = 6; 
		//list of supported attributes for rect
		std::string ATTRIBUTES[ATTR_COUNT]{ "x", "y", "height", "width", "stroke", "fill" };

		//append at begin
		tag += "<rect ";
		//build tag with attribute values
		for (int i = 0; i < ATTR_COUNT; ++i)
		{
			//crop off the type of the shape from its description
			token = token.substr(token.find_first_of(" ") + 1, token.length());
			//append attribute values
			tag += ATTRIBUTES[i] + "=\"" + token.substr(0, token.find_first_of(" ")) + "\" ";
		}
	}

	if (token.find("circle") != std::string::npos) //circle
	{
		//5 attributes defined in circle
		const int ATTR_COUNT = 5; 
		//list of supported attributes for circle
		std::string ATTRIBUTES[ATTR_COUNT]{ "cx", "cy", "r", "stroke", "fill" };

		//append at begin
		tag += "<circle ";
		//build tag with attribute values
		for (int i = 0; i < ATTR_COUNT; ++i)
		{
			//crop off the type of the shape from its description
			token = token.substr(token.find_first_of(" ") + 1, token.length());
			//append attribute values
			tag += ATTRIBUTES[i] + "=\"" + token.substr(0, token.find_first_of(" ")) + "\" ";
		}
	}

	return tag + "/>"; //close tag
}
