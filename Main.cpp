#include "RawData.h"
#include "Line.h"
#include "ShapeLab.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <regex>


void ToggleMenu(ShapeLab* shl)
{
	std::regex R_LINE("line [[:digit:]]+ [[:digit:]]+ [[:digit:]]+ [[:digit:]]+ ([a-z]+)?");
	std::regex R_CIRCLE("circle [[:digit:]]+ [[:digit:]]+ [[:digit:]]+ [a-z]+ [a-z]+");
	std::regex R_RECT("rect [[:digit:]]+ [[:digit:]]+ [[:digit:]]+ [[:digit:]]+ [a-z]+ [a-z]+");
	std::regex R_ID("[[:digit:]]+");

	std::regex R_TRANS("translate \([[:digit:]]+ )?vertical=(-)?[[:digit:]]+ horizontal=(-)?[[:digit:]]+");

	std::cout << "Command: ";
	std::string command;
	std::getline(std::cin, command);

	if (command.find("open ") == 0)
	{
		if (command.find(".svg") == command.length() - 4) //".svg".length()=4
		{
			delete shl;
			shl = new ShapeLab(command.substr(command.find_first_of(' ') + 1));
		}
		else
			std::cout << "Invalid file type.\n";
	}
	else if (command == "close" && shl)
	{
		delete shl;
		shl = nullptr;
		std::cout << "Buffer cleared.\n";
	}
	else if (command == "save" && shl)
	{
		shl->GetRawData().Save();
	}
	else if (command.find("save as ") == 0 && shl)
	{
		if (command.find(".svg")==command.length()-4) //".svg".length()=4
			shl->GetRawData().SaveAs(command.substr(8)); // "save as ".length() = 8
		else
			std::cout << "Invalid filetype.\n";
	}
	else if (command == "print" && shl)
	{
		shl->Print();
		std::cout << "Command complete.\n";
	}
	else if (command.find("create ") == 0 && shl &&
		(std::regex_match(command.substr(command.find_first_of(' ') + 1), R_LINE) ||
			std::regex_match(command.substr(command.find_first_of(' ') + 1), R_CIRCLE) ||
			std::regex_match(command.substr(command.find_first_of(' ') + 1), R_RECT)))
	{
		shl->Create(command.substr(command.find_first_of(' ') + 1));
	}
	else if (command.find("erase ") == 0 && shl &&
		std::regex_match(command.substr(command.find_first_of(' ') + 1), R_ID))
	{
		shl->Erase(stoi(command.substr(command.find_first_of(' ') + 1)));
	}
	else if (std::regex_match(command, R_TRANS) && shl)
	{
		shl->Translate(command.substr(command.find_first_of(' ') + 1));
	}
	else if (command.find("within ") == 0 && shl &&
		(std::regex_match(command.substr(command.find_first_of(' ') + 1), R_LINE) ||
			std::regex_match(command.substr(command.find_first_of(' ') + 1), R_CIRCLE) ||
			std::regex_match(command.substr(command.find_first_of(' ') + 1), R_RECT)))
	{
		shl->Within(command.substr(command.find_first_of(' ') + 1));
	}
	else if (command == "exit")
	{
		std::cout << "Exiting program. \n";
		return;
	}
	else
		std::cout << "Invalid command.\n";

	ToggleMenu(shl);
}

int main() 
{
	ShapeLab* shl = nullptr;
	ToggleMenu(shl);
	delete shl;
	return 0;
}
