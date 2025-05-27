#pragma once
#include "UIPage.h"

class AdvancedTerritory
{
	sf::Text* territory;
	Button madatory;
	Button blocker;
	Button multiplier;
	TextBox factor;
};

class ContinentEntry
{
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;
	sf::Text* bonusLabel;
	TextBox bonusBox;
	Button requiredButton;
	sf::Text* requiredLabel;
	TextBox requiredBox;
	sf::Text* territoryLabel;
	sf::Text* territories;
	std::vector<AdvancedTerritory> advanced;
	sf::Text* continentLabel;
	sf::Text* overideLabel;
	sf::Text* continents;
};

class ContinentPage : public UIPage
{
	Button addContinent;
	Button showContinents;
	Button overidesView; //chekbox to swap between overides and normal
	Button advancedView; //checkbox to swap to advance version of territories
	std::vector<ContinentEntry> entries;
};

