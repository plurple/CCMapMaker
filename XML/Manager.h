#pragma once
#include <vector>

class Transform;
class Reinforcement;
class Position;
class Objective;
class Continent;
class Territory;

class Manager
{
public:
	std::vector<Transform*> transforms;
	std::vector<Reinforcement*> reinforcements;
	int maxPositions;
	std::vector<Position*> positions;
	std::vector<Objective*> requirements; //by default required is 1
	std::vector<Objective*> objectives; //by default required is all
	std::vector<Continent*> continents;
	std::vector<Territory*> territories;
};

