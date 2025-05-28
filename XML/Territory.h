#pragma once
#include <SFML/System.hpp>
#include <vector>

struct BorderData
{
	unsigned int territory;
	unsigned int condition;
	bool conditionIsContintent;
};

class Territory
{
public:
	unsigned int index;
	std::string name;
	sf::Vector2u smallPos;
	sf::Vector2u LargePos;
	std::vector<BorderData> borders;
	std::vector<unsigned int> bombardments;
	unsigned int neutral;
	bool killer;
	int bonus;

	//Territory() {};
	/*todo move the positions and such both together and seperatly
	convert large coord to small and vice versa*/
};

