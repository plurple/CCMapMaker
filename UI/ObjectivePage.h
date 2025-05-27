#pragma once
#include "UIPage.h"

class ObjectiveEntry
{
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;
	sf::Text* territoryLabel;
	sf::Text* territories;
	sf::Text* continentLabel;
	sf::Text* continents;
	sf::Text* requiredLabel;
	TextBox numRequiredBox;
};

class ObjectivePage : public UIPage
{
	Button addObjective;
	Button showContinents;
	std::vector<ObjectiveEntry> entries;
};

