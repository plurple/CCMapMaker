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
//Territory* XMLData::AddTerritory()
//{
//
//}
