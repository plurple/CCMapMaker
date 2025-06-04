#pragma once
#include <unordered_map>

class Transform;
class Reinforcement;
class Position;
class Objective;
class Continent;
class Territory;

enum class UIPageType
{
	Territory,
	Continent,
	Position,
	Requirements,
	Objective,
	Reinforcement,
	Transform,
	NumPageTypes
};

class XMLData
{
public:
	std::vector<int> nextKey;
	std::unordered_map<int, Transform*> transforms;
	int minReinforcements;
	std::unordered_map<int, Reinforcement*> reinforcements;
	int maxPositions;
	std::unordered_map<int, Position*> positions;
	std::unordered_map<int, Objective*> requirements; //by default required is 1
	std::unordered_map<int, Objective*> objectives; //by default required is all
	std::unordered_map<int, Continent*> continents;
	std::unordered_map<int, Territory*> territories;

	XMLData();
	int	AddTransform();
	void RemoveTransform(int key);
	int	AddReinforcement();
	void RemoveReinforcement(int key);
	int	AddPosition();
	void RemovePosition(int key);
	int	AddRequirement();
	void RemoveRequirement(int key);
	int	AddObjective();
	void RemoveObjective(int key);
	int	AddContinent();
	void RemoveContinent(int key);
	int	AddTerritory();
	void RemoveTerritory(int key);

	void RemoveData(UIPageType type, int key);
};

