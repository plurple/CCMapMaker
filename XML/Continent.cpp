#include "Continent.h"

Continent::Continent() :
	name{""}
{
	BonusData bonus{};
	bonuses.push_back(bonus);
}