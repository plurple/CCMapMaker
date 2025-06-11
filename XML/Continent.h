#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class BonusData
{
public:
	int bonusAmount; //can be negative
	int numRequired;

	BonusData(int bonus = -1, int required = -1) : 
		bonusAmount{ bonus }, numRequired{ required } {};
};

class AdvancedData
{
public:
	bool mandatory;
	bool blocker;
	bool multiplier;
	float factor; //can be negative and a float uggh

	AdvancedData() : mandatory{ false },
		blocker{ false }, multiplier{ false }, factor{ 1.0f } {};
};

class Continent
{
public:
	std::string name;
	int bonusKey;
	std::unordered_map<int, BonusData> bonuses;
	std::unordered_map<int, AdvancedData> territories;
	std::vector<int> continents;
	std::vector<int> overrides;

	Continent();
	int AddBonus();
};

