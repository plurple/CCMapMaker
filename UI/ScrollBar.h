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
	sf::RectangleShape verticleBar;
	sf::RectangleShape verticleTrack;
	bool horizontal;
	sf::RectangleShape horizontalBar;
	sf::RectangleShape horizontalTrack;
	sf::View scrollWindow;
	sf::Vector2f currentScroll;
	sf::Vector2f maxScroll;
	sf::Vector2f minScroll;

	ScrollBar(sf::View view, sf::Vector2f verticlePos, 
		sf::Vector2f verticleSize, sf::Vector2f horizontalPos = { 0,0 },
		sf::Vector2f horizontalSize = { 0, 0 }, bool vert = true, bool horiz = false);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos);

	void BarSize(sf::Vector2f contentSize);
	void MoveBar(sf::Vector2f contentSize);
	void Scroll(sf::Vector2f offset);
};

