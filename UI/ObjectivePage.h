#pragma once
#include "UIPage.h"

class ObjectiveEntry
{
public:
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;
	sf::Text* territoryLabel;
	sf::Text* territories;
	sf::Text* continentLabel;
	sf::Text* continents;
	sf::Text* requiredLabel;
	TextBox numRequiredBox;
	bool selected;

	ObjectiveEntry(float entryTop, bool isObjective);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos, bool isObjective);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		std::string keyPressed, bool backspace, bool enter,
		bool showCursor);

	void MoveEntry(sf::Vector2f offset);
};

class ObjectivePage : public UIPage
{
public:
	Button addObjective;
	Button showContinents;
	bool isObjective;
	std::vector<ObjectiveEntry> entries;
	ObjectivePage(sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::RenderWindow& window,sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

	void AddObjective();
};

