#pragma once
#include "UIPage.h"

class PositionPair : public UIEntry
{
public:
	enum class LabelTypes
	{
		StartLabel,
		NumLabels
	};
	enum class BoxTypes
	{
		TerritoryName,
		StartBox,
		NumBoxes
	};
	int uiIndex;
	int otherXMLKey;
	std::shared_ptr<MapBox> mapBox;

	~PositionPair();
	PositionPair(int insertedKey) :
		UIEntry{ insertedKey }{};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
		bool& select, bool mapClicked) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class PositionEntry : public UIEntry
{
public:
	std::vector<std::shared_ptr<UIEntry>> positionPairs;
	float pairsPos;

	~PositionEntry() {};
	PositionEntry(int insertedKey) : UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
		bool& select, bool mapClicked) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void AddPositionPair(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
	void Select(int& selectedTextbox) override;
	void Unselect(int& selectedtextbox, bool white = false) override;
	void BorderBoxSize() override;
};

class PositionPage : public UIPage
{
public:
	std::shared_ptr<sf::Text> maxLabel;
	TextBox maxBox;

	~PositionPage() {};
	PositionPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize, bool& continentPanel);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	bool MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, 
		int& boxIndex) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor, UIPageType pageType) override;

	void AddPosition(XMLData& xmlData);
	void SelectPage() override;
	void UnselectPage() override;
};

