#include "Territory.h"

Territory::Territory(sf::Vector2f small, sf::Vector2f large) :
	name{ "" },
	smallPos{ small },
	largePos{ large },
	neutral{ -1 },
	bonus{ -1 },
	killer{ false }
{}

