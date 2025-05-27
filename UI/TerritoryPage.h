#pragma once
#include "UIPage.h"

struct Condition
{
	sf::Text* territory;
	Button addCondition;
	sf::Text* condition;
};

class TerritoryEntry
{
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;
	sf::Text* xLabel;
	TextBox xBox;
	sf::Text* yLabel;
	TextBox yBox;
	sf::Text* connectionLabel;
	std::vector<Condition> territories;
	sf::Text* bombardments;
	Button Killer;
	sf::Text* neutralLabel;
	TextBox neutralBox;
	sf::Text* bonusLabel;
	TextBox bonusBox;
};

class TerritoryPage : public UIPage
{
public:
	Button borders;
	Button Bombardments;
	Button Other;
	Button addTerritory;
	Button showContinents;
	Button conditions;
	Button linkCoordinates;
	std::vector<TerritoryEntry> entries;
};

