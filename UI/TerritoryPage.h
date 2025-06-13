#pragma once
#include "UIPage.h"

enum class TerritoryView
{
	Borders,
	Bombardments,
	Conditions,
	Extras,
	COUNT
};

class TerritoryEntry : public UIEntry
{
public:
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
	sf::Vector2f territoriesPos;
	std::vector<std::shared_ptr<LinkedData>> territories;
	sf::Vector2f conditionsPos;
	std::vector<std::shared_ptr<LinkedData>> conditions;
	sf::Vector2f bombardmentsPos;
	std::vector<std::shared_ptr<LinkedData>> bombardments;
	TerritoryView selectedView;
	std::shared_ptr<MapBox> mapBox;
	bool* linkedCoords;

	~TerritoryEntry();
	TerritoryEntry(TerritoryView view, int insertedKey, 
		std::shared_ptr<MapBox> mapbox) :
		selectedView(view), UIEntry{ insertedKey }, mapBox{ mapbox }, linkedCoords{ nullptr } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void SwapView(TerritoryView view);
	void Select() override;
	void Unselect(bool white = false) override;

	void AddBorder(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
	void AddBombardment(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
	void AddCondition(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> border, 
		int boxIndex, int otherXMLKey, bool isContinent);
	void RemoveCondition(XMLData& xmlData, int borderIndex);
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
		sf::Vector2f buttonBoxSize, bool& continentPanel);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	bool MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex) override;
	bool ContinentClick(UI& ui, XMLData& xmlData, ContinentPanel& panel, sf::Vector2i mousePos, int& continentIndex) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddTerritory(XMLData& xmlData, std::shared_ptr<MapBox> mapBox);
	void SwapView();
};

