#pragma once
#include <SFML/Graphics.hpp>
#include "Maps.h"
#include "Button.h"

enum class UIPageType
{
	Territory,
	Continent,
	Position,
	Requirements,
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
	sf::RectangleShape mapPanel;
	sf::RectangleShape tabPanel;
	std::vector<UIPage*> uiPages;
	UIPageType selectedPage;
	Maps maps;
	sf::Font font;
	sf::Text* mapSizeLabel;
	sf::Text* largeLabel;
	sf::Text* smallLabel;
	sf::CircleShape mapArrow;
	Button mapCheckBox;
	bool isLarge;
	sf::Vector2f largeArrowPos;
	sf::Vector2f smallArrowPos;
	bool showCursor;

	UI();
	void Draw(sf::RenderWindow &window);
	static bool CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect);
	void SwapMaps();
	void MouseClick(sf::Vector2i mousePos);
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter);
};

