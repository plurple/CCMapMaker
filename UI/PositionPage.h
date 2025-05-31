#pragma once
#include "UIPage.h"

class PositionEntry
{
public:
	sf::RectangleShape borderBox;
	sf::Text* territoryName;
	sf::Text* startLabel;
	TextBox startBox;
	bool selected;

	PositionEntry(float entryTop);
	void Draw(sf::RenderWindow& window, bool selected);
	void MouseClick(sf::Vector2i mousePos);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		std::string keyPressed, bool backspace, bool enter,
		bool showCursor);
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
	void MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

	void AddPosition();

};

