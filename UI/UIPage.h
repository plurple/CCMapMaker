#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"
#include "ScrollBar.h"
#include "../XML/XMLData.h"

class UIEntry
{
public:
	std::vector<sf::Shape*> shapes;
	std::vector<sf::Text*> labels;
	std::vector<Button*> buttons;
	std::vector<TextBox*> boxes;
	std::vector<UIEntry*> entries;
	bool selected;

	UIEntry() : selected{ false } {};
	virtual void CreateEntry(XMLData& xmlData, float entryTop) = 0;

	virtual void Draw(sf::RenderWindow& window);
	virtual void MouseClick(sf::Vector2i mousePos, bool mouseOnPage);
	virtual void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor);

	virtual void MoveEntry(sf::Vector2f offset);
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
	std::vector<UIEntry*> entries;

	UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	virtual void Draw(sf::RenderWindow& window, bool selected);
	virtual void MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos);
	virtual void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor);

	void AddEntry(XMLData& xmlData, UIEntry* entry);
};

