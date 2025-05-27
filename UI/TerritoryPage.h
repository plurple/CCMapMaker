#pragma once
#include "UIPage.h"
#include "Button.h"

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
	sf::Text* connections;
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
	std::vector<TerritoryEntry> entries;
};

