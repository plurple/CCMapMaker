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
	Transform,
	NumPageTypes
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
	sf::Text* largeLable = nullptr;
	sf::Text* smallLable = nullptr;
	sf::CircleShape mapArrow;
	sf::RectangleShape mapCheckBox;
	bool isLarge;
	sf::Vector2f largeArrowPos;
	sf::Vector2f smallArrowPos;

	UI();
	void Draw(sf::RenderWindow &window);
	bool CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect);
	void SwapMaps();
	void MouseClick(sf::Vector2i mousePos);
};

