#pragma once
#include "UIPage.h"

class PositionPair : public UIEntry
{
public:
	enum class LabelTypes
	{
		TerritoryName,
		StartLabel,
		NumLabels
	};
	enum class BoxTypes
	{
		StartBox,
		NumBoxes
	};
	int pairNum;

	PositionPair(int insertedKey, int pair) :
		UIEntry{ insertedKey }, pairNum{ pair } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class PositionEntry : public UIEntry
{
public:
	enum class ButtonTypes
	{
		AddPosition,
		NumBoxes
	};
	std::vector<std::shared_ptr<UIEntry>> positionPairs;

	~PositionEntry() {};
	PositionEntry(int insertedKey) : UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class PositionPage : public UIPage
{
public:
	std::shared_ptr<sf::Text> maxLabel;
	TextBox maxBox;

	~PositionPage() {};
	PositionPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddPosition(XMLData& xmlData);
};

