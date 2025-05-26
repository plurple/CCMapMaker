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
	sf::Text* largeLable = nullptr;
	sf::Text* smallLable = nullptr;
	sf::CircleShape mapArrow;
	sf::RectangleShape mapCheckBox;
	bool isLarge;
	sf::Vector2f largeArrowPos;
	sf::Vector2f smallArrowPos;

	Maps(sf::Font &font);
	void SwapMaps();
};