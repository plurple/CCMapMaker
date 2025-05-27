#pragma once
#include <SFML/Graphics.hpp>
#include "Maps.h"

enum class UIPageType
{
	Territory,
	Continent,
	Position,
	Objective,
	Reinforcement,
	Transform
};

class UIPage;

class UI
{
public:
	sf::RectangleShape uiPanel;
	std::vector<UIPage*> uiPages;
	UIPageType selectedPage;
	Maps maps;
	sf::Font font;
	UI();
	bool CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect);
	void Draw(sf::RenderWindow &window);
};

