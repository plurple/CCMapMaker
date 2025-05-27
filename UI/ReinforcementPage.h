#pragma once
#include "UIPage.h"

class ReinforcementEntry
{
	sf::RectangleShape borderBox;
	sf::Text* lowerLabel;
	TextBox lowerBox;
	sf::Text* upperLabel;
	TextBox upperBox;
	sf::Text* divisorLabel;
	TextBox divisorBox;
	sf::Text* explanation;
};

class ReinforcementPage : public UIPage
{
	Button addReinforcement;
	std::vector<ReinforcementEntry> entries;
};

