#include "Continent.h"

Continent::Continent() :
	name{""},
	bonusKey{0}
{
	BonusData bonus{};
	bonuses.insert({ bonusKey++, bonus });
}