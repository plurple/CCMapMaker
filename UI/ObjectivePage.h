#pragma once
#include "UIPage.h"

class ObjectiveEntry : public UIEntry
{
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

	~ObjectiveEntry() {};
	ObjectiveEntry(bool isObjective, int insertedKey) :
		isObjective{ isObjective }, UIEntry{ insertedKey }{};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

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

	~ObjectivePage() {};
	ObjectivePage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window,
		sf::Vector2i mousePos, Maps& maps) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddObjective(XMLData& xmlData);
};

