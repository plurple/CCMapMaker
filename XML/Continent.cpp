#include "Continent.h"

Continent::Continent() :
	name{""},
	bonusKey{0},
	required{-1}
{}

int Continent::AddBonus()
{
	BonusData bonus{};
	bonuses.insert({ bonusKey, bonus });
	return bonusKey++;
}