#pragma once
#include <unordered_map>
#include <memory>
#include "Transform.h"

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
	COUNT
};

class XMLData
{
public:
	std::vector<int> nextKey;
	std::unordered_map<int, std::shared_ptr<Transform>> transforms;
	int minReinforcements;
	std::unordered_map<int, std::shared_ptr<Reinforcement>> reinforcements;
	int maxPositions;
	std::unordered_map<int, std::shared_ptr<Position>> positions;
	std::unordered_map<int, std::shared_ptr<Objective>> requirements; //by default required is 1
	std::unordered_map<int, std::shared_ptr<Objective>> objectives; //by default required is all
	std::unordered_map<int, std::shared_ptr<Continent>> continents;
	std::unordered_map<int, std::shared_ptr<Territory>> territories;

	~XMLData() {};
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

	std::string GetTransformOptionString(TransformOptionType type, int option);
	void ChangeTransformOption(TransformOptionType type, int& option, bool plus, bool skipAll = false);
};

