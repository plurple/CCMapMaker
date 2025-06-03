#pragma once
#include <SFML/Graphics.hpp>
#include "Maps.h"
#include "Button.h"
#include "../UserInput.h"
#include "../XML/XMLData.h"

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
	static sf::Font font;
	static sf::Vector2u windowSize;
	sf::Text* mapSizeLabel;
	sf::Text* largeLabel;
	sf::Text* smallLabel;
	sf::CircleShape mapArrow;
	Button mapCheckBox;
	bool isLarge;
	sf::Vector2f largeArrowPos;
	sf::Vector2f smallArrowPos;
	bool showCursor;

	UI(XMLData& xmlData);
	void Draw(sf::RenderWindow &window);
	static bool CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect);
	static bool CheckMouseInBounds(sf::Vector2i mousePos, sf::FloatRect rect);
	void SwapMaps();
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos);
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput& input);
};

