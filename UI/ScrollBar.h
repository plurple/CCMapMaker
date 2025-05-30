#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"

class ScrollBar
{
public:
	Button upButton;
	sf::CircleShape upArrow;
	Button downButton;
	sf::CircleShape downArrow;
	Button leftButton;
	sf::CircleShape leftArrow;
	Button rightButton;
	sf::CircleShape rightArrow;
	bool verticle;
	sf::RectangleShape bar;
	sf::RectangleShape track;
	sf::View scrollWindow;

	ScrollBar(sf::View view, sf::Vector2f position, sf::Vector2f size, bool vert = true);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		std::string keyPressed, bool backspace, bool enter, bool showCursor);

};

