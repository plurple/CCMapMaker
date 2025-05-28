#pragma once
#include "UIPage.h"

struct Condition
{
	sf::Text* territory;
	Button addCondition;
	sf::Text* condition;
};

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
	std::vector<Condition> territories;
	sf::Text* bombardments;
	Button Killer;
	sf::Text* neutralLabel;
	TextBox neutralBox;
	sf::Text* bonusLabel;
	TextBox bonusBox;
};

enum class TerritoryView
{
	Borders,
	Bombardments,
	Conditions,
	Extras,
	NumViews
};

class TerritoryPage : public UIPage
{
public:
	Button addTerritory;
	Button showContinents;
	Button linkCoordinates;
	std::vector<Button> territoryViews;
	TerritoryView selectedView;
	std::vector<TerritoryEntry> entries;

	TerritoryPage(sf::Font& font, sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize); 
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

};

