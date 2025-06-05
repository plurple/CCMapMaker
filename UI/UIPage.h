#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"
#include "ScrollBar.h"
#include "../XML/XMLData.h"
#include "UI.h"

class UIEntry
{
public:
	std::vector<std::shared_ptr<sf::Shape>> shapes;
	std::vector<std::shared_ptr<sf::Text>> labels;
	std::vector<std::shared_ptr<Button>> buttons;
	std::vector<std::shared_ptr<TextBox>> boxes;
	std::vector<std::shared_ptr<UIEntry>> entries;
	bool selected;
	int xmlKey;

	~UIEntry() {};
	UIEntry(int insertedKey) : selected{ false }, xmlKey{ insertedKey } {};
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
	std::vector<std::shared_ptr<UIEntry>> entries;

	~UIPage() {};
	UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	virtual void Draw(sf::RenderWindow& window, bool selected);
	virtual void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps);
	virtual bool MapClick(XMLData& xmlData, Maps& maps, sf::Vector2i mousePos);
	virtual void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType page);

	void AddEntry(XMLData& xmlData, std::shared_ptr<UIEntry> entry);
};

