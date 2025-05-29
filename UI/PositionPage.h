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
public:
	Button addPosition;
	sf::Text* maxLabel;
	TextBox maxBox;
	std::vector<PositionEntry> entries;

	PositionPage(sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

};

