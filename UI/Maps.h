#pragma once
#include <SFML/Graphics.hpp>
#include "ScrollBar.h"
#include "../UserInput.h"

struct MapData
{
	std::string filePath;
	sf::Texture* mapTexture = nullptr;
	sf::Sprite* mapSprite = nullptr;

	MapData(std::string fileName);
};

class Maps
{
public:
	MapData largeMap;
	MapData smallMap;
	sf::RectangleShape mapCanvas;
	ScrollBar scrollBar;

	Maps();
	void Draw(sf::RenderWindow& window, bool isLarge);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input);

	void MoveMap(sf::Vector2f offset);
};