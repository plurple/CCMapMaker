#pragma once
#include <SFML/Graphics.hpp>
#include "../UserInput.h"

class TextBox
{
public:
	sf::RectangleShape box;
	sf::Text* displayText;
	std::string text;
	bool active;

	TextBox(sf::Vector2f pos, sf::Vector2f boxSize = { 400, 32 }, std::string defaultText = "");
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor);
	void Draw(sf::RenderWindow& window);
	void MoveBox(sf::Vector2f offset);
};

