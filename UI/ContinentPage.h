#pragma once
#include "UIPage.h"

enum class ContinentView
{
	Basic,
	Overrides,
	Advanced,
	COUNT
};

class AdvancedTerritory : public UIEntry
{
public:
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
	~AdvancedTerritory() {};
	AdvancedTerritory(int insertedKey) : UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class BonusLine : public UIEntry
{
public:
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
	int bonusNum;

	~BonusLine() {};
	BonusLine(int insertedKey, int bonus) : 
		UIEntry{ insertedKey }, bonusNum{ bonus } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class ContinentEntry : public UIEntry
{
public:
	enum class LabelTypes
	{
		NameLabel,
		BonusesLabel,
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
	std::vector<std::shared_ptr<sf::Text>> continents;
	std::vector<std::shared_ptr<UIEntry>> bonuses;
	ContinentView selectedView;

	~ContinentEntry() {};
	ContinentEntry(ContinentView view, int insertedKey) : 
		selectedView{ view }, UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class ContinentPage : public UIPage
{
public:
	std::vector<std::shared_ptr<Button>> continentViews;
	ContinentView selectedView;

	ContinentPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddContinent(XMLData& xmlData);
	void SwapView();
};

