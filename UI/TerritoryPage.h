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
		BonusLabel,
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
		BonusBox, //can be negative
		NumBoxes
	};
	enum class ButtonTypes
	{
		Killer,
		NumButtons
	};
public:
	std::vector<std::shared_ptr<sf::Text>> territories;
	std::vector<std::shared_ptr<sf::Text>> conditions;
	std::vector<std::shared_ptr<sf::Text>> bombardments;
	TerritoryView selectedView;
	std::shared_ptr<sf::RectangleShape> mapBox;

	~TerritoryEntry();
	TerritoryEntry(TerritoryView view, int insertedKey, 
		std::shared_ptr<sf::RectangleShape> mapbox) :
		selectedView(view), UIEntry{ insertedKey }, mapBox{ mapbox } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void SwapView(TerritoryView view);
	void Select() override;
	void Unselect() override;
};


class TerritoryPage : public UIPage
{
public:
	Button linkCoordinates;
	std::vector<std::shared_ptr<Button>> territoryViews;
	TerritoryView selectedView;

	~TerritoryPage() {};
	TerritoryPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize); 

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	bool MapClick(XMLData& xmlData, Maps& maps, sf::Vector2i mousePos);
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddTerritory(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> mapBox);
	void SwapView();
};

