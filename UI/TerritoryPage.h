#pragma once
#include "UIPage.h"

enum class TerritoryView
{
	Borders,
	Bombardments,
	Conditions,
	Extras,
	NumViews
};

class TerritoryEntry
{
public:
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;	
	sf::Text* coordinateLabel;
	sf::Text* smallLabel;
	sf::Text* largeLabel;
	sf::Text* xSmallLabel;
	TextBox   xSmallBox;
	sf::Text* ySmallLabel;
	TextBox   ySmallBox;
	sf::Text* xLargeLabel;
	TextBox   xLargeBox;
	sf::Text* yLargeLabel;
	TextBox   yLargeBox;
	sf::Text* connectionLabel;
	std::vector<sf::Text*> territories;
	sf::Text* conditionLabel;
	std::vector<sf::Text*> conditions;
	std::vector<sf::Text*> bombardments;
	Button killer;
	sf::Text* neutralLabel;
	TextBox neutralBox;
	sf::Text* bonusLabel;
	TextBox bonusBox;
	bool selected;

	TerritoryEntry(float entryTop, TerritoryView selectedView);
	void Draw(sf::RenderWindow& window, TerritoryView selectedView);
	void MouseClick(sf::Vector2i mousePos, TerritoryView selectedView, bool mouseOnPage);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, TerritoryView selectedView);
	void MoveEntry(sf::Vector2f offset);
	void SwapView();
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

	TerritoryPage(sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize); 
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor) override;

	void AddTerritory();
};

