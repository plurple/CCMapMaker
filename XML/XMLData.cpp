#include "XMLData.h"
#include "Continent.h"
#include "Objective.h"
#include "Position.h"
#include "Reinforcement.h"
#include "Territory.h"
#include "Transform.h"
#include <iostream>
#include "../EnumOperators.hpp"

static const std::vector<std::string> whenStrings =
{
	"Turn",
	"Round"
};

static const std::vector<std::string> whoStrings =
{
	"  Self",
	" Neutral",
	" Players",
	"Opponents",
	"  Others",
	"   Team",
	" Non Team",
	"Team Mates",
	"   All",
};

static const std::vector<std::string> incStrings =
{
	"   Plus",
	"   Minus",
	"Independent"
};

static const std::vector<std::string> conditionTypeStrings =
{
	"  Round",
	"  Player",
	"Territory",
	"ArmyCount"
};

static const std::vector<std::string> operatorStrings =
{
	"  =",
	"  !=",
	"  >",
	"  >=",
	"  <",
	"  <=",
	"  In",
	"Not In"
};

static const std::vector<std::vector<std::string>> enumStrings =
{
	whenStrings,
	whoStrings,
	incStrings,
	conditionTypeStrings,
	operatorStrings
};

XMLData::XMLData() :
	minReinforcements{3},
	maxPositions{-1}
{
	for (int i = 0; i < (int)UIPageType::COUNT; i++)
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

std::string XMLData::GetTransformOptionString(TransformOptionType type, int option)
{
	return enumStrings[(int)type][option];
}

void XMLData::ChangeTransformOption(TransformOptionType type, int& option, bool plus, bool skipAll)
{
	switch (type)
	{
	case TransformOptionType::When:
	{
		TransformType temp = (TransformType)option;
		plus ? temp++ : temp--;
		option = (int)temp;
		break;
	}
	case TransformOptionType::Who:
	{
		ApplyTo temp = (ApplyTo)option;
		plus ? temp++ : temp--;
		if (skipAll && temp == ApplyTo::All)
			plus ? temp++ : temp--;
		option = (int)temp;
		break;
	}
	case TransformOptionType::INC:
	{
		IncType temp = (IncType)option;
		plus ? temp++ : temp--;
		option = (int)temp;
		break;
	}
	case TransformOptionType::ConditionType:
	{
		ConditionType temp = (ConditionType)option;
		plus ? temp++ : temp--;
		option = (int)temp;
		break;
	}
	case TransformOptionType::Operator:
	{
		Operators temp = (Operators)option;
		plus ? temp++ : temp--;
		option = (int)temp;
		break;
	}
	}
}
