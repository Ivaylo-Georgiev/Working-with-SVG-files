#include "ShapeLab.h"
#include "AncillaryFuncs.h"
#include "RawData.h"
#include "Shape.h"
#include <string.h>
#include <string>
#include <algorithm>
#include <math.h>

ShapeLab::ShapeLab(const char * adress)
{
	RawData temp(adress);
	m_Data = temp;
	m_ShapesNum = 0;

	//count shapes
	for (int i = 0; i < m_Data.GetLines(); i++)
	{
		if (m_Data.GetRawTags()[i].find("<line")!=std::string::npos ||
			m_Data.GetRawTags()[i].find("<circle") != std::string::npos ||
			m_Data.GetRawTags()[i].find("<rect") != std::string::npos)
		{
			++m_ShapesNum;
		}
	}
	
	m_Shapes = new Shape*[m_ShapesNum];
	for (int i = 0, j = 0; i < m_Data.GetLines(); ++i)
	{
		if (m_Data.GetRawTags()[i].find("<line") != std::string::npos)
			m_Shapes[j++] = new Line(m_Data.GetRawTags()[i], i);
		if (m_Data.GetRawTags()[i].find("<circle") != std::string::npos)
			m_Shapes[j++] = new Circle(m_Data.GetRawTags()[i], i);
		if (m_Data.GetRawTags()[i].find("<rect") != std::string::npos)
			m_Shapes[j++] = new Rect(m_Data.GetRawTags()[i], i);
	}
}

ShapeLab::ShapeLab(const ShapeLab& rhs)
{
	m_Data = rhs.m_Data;
	m_ShapesNum = rhs.m_ShapesNum;
	m_Shapes = new Shape* [m_ShapesNum];
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		m_Shapes[i] = rhs.m_Shapes[i]->Clone();
	}
}

ShapeLab& ShapeLab::operator=(const ShapeLab& rhs)
{
	if (this!=&rhs)
	{
		m_Data = rhs.m_Data;
		m_ShapesNum = rhs.m_ShapesNum;

		/*
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			delete m_Shapes[i];
		}
		*/
		delete[] m_Shapes;

		m_Shapes = new Shape*[m_ShapesNum];
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			m_Shapes[i] = rhs.m_Shapes[i]->Clone();
		}
	}
	return *this;
}

//destructor
ShapeLab::~ShapeLab()
{
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		delete m_Shapes[i];
	}
	delete[] m_Shapes;
}

Shape* ShapeLab::GetShape(int ID)
{
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		if (m_Shapes[i]->GetID()==ID)
		{
			return m_Shapes[i];
		}
	}
	return nullptr;
}

void ShapeLab::Print() const
{
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		m_Shapes[i]->Print();
	}
}

ShapeLab& ShapeLab::Create(std::string tagDetails)
{
	std::string newTag = GenerateTag(tagDetails);
	m_Data.AddTag(newTag);
	++m_ShapesNum;

	Shape ** buffer = new Shape *[m_ShapesNum];
	for (int i = 0; i < m_ShapesNum-1; i++)
	{
		buffer[i] = m_Shapes[i];
	}

	/*
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		delete m_Shapes[i];
	}
	*/
	delete[] m_Shapes;

	if (newTag.find("<line") != std::string::npos)
		buffer[m_ShapesNum - 1] = new Line(newTag, m_Data.GetLastShapeIndex());
	if (newTag.find("<circle") != std::string::npos)
		buffer[m_ShapesNum - 1] = new Circle(newTag, m_Data.GetLastShapeIndex());
	if (newTag.find("<rect") != std::string::npos)
		buffer[m_ShapesNum - 1] = new Rect(newTag, m_Data.GetLastShapeIndex());
	
	m_Shapes = buffer;
	
	return *this;
}

ShapeLab& ShapeLab::Erase(int id)
{
	//remove tag from raw data and shift shapes
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		if (m_Shapes[i]->GetID()==id)
		{
			m_Data.RemoveTag(m_Shapes[i]->GetCorrespondingLine());
			for (int j = i; j < m_ShapesNum; ++j)
			{
				m_Shapes[j]->DecreaseLine();
				m_Shapes[j] = m_Shapes[j + 1];
			}
			break;
		}
	}

	Shape ** buffer = new Shape * [m_ShapesNum - 1];
	--m_ShapesNum;
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		buffer[i] = m_Shapes[i];
	}

	/*
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		delete m_Shapes[i];
	}
	*/
	delete[] m_Shapes;

	m_Shapes = buffer;

	return *this;
}

ShapeLab& ShapeLab::Translate(std::string info)
{
	//snippet borrowed by Antoine, a Stack Overflow user 
	//cannonical form for erasing whitespace in a string
	info.erase(std::remove(info.begin(), info.end(), ' '), info.end());

	int vertical = stoi(info.substr(info.find_first_of("=") + 1,info.find("horizontal=") - info.find_first_of("=")));
	int horizontal = stoi(info.substr(info.find_last_of('=') + 1));

	//translate all shapes
	if (info.find("vertical=")==0)
	{
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			Trans(m_Shapes[i], vertical, horizontal);
			m_Data.ReplaceTag(m_Shapes[i]->GetCorrespondingLine(), m_Shapes[i]);
		}
	}
	//translate by ID
	else
	{
		int transID = stoi(info.substr(0, info.find("vertical")));
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			if (m_Shapes[i]->GetID() == transID)
			{
				Trans(m_Shapes[i], vertical, horizontal);
				m_Data.ReplaceTag(m_Shapes[i]->GetCorrespondingLine(), m_Shapes[i]);
			}
		}
	}
	return *this;
}

void ShapeLab::Within(std::string info) const
{
	if (info.find("circle") == 0)
	{
		/*
		The distance between (xc, yc) and (xp, yp) is given by the Pythagorean theorem as
		d = sqrt((xp - xc)^2 + (yp - yc)^2)
		The point (xp, yp) is inside the circle if d^2<r^2 and on the circle if d^2=r^2
		*/

		Circle* temp = new Circle(GenerateTag(info), -1);
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			//line
			if (dynamic_cast<Line*>(m_Shapes[i]) != nullptr)
			{
				std::cout << "LINE\n";
				if (pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX1()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY1()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX2()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY2()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2))
				{
					m_Shapes[i]->Print();
				}
			}

			//circle
			if (dynamic_cast<Circle*>(m_Shapes[i]) != nullptr)
			{
				std::cout << "CIRCLE\n";
				if (pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) + stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) - stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) + stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) - stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2))
				{
					m_Shapes[i]->Print();
				}
			}

			//rect
			if (dynamic_cast<Rect*>(m_Shapes[i]) != nullptr)
			{
				std::cout << "RECT\n";
				if (pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetWidth()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetWidth()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetHeight()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetHeight()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2))
				{
					m_Shapes[i]->Print();
				}
			}
		}
		delete temp;
	}
	else if (info.find("rect") == 0)
	{
		Rect* temp = new Rect(GenerateTag(info), -1);
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			//line
			if (dynamic_cast<Line*>(m_Shapes[i])!=nullptr)
			{
				std::cout << "LINE\n";
				if (stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX1()) >= stoi(temp->GetX()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX1()) <= stoi(temp->GetX()) + stoi(temp->GetWidth()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY1()) >= stoi(temp->GetY()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY1()) <= stoi(temp->GetY()) + stoi(temp->GetHeight()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX2()) >= stoi(temp->GetX()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX2()) <= stoi(temp->GetX()) + stoi(temp->GetWidth()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY2()) >= stoi(temp->GetY()) &&
					stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY2()) <= stoi(temp->GetY()) + stoi(temp->GetHeight()))
				{
					m_Shapes[i]->Print();
				}
			}

			//circle
			if (dynamic_cast<Circle*>(m_Shapes[i]) != nullptr)
			{
				std::cout << "CIRCLE\n";

				if (stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) + stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) <= stoi(temp->GetX())+stoi(temp->GetWidth()) &&
					stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) - stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) >= stoi(temp->GetX()) &&
					stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) + stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) <= stoi(temp->GetY()) + stoi(temp->GetHeight()) &&
					stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) - stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) >= stoi(temp->GetY()))
				{
					m_Shapes[i]->Print();
				}
			}
			if (dynamic_cast<Rect*>(m_Shapes[i]) != nullptr)
			{
				std::cout << "RECT\n";

				if (stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) >= stoi(temp->GetX()) &&
					stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) <= stoi(temp->GetX()) + stoi(temp->GetWidth()) &&
					stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) >= stoi(temp->GetY()) &&
					stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) <= stoi(temp->GetY()) + stoi(temp->GetHeight()) &&
					stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetWidth()) <= stoi(temp->GetX()) + stoi(temp->GetWidth()) &&
					stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetHeight()) <= stoi(temp->GetY()) + stoi(temp->GetHeight()))
				{
					m_Shapes[i]->Print();
				}
			}
		}
		delete temp;
	}
	else
		std::cout << "Shape not supported. \n";
}
