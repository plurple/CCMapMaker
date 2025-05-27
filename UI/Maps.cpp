#include "Maps.h"

MapData::MapData(std::string fileName) : filePath{ fileName }, mapTexture{nullptr}, mapSprite{nullptr}
{
    mapTexture = new sf::Texture();
    if (!mapTexture->loadFromFile(filePath))
    {
    }
    mapSprite = new sf::Sprite(*mapTexture);
}

Maps::Maps(sf::Font &font) :
	isLarge{ true },
    mapCheckBox{{50,50}},
    mapArrow{(15)},
    largeArrowPos{1200, 110},
    smallArrowPos{1250, 110},
    largeMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsLarge4.17a.png"),
    smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsSmall4.17.png")
{
    mapCheckBox.setPosition({ 1200,100 });
    mapCheckBox.setFillColor(sf::Color::Black);
    mapCheckBox.setOutlineThickness(2.0f);
    mapCheckBox.setOutlineColor(sf::Color::White);

    mapArrow.setPointCount(3);
    mapArrow.setPosition(largeArrowPos);
    mapArrow.setFillColor(sf::Color::White);
    mapArrow.setRotation(sf::degrees(90));
    mapArrow.setScale({ 1.0f, -1.0f });

    largeLable = new sf::Text(font, "Large");
    largeLable->setPosition({ 1000, 100 });
    largeLable->setFillColor(sf::Color::Red);
    smallLable = new sf::Text(font, "Small");
    smallLable->setPosition({ 1300, 100 });
}

void Maps::SwapMaps()
{
    if (isLarge)
    {
        largeLable->setFillColor(sf::Color::White);
        smallLable->setFillColor(sf::Color::Red);
        mapArrow.setPosition(smallArrowPos);
    }
    else
    {
        smallLable->setFillColor(sf::Color::White);
        largeLable->setFillColor(sf::Color::Red);
        mapArrow.setPosition(largeArrowPos);
    }
    mapArrow.scale({ 1, -1 });
    isLarge = !isLarge;
}

void Maps::Draw(sf::RenderWindow& window)
{
    window.draw(isLarge ? *largeMap.mapSprite : *smallMap.mapSprite);
    window.draw(mapCheckBox);
    window.draw(mapArrow);
    window.draw(*largeLable);
    window.draw(*smallLable);
}