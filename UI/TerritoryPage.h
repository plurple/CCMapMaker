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

struct BorderEntry
{
	std::shared_ptr<TextBox> nameLabel;
	int index = -1;
	int xmlKey = -1;
	std::shared_ptr<sf::RectangleShape> mapBox;
	bool isContinent = false;
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
	std::vector<std::shared_ptr<BorderEntry>> territories;
	sf::Vector2f conditionsPos;
	std::vector<std::shared_ptr<BorderEntry>> conditions;
	sf::Vector2f bombardmentsPos;
	std::vector<std::shared_ptr<BorderEntry>> bombardments;
	TerritoryView selectedView;
	std::shared_ptr<sf::RectangleShape> mapBox;
	bool* linkedCoords;

	~TerritoryEntry();
	TerritoryEntry(TerritoryView view, int insertedKey, 
		std::shared_ptr<sf::RectangleShape> mapbox) :
		selectedView(view), UIEntry{ insertedKey }, mapBox{ mapbox }, linkedCoords{ nullptr } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void SwapView(TerritoryView view);
	void Select() override;
	void Unselect() override;

	void AddBorder(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
	void AddBombardment(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
	void AddCondition(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
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
		sf::Vector2f buttonBoxSize); 

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	bool MapClick(XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddTerritory(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> mapBox);
	void SwapView();
};

