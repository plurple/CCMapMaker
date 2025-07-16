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
#include "../UI/UI.h"
#include "../UI/UIPage.h"
#include "../UI/ContinentPage.h"
#include "../UI/TerritoryPage.h"
#include "../UI/ObjectivePage.h"
#include "../UI/ReinforcementPage.h"
#include "../UI/TransformPage.h"
#include "../UI/PositionPage.h"

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
	int i = 0;
	tinyxml2::XMLDocument doc;
	doc.InsertFirstChild(doc.NewDeclaration());
	
	const char* awesomeMsg = "This XML was made using Plurple's amazing CC Map Maker";
	tinyxml2::XMLComment* awesome = doc.NewComment(awesomeMsg);
	doc.InsertEndChild(awesome);

	tinyxml2::XMLElement* map = doc.NewElement("map");
	doc.InsertEndChild(map);

	if (minReinforcements != 3)
	{
		tinyxml2::XMLElement* minreinforcement = doc.NewElement("minreinforcement");
		minreinforcement->SetText(minReinforcements);
		map->InsertEndChild(minreinforcement);
	}

	i = 0;
	if (transforms.size())
	{
		//do this shit
		for (auto transform : transforms)
		{
			i++;
		}
	}

	if (reinforcements.size())
	{
		tinyxml2::XMLElement* reins = doc.NewElement("reinforcements");
		map->InsertEndChild(reins);
		for (auto reinforcment : reinforcements)
		{
			tinyxml2::XMLElement* rein = doc.NewElement("reinforcement");
			reins->InsertEndChild(rein);
			tinyxml2::XMLElement* lower = doc.NewElement("lower");
			lower->SetText(reinforcment.second->lower);
			rein->InsertEndChild(lower);
			tinyxml2::XMLElement* upper = doc.NewElement("upper");
			upper->SetText(reinforcment.second->upper);
			rein->InsertEndChild(upper);
			tinyxml2::XMLElement* divisor = doc.NewElement("divisor");
			divisor->SetText(reinforcment.second->divisor);
			rein->InsertEndChild(divisor);
		}
	}

	i = 0;
	if (positions.size())
	{
		tinyxml2::XMLElement* poss = doc.NewElement("positions");
		if (maxPositions != -1)
		{
			poss->SetAttribute("max", maxPositions);
		}
		map->InsertEndChild(poss);
		for (auto position : positions)
		{
			tinyxml2::XMLElement* pos = doc.NewElement("position");
			poss->InsertEndChild(pos);
			std::string comment = "position " + std::to_string(i);
			tinyxml2::XMLComment* posNum = doc.NewComment(comment.c_str());
			pos->InsertEndChild(posNum);
			for (auto posPair : position.second->positions)
			{
				tinyxml2::XMLElement* terr = doc.NewElement("territory");
				terr->SetText(territories.at(posPair.first)->name.c_str());
				if (posPair.second != 3)
					terr->SetAttribute("start", posPair.second);
				pos->InsertEndChild(terr);
			}
			i++;
		}
	}

	i = 0;
	for (auto requirement : requirements)
	{
		tinyxml2::XMLElement* req = doc.NewElement("requirement");
		map->InsertEndChild(req);
		std::string comment = "requirement " + std::to_string(i);
		tinyxml2::XMLComment* reqNum = doc.NewComment(comment.c_str());
		req->InsertEndChild(reqNum);
		tinyxml2::XMLElement* name = doc.NewElement("name");
		name->SetText(requirement.second->name.c_str());
		req->InsertEndChild(name);

		tinyxml2::XMLElement* components = doc.NewElement("components");
		req->InsertEndChild(components);
		for (auto territory : requirement.second->territories)
		{
			tinyxml2::XMLElement* terr = doc.NewElement("territory");
			terr->SetText(territories.at(territory)->name.c_str());
			components->InsertEndChild(terr);
		}
		for (auto continent : requirement.second->continents)
		{
			tinyxml2::XMLElement* con = doc.NewElement("continent");
			con->SetText(continents.at(continent)->name.c_str());
			components->InsertEndChild(con);
		}

		if (requirement.second->numRequired != -1 &&
			requirement.second->numRequired != 1 )
		{
			tinyxml2::XMLElement* required = doc.NewElement("required");
			required->SetText(requirement.second->numRequired);
			req->InsertEndChild(required);
		}
	}

	i = 0;
	for (auto objective : objectives)
	{
		tinyxml2::XMLElement* obj = doc.NewElement("objective");
		map->InsertEndChild(obj);
		std::string comment = "objective " + std::to_string(i);
		tinyxml2::XMLComment* objNum = doc.NewComment(comment.c_str());
		obj->InsertEndChild(objNum);
		tinyxml2::XMLElement* name = doc.NewElement("name");
		name->SetText(objective.second->name.c_str());
		obj->InsertEndChild(name);

		tinyxml2::XMLElement* components = doc.NewElement("components");
		obj->InsertEndChild(components);
		for (auto territory : objective.second->territories)
		{
			tinyxml2::XMLElement* terr = doc.NewElement("territory");
			terr->SetText(territories.at(territory)->name.c_str());
			components->InsertEndChild(terr);
		}
		for (auto continent : objective.second->continents)
		{
			tinyxml2::XMLElement* con = doc.NewElement("continent");
			con->SetText(continents.at(continent)->name.c_str());
			components->InsertEndChild(con);
		}

		if (objective.second->numRequired != -1 &&
			objective.second->numRequired !=
			(objective.second->territories.size() +
				objective.second->continents.size()))
		{
			tinyxml2::XMLElement* required = doc.NewElement("required");
			required->SetText(objective.second->numRequired);
			obj->InsertEndChild(required);
		}
	}

	i = 0;
	for (auto continent : continents)
	{
		tinyxml2::XMLElement* cont = doc.NewElement("continent");
		map->InsertEndChild(cont);
		std::string comment = "continent " + std::to_string(i);
		tinyxml2::XMLComment* contNum = doc.NewComment(comment.c_str());
		cont->InsertEndChild(contNum);
		tinyxml2::XMLElement* name = doc.NewElement("name");
		name->SetText(continent.second->name.c_str());
		cont->InsertEndChild(name);
		if (continent.second->bonuses.size() > 1)
		{
			tinyxml2::XMLElement* bonuses = doc.NewElement("bonuses");
			cont->InsertEndChild(bonuses);
		
			for (auto bonus : continent.second->bonuses)
			{
				tinyxml2::XMLElement* bon = doc.NewElement("bonus");
				bon->SetText(bonus.second.bonusAmount);
				if (bonus.second.numRequired > 0)
				{
					bon->SetAttribute("required", bonus.second.numRequired);
				}
				bonuses->InsertEndChild(bon);
			}
		}
		else
		{
			tinyxml2::XMLElement* bon = doc.NewElement("bonus");
			bon->SetText(continent.second->bonuses.at(0).bonusAmount);
			if (continent.second->bonuses.at(0).numRequired > 0)
			{
				bon->SetAttribute("required", continent.second->bonuses.at(0).numRequired);
			}
			cont->InsertEndChild(bon);
		}

		tinyxml2::XMLElement* components = doc.NewElement("components");
		cont->InsertEndChild(components);
		
		for (auto territory : continent.second->territories)
		{
			tinyxml2::XMLElement* terr = doc.NewElement("territory");
			terr->SetText(territories.at(territory.first)->name.c_str());
			if (territory.second.blocker)
				terr->SetAttribute("type", "blocker");
			else if (territory.second.mandatory)
				terr->SetAttribute("type", "mandatory");
			else if (territory.second.multiplier)
			{
				terr->SetAttribute("type", "multiplier");
				terr->SetAttribute("factor", territory.second.factor);
			}
			components->InsertEndChild(terr);
		}
		for (auto continent : continent.second->continents)
		{
			tinyxml2::XMLElement* con = doc.NewElement("continent");
			con->SetText(continents.at(continent)->name.c_str());
			components->InsertEndChild(con);
		}
		if (continent.second->required != -1)
		{
			tinyxml2::XMLElement* required = doc.NewElement("required");
			required->SetText(continent.second->required);
			cont->InsertEndChild(required);
		}

		if (continent.second->overrides.size())
		{
			tinyxml2::XMLElement* overrides = doc.NewElement("overrides");
			cont->InsertEndChild(overrides);
			for (auto over : continent.second->overrides)
			{
				tinyxml2::XMLElement* overr = doc.NewElement("override");
				overr->SetText(continents.at(over)->name.c_str());
				overrides->InsertEndChild(overr);
			}
		}
		i++;
	}

	i = 0;
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
		if (territory.second->bonus != 0)
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

void XMLData::LoadXML(UI& ui, Maps& maps)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Output.xml");

	auto terrUIPage = std::dynamic_pointer_cast<TerritoryPage>(ui.uiPages[(int)UIPageType::Territory]);

	tinyxml2::XMLElement* map = doc.FirstChildElement("map");

	std::unordered_map<std::string, int> territoryToIndex;
	int i = 0;
	for (tinyxml2::XMLElement* terrElem = map->FirstChildElement("territory");
		terrElem != nullptr;
		terrElem = terrElem->NextSiblingElement("territory"))
	{
		int terrKey = nextKey.at((int)UIPageType::Territory);
		terrUIPage->AddTerritory(*this, maps.AddMapBox({ 0, 0 }));

		auto territory = territories.at(terrKey);
		
		auto name = terrElem->FirstChildElement("name");
		if (name && name->GetText())
		{
			territory->name = name->GetText();
			territoryToIndex.insert({ territory->name, i });
		}

		auto coords = terrElem->FirstChildElement("coordinates");
		if (coords)
		{
			coords->FirstChildElement("smallx")->QueryIntText(&territory->smallPos.x);
			coords->FirstChildElement("smally")->QueryIntText(&territory->smallPos.y);
			coords->FirstChildElement("largex")->QueryIntText(&territory->largePos.x);
			coords->FirstChildElement("largey")->QueryIntText(&territory->largePos.y);
		}

		auto neutral = terrElem->FirstChildElement("neutral");
		if (neutral)
		{
			neutral->QueryIntText(&territory->neutral);
			const char* killer = neutral->Attribute("killer");
			territory->killer = (killer && strcmp(killer, "yes") == 0);
		}

		auto bonus = terrElem->FirstChildElement("bonus");
		if (bonus)
		{
			bonus->QueryIntText(&territory->bonus);
		}
		i++;
	}

	for (tinyxml2::XMLElement* terrElem = map->FirstChildElement("territory");
		terrElem != nullptr;
		terrElem = terrElem->NextSiblingElement("territory"))
	{
		auto name = terrElem->FirstChildElement("name");
		if (!name || !name->GetText())
		{
			continue;
		}

		int index = territoryToIndex.at(name->GetText());
		auto terrUI = std::dynamic_pointer_cast<TerritoryEntry>(terrUIPage->entries[index]);

		auto borders = terrElem->FirstChildElement("borders");
		if (borders)
		{
			int j = 0;
			for (tinyxml2::XMLElement* borderElem = borders->FirstChildElement("border");
				borderElem != nullptr;
				borderElem = borderElem->NextSiblingElement("border"))
			{
				std::string name = borderElem->GetText();
				int borderIndex = territoryToIndex.at(name);
				terrUI->AddBorder(*this, maps, borderIndex, terrUIPage->entries[borderIndex]->xmlKey);
				const char* condition = borderElem->Attribute("condition");
				if (condition)
				{
					std::string conditionName(condition);
					if (territoryToIndex.count(conditionName))
					{
						int conditionIndex = territoryToIndex.at(conditionName);
						terrUI->AddCondition(*this, maps.mapBoxes[conditionIndex]->border, conditionIndex, terrUIPage->entries[conditionIndex]->xmlKey, false, j);
					}
					/*else if (continentToIndex.count(conditionName))
					{
						int conditionIndex = continentToIndex.at(conditionName);
						terrUI->AddCondition(*this, panel.continents[conditionIndex]->border, conditionIndex, contUIPage->entries[conditionIndex]->xmlKey, true, j);
					}*/
				}
				j++;
			}
		}

		auto bombardments = terrElem->FirstChildElement("bombardments");
		if (bombardments)
		{
			for (tinyxml2::XMLElement* bombElem = bombardments->FirstChildElement("bombardment");
				bombElem != nullptr;
				bombElem = bombElem->NextSiblingElement("bombardment"))
			{
				std::string name = bombElem->GetText();
				int bombIndex = territoryToIndex.at(name);
				terrUI->AddBombardment(*this, maps, bombIndex, terrUIPage->entries[bombIndex]->xmlKey);
			}
		}
		terrUI->Unselect(true);
		terrUI->BorderBoxSize();
	}
	terrUIPage->PositionEntries();
}