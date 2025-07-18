#pragma once
#include <SFML/System.hpp>
#include <vector>

struct BorderData
{
	int territory;
	int condition;
	bool conditionIsContintent = false;
};

class Territory
{
public:
	std::string name;
	sf::Vector2i smallPos;
	sf::Vector2i largePos;
	std::vector<BorderData> borders;
	std::vector<int> bombardments;
	int neutral;
	int bonus;
	bool killer;

	Territory(sf::Vector2f small = { 0,0 }, sf::Vector2f large = { 0,0 });
};

