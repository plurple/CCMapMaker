#pragma once
#include <SFML/Graphics.hpp>
#include "ScrollBar.h"
#include "../UserInput.h"

struct MapData
{
	std::string filePath;
	std::shared_ptr<sf::Texture> mapTexture = nullptr;
	std::shared_ptr<sf::Sprite> mapSprite = nullptr;

	~MapData() {};
	MapData(std::string fileName);
};

struct MapBox
{
	std::shared_ptr<sf::RectangleShape> border;
	sf::Vector2f smallPos;
	sf::Vector2f largePos;
};

class Maps
{
public:
	MapData largeMap;
	MapData smallMap;
	sf::RectangleShape mapCanvas;
	ScrollBar scrollBar;
	std::vector<std::shared_ptr<MapBox>> mapBoxes;
	static float widthRatio;
	static float heightRatio;
	bool clicked;

	~Maps() {};
	Maps();
	void Draw(sf::RenderWindow& window, bool isLarge);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input);

	void MoveMap(sf::Vector2f offset);
	std::shared_ptr<MapBox> AddMapBox(sf::Vector2i position);
	static int ConvertLarge(int small, bool width);
	static int ConvertSmall(int large, bool width);
};