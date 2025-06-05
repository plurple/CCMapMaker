#include "XMLData.h"
#include "Continent.h"
#include "Objective.h"
#include "Position.h"
#include "Reinforcement.h"
#include "Territory.h"
#include "Transform.h"

XMLData::XMLData() :
	minReinforcements{3},
	maxPositions{-1}
{
	for (int i = 0; i < (int)UIPageType::NumPageTypes; i++)
	{
		nextKey.push_back(0);
	}
}

int XMLData::AddTransform()
{
	int key = nextKey[(int)UIPageType::Transform]++;
	transforms.insert({ key, std::make_shared<Transform>() });
	return key;
}

void XMLData::RemoveTransform(int key)
{
	transforms.erase(key);
}

int XMLData::AddReinforcement()
{
	int key = nextKey[(int)UIPageType::Reinforcement]++;
	reinforcements.insert({ key, std::make_shared<Reinforcement>(*this) });
	return key;
}

void XMLData::RemoveReinforcement(int key)
{
	reinforcements.erase(key);
}

int XMLData::AddPosition()
{
	int key = nextKey[(int)UIPageType::Position]++;
	positions.insert({ key, std::make_shared < Position>() });
	return key;
}

void XMLData::RemovePosition(int key)
{
	positions.erase(key);
}

int XMLData::AddRequirement()
{
	int key = nextKey[(int)UIPageType::Requirements]++;
	requirements.insert({ key, std::make_shared < Objective>() });
	return key;
}

void XMLData::RemoveRequirement(int key)
{
	requirements.erase(key);
}

int XMLData::AddObjective()
{
	int key = nextKey[(int)UIPageType::Objective]++;
	objectives.insert({ key, std::make_shared < Objective>() });
	return key;
}

void XMLData::RemoveObjective(int key)
{
	objectives.erase(key);
}

int XMLData::AddContinent()
{
	int key = nextKey[(int)UIPageType::Continent]++;
	continents.insert({ key, std::make_shared < Continent>() });
	return key;
}

void XMLData::RemoveContinent(int key)
{
	continents.erase(key);
}

int XMLData::AddTerritory()
{
	int key = nextKey[(int)UIPageType::Territory]++;
	territories.insert({ key, std::make_shared < Territory>() });
	return key;
}

void XMLData::RemoveTerritory(int key)
{
	territories.erase(key);
}

void XMLData::RemoveData(UIPageType type, int key)
{
	switch (type)
	{
	case UIPageType::Territory:
		RemoveTerritory(key);
		break;
	case UIPageType::Continent:
		RemoveContinent(key);
		break;
	case UIPageType::Position:
		RemovePosition(key);
		break;
	case UIPageType::Requirements:
		RemoveRequirement(key);
		break;
	case UIPageType::Objective:
		RemoveObjective(key);
		break;
	case UIPageType::Reinforcement:
		RemoveReinforcement(key);
		break;
	case UIPageType::Transform:
		RemoveTransform(key);
		break;
	}
}