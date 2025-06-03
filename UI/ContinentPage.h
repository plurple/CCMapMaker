#pragma once
#include "UIPage.h"

enum class ContinentView
{
	Basic,
	Overrides,
	Advanced,
	NumViews
};

class AdvancedTerritory : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	enum class LabelTypes
	{
		TerritoryName,
		FactorLabel,
		NumLabels
	};	
	enum class ButtonTypes
	{
		Mandatory,
		Blocker,
		Multiplier,
		NumButtons
	};
	enum class BoxTypes
	{
		FactorBox, //can be negative and a float the bastard
		NumBoxes
	};
public:
	AdvancedTerritory() {};
	void CreateEntry(XMLData& xmlData, float entryTop, int insertedKey) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class BonusLine : public UIEntry
{
	enum class LabelTypes
	{
		BonusLabel,
		RequiredLabel,
		NumLabels
	};
	enum class BoxTypes
	{
		BonusBox, //can be negative
		RequiredBox,
		NumBoxes
	};
public:
	BonusLine() {};
	void CreateEntry(XMLData& xmlData, float entryTop, int insertedKey) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class ContinentEntry : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	enum class LabelTypes
	{
		NameLabel,
		BonusesLabel,
		TerritoriesLabel,
		ContinentsLabel,
		NumLabels
	};	
	enum class ButtonTypes
	{
		AddBonus,
		RemoveBonus,
		NumButtons
	};
	enum class BoxTypes
	{
		NameBox,
		NumBoxes
	};
	enum class EntryTypes
	{
		Advanced,
		NumButtons
	};
public:
	std::vector<sf::Text*> territories;
	std::vector<sf::Text*> continents;
	std::vector<UIEntry*> bonuses;
	ContinentView selectedView;

	ContinentEntry(ContinentView view) : selectedView{ view } {};
	void CreateEntry(XMLData& xmlData, float entryTop, int insertedKey) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class ContinentPage : public UIPage
{
public:
	std::vector<Button> continentViews;
	ContinentView selectedView;

	ContinentPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor) override;

	void AddContinent(XMLData& xmlData);
	void SwapView();
};

