#pragma once
#include "UIPage.h"

class ObjectiveEntry : public UIEntry
{
public:
	enum class LabelTypes
	{
		Name,
		Required,
		Territories,
		Continents,
		NumLabels
	};
	enum class BoxTypes
	{
		Name,
		NumRequired,
		NumBoxes
	};
	bool isObjective;
	sf::Vector2f territoryPos;
	std::vector<std::shared_ptr<LinkedData>> territories;
	sf::Vector2f continentPos;
	std::vector<std::shared_ptr<LinkedData>> continents;

	~ObjectiveEntry() {};
	ObjectiveEntry(bool isObjective, int insertedKey) :
		isObjective{ isObjective }, UIEntry{ insertedKey }{};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage,
		bool& select, bool mapClicked) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset);
	void Select() override;
	void Unselect(bool white = false) override;
	void AddTerritory(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey);
	void AddContinent(XMLData& xmlData, ContinentPanel& panel, int continentIndex, int otherXMLKey);
	void BorderBoxSize() override;
};

class ObjectivePage : public UIPage
{
public:
	bool isObjective;

	~ObjectivePage() {};
	ObjectivePage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize, bool& continentPanel);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window,
		sf::Vector2i mousePos, Maps& maps) override;
	bool MapClick(UI& ui, XMLData& xmlData, Maps& maps,
		sf::Vector2i mousePos, int& boxIndex) override;
	bool ContinentClick(UI& ui, XMLData& xmlData, ContinentPanel& panel, 
		sf::Vector2i mousePos, int& continentIndex);
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor, UIPageType pageType) override;

	void AddObjective(XMLData& xmlData);
};

