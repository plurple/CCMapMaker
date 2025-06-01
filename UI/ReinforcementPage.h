#pragma once
#include "UIPage.h"

class ReinforcementEntry
{
public:
	sf::RectangleShape borderBox;
	sf::Text* lowerLabel;
	TextBox lowerBox;
	sf::Text* upperLabel;
	TextBox upperBox;
	sf::Text* divisorLabel;
	TextBox divisorBox;
	sf::Text* explanation;

	ReinforcementEntry(float entryTop);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		std::string keyPressed, bool backspace, bool enter,
		bool showCursor, float scrolled);
	void MoveEntry(sf::Vector2f offset);
};

class ReinforcementPage : public UIPage
{
public:
	Button addReinforcement;
	sf::Text* minLabel;
	TextBox minReinforcements;
	std::vector<ReinforcementEntry> entries;
	ReinforcementPage(sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor, bool verticle, float scrolled) override;

	void AddReinforcement();
};

