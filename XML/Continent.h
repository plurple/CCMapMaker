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
	std::vector<unsigned int> mandatory; //this will be an index in the territories vector
	std::vector<unsigned int> blocker; //this will be an index in the territories vector
	std::vector<MultiplierData> multiplier; //this will be an index in the territories vector
	std::vector<unsigned int> overrides;
};

