#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	sf::RectangleShape rect;
	sf::Text* label;
	bool selected;
	bool* xmlLink;

	Button(sf::Vector2f pos, sf::Vector2f dimensions = { 50,50 }, 
		std::string lab = "", bool select = false);
	void Draw(sf::RenderWindow& window);
	void Select();
	void Unselect();
	void Toggle();
	void Move(sf::Vector2f offset);
	void SetPosition(sf::Vector2f);
	void Hide(bool show = false);
};

