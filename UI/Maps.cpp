#include "Maps.h"
#include <iostream>

MapData::MapData(std::string fileName) : filePath{ fileName }, mapTexture{nullptr}, mapSprite{nullptr}
{
    mapTexture = new sf::Texture();
    if (!mapTexture->loadFromFile(filePath))
    {
        std::cout << "error with file path:" << filePath << "\n";
    }
    mapSprite = new sf::Sprite(*mapTexture);
}

Maps::Maps() :
    largeMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsLarge4.17a.png"),
    smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsSmall4.17.png")
{}

void Maps::Draw(sf::RenderWindow& window, bool isLarge)
{
    window.draw(isLarge ? *largeMap.mapSprite : *smallMap.mapSprite);    
}