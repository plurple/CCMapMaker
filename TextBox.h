#pragma once
#include <SFML/Graphics.hpp>
class TextBox
{
public:
	sf::Vector2f position;
	sf::RectangleShape box;
	sf::Text* displayText;
	std::string text;
	bool active;

	TextBox(sf::Vector2f pos, sf::Font& font);
};

