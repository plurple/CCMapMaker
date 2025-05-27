#pragma once
#include "UIPage.h"

class PositionEntry
{
	sf::RectangleShape borderBox;
	sf::Text* territoryName;
	sf::Text* startLabel;
	TextBox startBox;
};

class PositionPage : public UIPage
{
	Button addPosition;
	sf::Text* maxLabel;
	TextBox maxBox;
	std::vector<PositionEntry> entries;
};

