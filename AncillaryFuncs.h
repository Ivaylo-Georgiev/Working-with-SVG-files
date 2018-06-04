#pragma once
#include "Shape.h"
#include "Line.h"
#include "Circle.h"
#include "Rect.h"
#include <iostream>
#include <string>

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
		dynamic_cast<Line*>(sh)->ChangeX1(std::to_string(horizontal + stoi(dynamic_cast<Line*>(sh)->GetX1())));
		dynamic_cast<Line*>(sh)->ChangeY1(std::to_string(vertical + stoi(dynamic_cast<Line*>(sh)->GetY1())));
		dynamic_cast<Line*>(sh)->ChangeX2(std::to_string(horizontal + stoi(dynamic_cast<Line*>(sh)->GetX2())));
		dynamic_cast<Line*>(sh)->ChangeY2(std::to_string(vertical + stoi(dynamic_cast<Line*>(sh)->GetY2())));
	}

	//circle
	if (dynamic_cast<Circle*>(sh) != nullptr)
	{
		dynamic_cast<Circle*>(sh)->ChangeCX(std::to_string(horizontal + stoi(dynamic_cast<Circle*>(sh)->GetCX())));
		dynamic_cast<Circle*>(sh)->ChangeCY(std::to_string(vertical + stoi(dynamic_cast<Circle*>(sh)->GetCY())));
	}

	//rect
	if (dynamic_cast<Rect*>(sh) != nullptr)
	{
		dynamic_cast<Rect*>(sh)->ChangeX(std::to_string(horizontal + stoi(dynamic_cast<Rect*>(sh)->GetX())));
		dynamic_cast<Rect*>(sh)->ChangeY(std::to_string(vertical + stoi(dynamic_cast<Rect*>(sh)->GetY())));
	}
}

//generate tag by shape details
//e.g. rect 1 1 10 20 green none
std::string GenerateTag(std::string token)
{
	std::string tag = "";

	if (token.find("line") != std::string::npos)
	{
		const int ATTR_COUNT = 5; //5 attributes defined in line
		std::string ATTRIBUTES[ATTR_COUNT]{ "x1", "y1", "x2", "y2", "stroke" };

		tag += "<line ";
		for (int i = 0; i < ATTR_COUNT; ++i)
		{
			token = token.substr(token.find_first_of(" ") + 1, token.length());
			tag += ATTRIBUTES[i] + "=\"" + token.substr(0, token.find_first_of(" ")) + "\" ";
		}
	}

	if (token.find("rect") != std::string::npos)
	{
		const int ATTR_COUNT = 6; //6 attributes defined in rect
		std::string ATTRIBUTES[ATTR_COUNT]{ "x", "y", "height", "width", "stroke", "fill" };

		tag += "<rect ";
		for (int i = 0; i < ATTR_COUNT; ++i)
		{
			token = token.substr(token.find_first_of(" ") + 1, token.length());
			tag += ATTRIBUTES[i] + "=\"" + token.substr(0, token.find_first_of(" ")) + "\" ";
		}
	}

	if (token.find("circle") != std::string::npos)
	{
		const int ATTR_COUNT = 5; //5 attributes defined in circle
		std::string ATTRIBUTES[ATTR_COUNT]{ "cx", "cy", "r", "stroke", "fill" };

		tag += "<circle ";
		for (int i = 0; i < ATTR_COUNT; ++i)
		{
			token = token.substr(token.find_first_of(" ") + 1, token.length());
			tag += ATTRIBUTES[i] + "=\"" + token.substr(0, token.find_first_of(" ")) + "\" ";
		}
	}

	return tag + "/>";
}
