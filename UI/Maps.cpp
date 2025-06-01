#include "Maps.h"
#include <iostream>
#include "UI.h"

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
    smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsSmall4.17.png"),
    mapCanvas{ {UI::windowSize.x-600.0f,UI::windowSize.y-0.0f } },
    scrollBar(sf::View{ mapCanvas.getGlobalBounds() }, { mapCanvas.getSize().x - 40, 50 }/*position*/, 
        { 30, mapCanvas.getSize().y - 100 }/*size*/, { 50, mapCanvas.getSize().y - 40 }/*position*/,
        { mapCanvas.getSize().x - 150, 30 }/*size*/, true, true)
{
    mapCanvas.setPosition({ 0, 0 });
    mapCanvas.setFillColor(sf::Color(192, 192, 192, 0));

    scrollBar.scrollWindow = sf::View{ mapCanvas.getGlobalBounds() };
    float bob = (mapCanvas.getSize().y) / UI::windowSize.y;
    float bob2 = (mapCanvas.getSize().x) / UI::windowSize.x;
    scrollBar.scrollWindow.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { bob2, bob }));
    scrollBar.BarSize(sf::Vector2f{largeMap.mapTexture->getSize()});
}

void Maps::Draw(sf::RenderWindow& window, bool isLarge)
{
    window.setView(scrollBar.scrollWindow);
    window.draw(mapCanvas);
    window.draw(isLarge ? *largeMap.mapSprite : *smallMap.mapSprite);
    scrollBar.Draw(window);
    window.setView(window.getDefaultView());
}

void Maps::Update(sf::RenderWindow& window, sf::Time timePassed,
    UserInput input)
{
    bool mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), mapCanvas);

    if (!mouseOnPage)
    {
        input.scroll = 0.0f;
    }
    else
    {
        input.scroll *= 7;
    }
    scrollBar.MoveBar(sf::Vector2f{largeMap.mapTexture->getSize()});
    sf::Vector2f scroll = input.verticle ? 
        sf::Vector2f{ 0, input.scroll } : 
        sf::Vector2f{ input.scroll, 0} ;
    scrollBar.Scroll(scroll);
    scroll.y = scrollBar.currentScroll.y - largeMap.mapSprite->getPosition().y;
    scroll.x = scrollBar.currentScroll.x - largeMap.mapSprite->getPosition().x;
    MoveMap(scroll);
}

void Maps::MoveMap(sf::Vector2f offset)
{
    largeMap.mapSprite->move(offset);
    smallMap.mapSprite->move(offset);
}