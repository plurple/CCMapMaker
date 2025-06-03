#include "Reinforcement.h"

Reinforcement::Reinforcement(XMLData& xmlData) :
	lower{1},
	upper{(int)xmlData.territories.size()},
	divisor{3}
{

}