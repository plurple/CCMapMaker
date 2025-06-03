#pragma once
#include "UIPage.h"

class PositionEntry : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
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
public:
	PositionEntry(int insertedKey) : UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class PositionPage : public UIPage
{
public:
	sf::Text* maxLabel;
	TextBox maxBox;

	PositionPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddPosition(XMLData& xmlData);
};

