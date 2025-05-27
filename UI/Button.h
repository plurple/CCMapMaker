#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	sf::RectangleShape rect;
	sf::Text* label = nullptr;

	Button(sf::Font& font, sf::Vector2f pos, std::string lab = "");
	void Draw(sf::RenderWindow& window);
	void Select();
	void Unselect();
};

