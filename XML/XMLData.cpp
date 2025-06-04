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

//Transform* XMLData::AddTransform()
//{
//
//}

int XMLData::AddReinforcement()
{
	int key = nextKey[(int)UIPageType::Reinforcement]++;
	reinforcements.insert({ key, new Reinforcement(*this) });
	return key;
}

void XMLData::RemoveReinforcement(int key)
{
	reinforcements.erase(key);
}

//Position* XMLData::AddPosition()
//{
//
//}
//
//Objective* XMLData::AddRequirement()
//{
//	
//}
//
//Objective* XMLData::AddObjective()
//{
//
//}
//
//Continent* XMLData::AddContinent()
//{
//
//}
//
int XMLData::AddTerritory()
{
	int key = nextKey[(int)UIPageType::Territory]++;
	territories.insert({ key, new Territory() });
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
		break;
	case UIPageType::Position:
		break;
	case UIPageType::Requirements:
		break;
	case UIPageType::Objective:
		break;
	case UIPageType::Reinforcement:
		RemoveReinforcement(key);
		break;
	case UIPageType::Transform:
		break;
	}
}