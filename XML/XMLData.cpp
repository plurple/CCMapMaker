#include "XMLData.h"
#include "Continent.h"
#include "Objective.h"
#include "Position.h"
#include "Reinforcement.h"
#include "Territory.h"
#include "Transform.h"
#include <iostream>
#include "../EnumOperators.hpp"
#include "tinyxml2.h"

static const std::vector<std::string> whenStrings =
{
	"Turn",
	"Round"
};

static const std::vector<std::string> whoStrings =
{
	"       Self",
	"    Neutral",
	"    Players",
	" Opponents",
	"    Others",
	"      Team",
	"  Non Team",
	"Team Mates",
	"        All"
};

static const std::vector<std::string> incStrings =
{
	"     Plus",
	"     Minus",
	"Independent"
};

static const std::vector<std::string> conditionTypeStrings =
{
	"    Round",
	"    Player",
	"  Territory",
	"ArmyCount"
};

static const std::vector<std::string> operatorStrings =
{
	"    =",
	"   !=",
	"    >",
	"   >=",
	"    <",
	"   <=",
	"   In",
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

void XMLData::SaveXML()
{
	tinyxml2::XMLDocument doc;
	doc.InsertFirstChild(doc.NewDeclaration());
	
	const char* awesomeMsg = "This XML was made using Plurple's amazing CC Map Maker";
	tinyxml2::XMLComment* awesome = doc.NewComment(awesomeMsg);
	doc.InsertEndChild(awesome);

	tinyxml2::XMLElement* map = doc.NewElement("map");
	doc.InsertEndChild(map);

	int i = 0;
	for (auto territory : territories)
	{
		tinyxml2::XMLElement* terr = doc.NewElement("territory");
		map->InsertEndChild(terr);
		std::string comment = "territory " + std::to_string(i);
		tinyxml2::XMLComment* terrNum = doc.NewComment(comment.c_str());
		terr->InsertEndChild(terrNum);
		tinyxml2::XMLElement* name = doc.NewElement("name");
		name->SetText(territory.second->name.c_str());
		terr->InsertEndChild(name);
		tinyxml2::XMLElement* borders = doc.NewElement("borders");
		terr->InsertEndChild(borders);
		for (auto border : territory.second->borders)
		{
			tinyxml2::XMLElement* bord = doc.NewElement("border");
			bord->SetText(territories.at(border.territory)->name.c_str());
			if (border.condition >= 0)
			{
				auto condition = border.conditionIsContintent ? 
					continents.at(border.condition)->name.c_str() : 
					territories.at(border.condition)->name.c_str();
				bord->SetAttribute("condition", condition);
			}
			borders->InsertEndChild(bord);
		}
		if (territory.second->bombardments.size())
		{
			tinyxml2::XMLElement* bombardments = doc.NewElement("bombardments");
			terr->InsertEndChild(bombardments);
			for (auto bombardement : territory.second->bombardments)
			{
				tinyxml2::XMLElement* bomb = doc.NewElement("bombardment");
				bomb->SetText(territories.at(bombardement)->name.c_str());
				bombardments->InsertEndChild(bomb);
			}
		}
		tinyxml2::XMLElement* coords = doc.NewElement("coordinates");
		terr->InsertEndChild(coords);
		tinyxml2::XMLElement* smallx = doc.NewElement("smallx");
		smallx->SetText(territory.second->smallPos.x);
		coords->InsertEndChild(smallx);
		tinyxml2::XMLElement* smally = doc.NewElement("smally");
		smally->SetText(territory.second->smallPos.y);
		coords->InsertEndChild(smally);
		tinyxml2::XMLElement* largex = doc.NewElement("largex");
		largex->SetText(territory.second->largePos.x);
		coords->InsertEndChild(largex);
		tinyxml2::XMLElement* largey = doc.NewElement("largey");
		largey->SetText(territory.second->largePos.y);
		coords->InsertEndChild(largey);

		if (territory.second->neutral != -1)
		{
			tinyxml2::XMLElement* neutral = doc.NewElement("neutral");
			if (territory.second->killer)
				neutral->SetAttribute("killer", "yes");
			neutral->SetText(territory.second->neutral);
			terr->InsertEndChild(neutral);
		}
		if (territory.second->bonus != -1)
		{
			tinyxml2::XMLElement* bonus = doc.NewElement("bonus");
			bonus->SetText(territory.second->bonus);
			terr->InsertEndChild(bonus);
		}

		i++;
	}

	tinyxml2::XMLComment* awesome1 = doc.NewComment(awesomeMsg);
	doc.InsertEndChild(awesome1);

	doc.SaveFile("Output.xml");
}