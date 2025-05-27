#pragma once
#include <string>
#include <vector>

class Objective
{
public:
	std::string name;
	std::vector<unsigned int> territories;
	std::vector<unsigned int> continents;
	unsigned int numRequired;
};

