#pragma once
#include <string>
#include <vector>

class BonusData
{
public:
	int bonusAmount; //can be negative
	int numRequired;

	BonusData(int bonus = 5, int required = -1) : 
		bonusAmount{ bonus }, numRequired{ required } {};
};

class AdvancedData
{
public:
	int territory;
	bool mandatory;
	bool blocker;
	bool multiplier;
	float factor; //can be negative and a float uggh

	AdvancedData(int terr = -1) : territory{ terr }, mandatory{ false },
		blocker{ false }, multiplier{ false }, factor{ 1.0f } {};
};

class Continent
{
public:
	std::string name;
	std::vector<BonusData> bonuses;
	std::vector<AdvancedData> territories;
	std::vector<int> continents;
	std::vector<int> overrides;

	Continent();
};

