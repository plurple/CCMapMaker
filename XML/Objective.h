#pragma once
#include <string>
#include <vector>

class Objective
{
public:
	std::string name;
	std::vector<int> territories;
	std::vector<int> continents;
	int numRequired;

	Objective() : name{ "" }, numRequired{ 1 } {};
};

