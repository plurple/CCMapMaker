#pragma once
#include <SFML/Graphics.hpp>
class TextBox
{
public:
	sf::RectangleShape box;
	sf::Text* displayText;
	std::string text;
	bool active;

	TextBox(sf::Vector2f pos, sf::Vector2f boxSize = { 400, 32 }, std::string defaultText = "");
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor);
	void Draw(sf::RenderWindow& window);
};

