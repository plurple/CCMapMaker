#pragma once
#include <SFML/Graphics.hpp>

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

	Maps(sf::Font &font);
	void Draw(sf::RenderWindow& window, bool isLarge);
};