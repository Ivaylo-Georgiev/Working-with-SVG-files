#include "ShapeLab.h"
#include "AncillaryFuncs.h"
#include "RawData.h"
#include "Shape.h"
#include <string.h>
#include <string>
#include <algorithm> //needed for the snippet
#include <math.h>

//constructor with parameters
ShapeLab::ShapeLab(std::string adress)
{
	//create a buffer
	RawData temp(adress);

	//init member variables
	m_Data = temp;
	m_ShapesNum = 0;

	//count shapes
	for (int i = 0; i < m_Data.GetLines(); i++)
	{
		//if a supported shape is found
		if (m_Data.GetRawTags()[i].find("<line") != std::string::npos ||
			m_Data.GetRawTags()[i].find("<circle") != std::string::npos ||
			m_Data.GetRawTags()[i].find("<rect") != std::string::npos)
		{
			//increase the numbers of shapes
			++m_ShapesNum;
		}
	}

	//allocate memory for the supported shapes
	m_Shapes = new Shape*[m_ShapesNum];

	//traverse all tags
	for (int i = 0, j = 0; i < m_Data.GetLines(); ++i)
	{
		//create a new line
		if (m_Data.GetRawTags()[i].find("<line") != std::string::npos)
			m_Shapes[j++] = new Line(m_Data.GetRawTags()[i], i);
		//create a new circle
		if (m_Data.GetRawTags()[i].find("<circle") != std::string::npos)
			m_Shapes[j++] = new Circle(m_Data.GetRawTags()[i], i);
		//create a new rect
		if (m_Data.GetRawTags()[i].find("<rect") != std::string::npos)
			m_Shapes[j++] = new Rect(m_Data.GetRawTags()[i], i);
	}
}

//copy constructor
ShapeLab::ShapeLab(const ShapeLab& rhs)
{
	m_Data = rhs.m_Data;
	m_ShapesNum = rhs.m_ShapesNum;
	m_Shapes = new Shape*[m_ShapesNum];
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		//beacause all shapes are pointers, a copy is needed,
		//so that the two objects don't point to the same location in the memory
		m_Shapes[i] = rhs.m_Shapes[i]->Clone();
	}
}

//operator overload
ShapeLab& ShapeLab::operator=(const ShapeLab& rhs)
{
	if (this != &rhs)
	{
		//free old memory
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			delete m_Shapes[i];
		}
		delete[] m_Shapes;

		//allocate new memory
		m_Shapes = new Shape*[rhs.m_ShapesNum];
		for (int i = 0; i < rhs.m_ShapesNum; ++i)
		{
			//beacause all shapes are pointers, a copy is needed,
			//so that the two objects don't point to the same location in the memory
			m_Shapes[i] = rhs.m_Shapes[i]->Clone();
		}
		m_Data = rhs.m_Data;
		m_ShapesNum = rhs.m_ShapesNum;
	}
	return *this;
}

//destructor
ShapeLab::~ShapeLab()
{
	//free memory
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		delete m_Shapes[i];
	}
	delete[] m_Shapes;
}

//get shape by ID
Shape* ShapeLab::GetShape(int id)
{
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		if (m_Shapes[i]->GetID() == id)
		{
			return m_Shapes[i];
		}
	}
	return nullptr;
}

//print information about the shapes
void ShapeLab::Print() const
{
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		m_Shapes[i]->Print();
	}
}

//create new shape and add its corresponding tag to the raw data
ShapeLab& ShapeLab::Create(std::string tagDetails)
{
	//generate tag from shape features given in tagDetails
	std::string newTag = GenerateTag(tagDetails);
	//add that tag to the raw data
	m_Data.AddTag(newTag);
	++m_ShapesNum; //increase shapes
	//get the index of the last shape
	int index = m_Data.GetLastShapeIndex();

	//a buffer with the old shapes /array of pointers/
	Shape ** buffer = new Shape *[m_ShapesNum];
	for (int i = 0; i < m_ShapesNum - 1; i++)
	{
		//copy shapes to the buffer - change adress in memory
		buffer[i] = m_Shapes[i];
	}
	//all shapes now point to a different location in the memory

	//free old memory
	//NB: no need to deallocate each shape beacause it still exists, but is now in a different container
	delete[] m_Shapes;

	//create a new line
	if (newTag.find("<line") != std::string::npos)
		buffer[m_ShapesNum - 1] = new Line(newTag, index);
	//create a new circle
	if (newTag.find("<circle") != std::string::npos)
		buffer[m_ShapesNum - 1] = new Circle(newTag, index);
	//create a new rect
	if (newTag.find("<rect") != std::string::npos)
		buffer[m_ShapesNum - 1] = new Rect(newTag, index);

	//m_Shapes now points to buffer
	m_Shapes = buffer;

	return *this;
}

//remove a shape together with its corresponding tag in the raw data
ShapeLab& ShapeLab::Erase(int id)
{
	bool found = false;

	//check if a shape with that ID is found
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		if (m_Shapes[i]->GetID() == id)
		{
			found = true;
			break;
		}
	}

	//display a proper message if the shape is not found
	if (!found)
	{
		std::cout << "Shape not found.\n";
		return *this;
	}

	//remove tag from raw data and shift shapes in m_Shapes
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		if (m_Shapes[i]->GetID() == id)
		{
			//erase tag from raw data
			m_Data.RemoveTag(m_Shapes[i]->GetCorrespondingLine());
			//free memory for the shape to be erased
			delete m_Shapes[i];
			for (int j = i; j < m_ShapesNum; ++j)
			{
				//change corresponding lines in raw data for each shape forward
				m_Shapes[j]->DecreaseLine();
				//shift shapes
				m_Shapes[j] = m_Shapes[j + 1];
			}
			break;
		}
	}

	//a buffer without the erased shape
	Shape ** buffer = new Shape *[m_ShapesNum - 1];
	--m_ShapesNum; //decrease shapes
	for (int i = 0; i < m_ShapesNum; ++i)
	{
		//copy shapes to buffer with no need of cloning
		buffer[i] = m_Shapes[i];
	}

	//free old memory
	//NB: no need to deallocate each shape beacause it still exists, but is now in a different container
	delete[] m_Shapes;

	//m_Shapes now points to buffer
	m_Shapes = buffer;

	return *this;
}

//translate a single shape or all shapes
ShapeLab& ShapeLab::Translate(std::string info)
{
	//snippet borrowed by Antoine, a Stack Overflow user 
	//cannonical form for erasing whitespace in a string
	info.erase(std::remove(info.begin(), info.end(), ' '), info.end());

	//extract vertical translation parameters
	int vertical = stoi(info.substr(info.find_first_of("=") + 1, info.find("horizontal=") - info.find_first_of("=")));
	//extract horizontal translation parameters
	int horizontal = stoi(info.substr(info.find_last_of('=') + 1));

	//translate all shapes
	if (info.find("vertical=") == 0)
	{
		//traverse all shapes
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			//translate shapes
			Trans(m_Shapes[i], vertical, horizontal);
			//update raw data
			m_Data.ReplaceTag(m_Shapes[i]->GetCorrespondingLine(), m_Shapes[i]);
		}
	}
	//translate by ID
	else
	{
		//ID of the shape to be translated
		int transID = stoi(info.substr(0, info.find("vertical")));
		bool found = false;

		//traverse all shapes
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			//shape found
			if (m_Shapes[i]->GetID() == transID)
			{ 
				found = true;
				//translate shape
				Trans(m_Shapes[i], vertical, horizontal);
				//update raw data
				m_Data.ReplaceTag(m_Shapes[i]->GetCorrespondingLine(), m_Shapes[i]);
				break;
			}
		}

		//display proper message if a shape with that ID is not found
		if (!found)
		{
			std::cout << "Shape not found\n";
		}
	}
	return *this;
}

//print all shapes completely inside a given shape
void ShapeLab::Within(std::string info) const
{
	//circle
	if (info.find("circle") == 0)
	{
		/*
		The distance between (xc, yc) and (xp, yp) is given by the Pythagorean theorem as
		d = sqrt((xp - xc)^2 + (yp - yc)^2)
		The point (xp, yp) is inside the circle if d^2<r^2 and on the circle if d^2=r^2
		*/

		//temporary shape
		Circle* temp = new Circle(GenerateTag(info), -1);
		//traverse all shapes
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			//line
			if (dynamic_cast<Line*>(m_Shapes[i]) != nullptr)
			{
				//the line is inside the circle
				if (pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX1()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY1()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetX2()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Line*>(m_Shapes[i])->GetY2()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2))
				{
					m_Shapes[i]->Print();
				}
			}

			//circle
			if (dynamic_cast<Circle*>(m_Shapes[i]) != nullptr)
			{
				//the circle is inside the circle
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
				//the rect is inside the circle
				if (pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetWidth()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetWidth()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetHeight()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2) &&
					pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetX()) - stoi(temp->GetCX()), 2) + pow(stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetY()) + stoi(dynamic_cast<Rect*>(m_Shapes[i])->GetHeight()) - stoi(temp->GetCY()), 2) <= pow(stoi(temp->GetR()), 2))
				{
					m_Shapes[i]->Print();
				}
			}
		}
		//free memory
		delete temp;
	}
	//rect
	else if (info.find("rect") == 0)
	{
		//create temporary shape
		Rect* temp = new Rect(GenerateTag(info), -1);
		//traverse all shapes
		for (int i = 0; i < m_ShapesNum; ++i)
		{
			//line
			if (dynamic_cast<Line*>(m_Shapes[i]) != nullptr)
			{
				//the line is inside the rect
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
				//the circle is inside the rect
				if (stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) + stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) <= stoi(temp->GetX()) + stoi(temp->GetWidth()) &&
					stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCX()) - stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) >= stoi(temp->GetX()) &&
					stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) + stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) <= stoi(temp->GetY()) + stoi(temp->GetHeight()) &&
					stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetCY()) - stoi(dynamic_cast<Circle*>(m_Shapes[i])->GetR()) >= stoi(temp->GetY()))
				{
					m_Shapes[i]->Print();
				}
			}
			//rect
			if (dynamic_cast<Rect*>(m_Shapes[i]) != nullptr)
			{
				//the rect is inside the rect
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
		//free memory
		delete temp;
	}
	else
		std::cout << "Shape not supported. \n";
}
