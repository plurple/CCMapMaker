#pragma once
#include <SFML/Graphics.hpp>

class UIPage
{
public:
	sf::RectangleShape tab;
	sf::Text* label = nullptr;

	UIPage(sf::Font& font, sf::Vector2f pos, std::string lab);
	void Draw(sf::RenderWindow& window);
	void SelectTab();
	void UnselectTab();
};

