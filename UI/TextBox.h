#pragma once
#include <SFML/Graphics.hpp>
#include "../UserInput.h"

class TextBox
{
public:
	sf::RectangleShape box;
	std::shared_ptr<sf::Text> displayText;
	std::shared_ptr<std::string> text;
	std::shared_ptr<int> number;
	bool active;

	TextBox(sf::Vector2f pos, sf::Vector2f boxSize = { 400, 32 }, std::shared_ptr<std::string> defaultText = nullptr, 
		std::shared_ptr<int> defaultNum = nullptr);
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor);
	void Draw(sf::RenderWindow& window);
	void Move(sf::Vector2f offset);
	void Hide(bool show = false);
	bool IsNumber(char key);
	void AddNumber(std::string key);
	void RemoveNumber();
};

