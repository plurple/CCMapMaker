#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "TextBox.h"

class UIPage
{
public:
	Button tabButton;
	sf::RectangleShape page;

	//ScrollBar scrollbar;

	UIPage(sf::Font& font, sf::Vector2f pos, std::string lab);
	void Draw(sf::RenderWindow& window);
};

