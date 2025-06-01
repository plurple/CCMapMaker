#pragma once
#include "UIPage.h"

enum class ContinentView
{
	Basic,
	Overrides,
	Advanced,
	NumViews
};

class AdvancedTerritory
{
public:
	sf::RectangleShape borderBox;
	sf::Text* territory;
	Button mandatory;
	sf::Text* mandatoryLabel;
	Button blocker;
	sf::Text* blockerLabel;
	Button multiplier;
	sf::Text* multiplierLabel;
	TextBox factor;
	sf::Text* factorLabel;

	AdvancedTerritory(float entryTop);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor);

	void Move(sf::Vector2f offset);
	void SwapView();
};

class ContinentEntry
{
public:
	sf::RectangleShape borderBox;
	sf::Text* nameLabel;
	TextBox nameBox;
	sf::Text* bonusesLabel;
	Button addBonus;
	Button removeBonus;
	std::vector<sf::Text*> bonusLabels;
	std::vector<TextBox> bonusBoxs;
	std::vector<sf::Text*> requiredLabels;
	std::vector<TextBox> requiredBoxs;
	sf::Text* territoryLabel;
	std::vector<sf::Text*> territories;
	std::vector<AdvancedTerritory> advanced;
	sf::Text* continentLabel;
	std::vector<sf::Text*> continents;
	bool selected;

	ContinentEntry(float entryTop);
	void Draw(sf::RenderWindow& window, ContinentView selectedView);
	void MouseClick(sf::Vector2i mousePos, ContinentView selectedView, bool mouseOnPage);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor);

	void MoveEntry(sf::Vector2f offset);
	void swapView();
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
	void MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor) override;

	void AddContinent();
};

