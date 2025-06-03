#pragma once
#include "UIPage.h"

class ObjectiveEntry : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	enum class LabelTypes
	{
		Name,
		Territories,
		TerritoryList,
		Continents,
		ContinentList,
		Required,
		NumLabels
	};
	enum class BoxTypes
	{
		Name,
		NumRequired,
		NumBoxes
	};
public:
	bool isObjective;

	ObjectiveEntry(bool isObjective) : 
		isObjective{ isObjective } {};
	void CreateEntry(XMLData& xmlData, float entryTop, 
		int insertedKey) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset);
};

class ObjectivePage : public UIPage
{
public:
	bool isObjective;

	ObjectivePage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window,sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor) override;

	void AddObjective(XMLData& xmlData);
};

