#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"
#include "ScrollBar.h"
#include "../XML/XMLData.h"
#include "UI.h"

struct LinkedData
{
	std::shared_ptr<TextBox> nameLabel;
	int uiIndex = -1;
	int xmlKey = -1;
	std::shared_ptr<MapBox> mapBox;
	bool isContinent = false;
};

class UIEntry
{
public:
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	std::vector<std::shared_ptr<sf::Text>> labels;
	std::vector<std::shared_ptr<Button>> buttons;
	std::vector<std::shared_ptr<TextBox>> boxes;
	std::vector<std::shared_ptr<UIEntry>> entries;
	bool selected;
	int xmlKey;
	sf::Color baseColor;
	sf::Color selectedColor;

	~UIEntry() {};
	UIEntry(int insertedKey) : selected{ false }, xmlKey{ insertedKey } {};
	virtual void CreateEntry(XMLData& xmlData, float entryTop) = 0;

	virtual void Draw(sf::RenderWindow& window);
	virtual void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
		bool& select, bool mapClicked);
	virtual void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor);

	virtual void MoveEntry(sf::Vector2f offset);
	virtual void Select();
	virtual void Unselect(bool white = false);
	void Toggle(bool toggle);
	virtual void BorderBoxSize() {};
};

class UIPage
{
public:
	Button tabButton;
	Button addEntry;
	Button showContinents;
	sf::RectangleShape page;
	sf::RectangleShape buttonBox;
	ScrollBar scrollBar;
	bool mouseOnPage;
	std::vector<std::shared_ptr<UIEntry>> entries;
	int selectedEntry;
	float contentSize;

	~UIPage() {};
	UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
		std::string tabLabel, sf::Vector2f buttonBoxSize, bool& continentPanel);
	virtual void Draw(sf::RenderWindow& window, bool selected);
	virtual void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps);
	virtual bool MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex);
	virtual bool ContinentClick(UI& ui, XMLData& xmlData, ContinentPanel& panel, sf::Vector2i mousePos, int& continentIndex);
	virtual void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor, UIPageType page);

	void AddEntry(XMLData& xmlData, std::shared_ptr<UIEntry> entry);
	void SwapEntry(int previous, int future);
	void PositionEntries();
	virtual void SelectPage();
	virtual void UnselectPage();
};

