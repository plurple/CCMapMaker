#pragma once
#include <string>
#include <vector>

struct BonusData
{
	unsigned int bonusAmount;
	unsigned int numRequired;
};

struct MultiplierData
{
	unsigned int territory;
	float factor;
};

class Continent
{
public:
	unsigned int index;
	std::string name;
	std::vector<BonusData> bonuses;
	std::vector<unsigned int> territories;
	std::vector<unsigned int> continents;
	std::vector<unsigned int> mandatory;
	std::vector<unsigned int> blocker;
	std::vector<MultiplierData> multiplier;
	std::vector<unsigned int> overrides;
};

