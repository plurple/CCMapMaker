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
public:
	Button addReinforcement;
	std::vector<ReinforcementEntry> entries;
	ReinforcementPage(sf::Font& font, sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

};

