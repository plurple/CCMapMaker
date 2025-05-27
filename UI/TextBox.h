#pragma once
#include <SFML/Graphics.hpp>
class TextBox
{
public:
	sf::RectangleShape box;
	sf::Text* displayText;
	std::string text;
	bool active;

	TextBox(sf::Vector2f pos, sf::Font& font);
	void Update(sf::RenderWindow& window, sf::Time timePassed);
	void Draw(sf::RenderWindow& window);
};

