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

class TerritoryEntry : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	enum class LabelTypes
	{
		NameLabel,
		CoordinatesLabel,
		SmallLabel,
		SmallXLabel,
		SmallYLabel,
		LargeLabel,
		LargeXLabel,
		LargeYLabel,
		ConnectionLabel,
		ConditionLabel,
		NeutralLabel,
		BonusLabel, //can be negative
		NumLabels
	};
	enum class BoxTypes
	{
		NameBox,
		SmallXBox,
		SmallYBox,
		LargeXBox,
		LargeYBox,
		NeutralBox,
		BonusBox,
		NumBoxes
	};
	enum class ButtonTypes
	{
		Killer,
		NumButtons
	};
public:
	std::vector<sf::Text*> territories;
	std::vector<sf::Text*> conditions;
	std::vector<sf::Text*> bombardments;
	TerritoryView selectedView;

	TerritoryEntry(TerritoryView view) : selectedView(view) {};
	void CreateEntry(XMLData& xmlData, float entryTop, int insertedKey) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void SwapView(TerritoryView view);
};


class TerritoryPage : public UIPage
{
public:
	Button linkCoordinates;
	std::vector<Button> territoryViews;
	TerritoryView selectedView;

	TerritoryPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize); 

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor) override;

	void AddTerritory(XMLData& xmlData);
	void SwapView();
};

