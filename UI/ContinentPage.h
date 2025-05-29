#pragma once
#include "UIPage.h"


class AdvancedTerritory
{
	sf::Text* territory;
	Button madatory;
	Button blocker;
	Button multiplier;
	TextBox factor;
};

class ContinentEntry
{
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;
	sf::Text* bonusLabel;
	TextBox bonusBox;
	Button requiredButton;
	sf::Text* requiredLabel;
	TextBox requiredBox;
	sf::Text* territoryLabel;
	sf::Text* territories;
	std::vector<AdvancedTerritory> advanced;
	sf::Text* continentLabel;
	sf::Text* overideLabel;
	sf::Text* continents;
};

enum class ContinentView
{
	Basic,
	Overrides,
	Advanced,
	NumViews
};

class ContinentPage : public UIPage
{
public:
	Button addContinent;
	Button showContinents;
	std::vector<Button> continentViews;
	ContinentView selectedView;
	std::vector<ContinentEntry> entries;

	ContinentPage(sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

};

