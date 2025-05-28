#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"

class UIPage
{
public:
	Button tabButton;
	sf::RectangleShape page;
	sf::RectangleShape buttonBox;

	//ScrollBar scrollbar;

	UIPage(sf::Font& font, sf::Vector2f tabPos, sf::Vector2f tabSize, 
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	virtual void Draw(sf::RenderWindow& window, bool selected);
	virtual void MouseClick(sf::Vector2i mousePos) = 0;
	virtual void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, bool showCursor) = 0;
};

