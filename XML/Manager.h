#pragma once
#include <unordered_map>

class Transform;
class Reinforcement;
class Position;
class Objective;
class Continent;
class Territory;

class Manager
{
public:
	std::unordered_map<int, Transform*> transforms;
	std::unordered_map<int, Reinforcement*> reinforcements;
	int maxPositions;
	std::unordered_map<int, Position*> positions;
	std::unordered_map<int, Objective*> requirements; //by default required is 1
	std::unordered_map<int, Objective*> objectives; //by default required is all
	std::unordered_map<int, Continent*> continents;
	std::unordered_map<int, Territory*> territories;
};

