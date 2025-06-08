#include "Maps.h"
#include <iostream>
#include "UI.h"

float Maps::heightRatio;
float Maps::widthRatio;

MapData::MapData(std::string fileName) : filePath{ fileName }, mapTexture{nullptr}, mapSprite{nullptr}
{
    mapTexture = std::make_shared<sf::Texture>();
    if (!mapTexture->loadFromFile(filePath))
    {
        std::cout << "error with file path:" << filePath << "\n";
    }
    mapSprite = std::make_shared<sf::Sprite>(*mapTexture);
}

Maps::Maps() :
    largeMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsLarge4.17a.png"),
    smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsSmall4.17.png"),
    mapCanvas{ {UI::windowSize.x-600.0f,UI::windowSize.y-0.0f } },
    scrollBar(sf::View{ mapCanvas.getGlobalBounds() }, { mapCanvas.getSize().x - 40, 50 }/*position*/, 
        { 30, mapCanvas.getSize().y - 100 }/*size*/, { 50, mapCanvas.getSize().y - 40 }/*position*/,
        { mapCanvas.getSize().x - 150, 30 }/*size*/, true, true),
    clicked{false}
{
    mapCanvas.setPosition({ 0, 0 });
    mapCanvas.setFillColor(sf::Color::Transparent);

    scrollBar.scrollWindow = sf::View{ mapCanvas.getGlobalBounds() };
    float hRatio = (mapCanvas.getSize().y) / UI::windowSize.y;
    float wRatio = (mapCanvas.getSize().x) / UI::windowSize.x;
    scrollBar.scrollWindow.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { wRatio, hRatio }));
    scrollBar.BarSize(sf::Vector2f{largeMap.mapTexture->getSize()});

    heightRatio = (float)smallMap.mapTexture->getSize().y / (float)largeMap.mapTexture->getSize().y;
    widthRatio = (float)smallMap.mapTexture->getSize().x / (float)largeMap.mapTexture->getSize().x;
}

void Maps::Draw(sf::RenderWindow& window, bool isLarge)
{
    window.setView(scrollBar.scrollWindow);
    window.draw(mapCanvas);
    window.draw(isLarge ? *largeMap.mapSprite : *smallMap.mapSprite);
    scrollBar.Draw(window);
    for (std::shared_ptr<sf::RectangleShape> box : mapBoxes)
    {
        window.draw(*box);
    }
    window.setView(window.getDefaultView());
}

void Maps::Update(sf::RenderWindow& window, sf::Time timePassed,
    UserInput input)
{
    for (int i = mapBoxes.size()-1; i >= 0;)
    {
        if (i < mapBoxes.size() && mapBoxes[i]->getScale().lengthSquared() == 0.0f)
        {
            mapBoxes.erase(mapBoxes.begin() + i);
        }
        else
        {
            i--;
        }
    }
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
    for (std::shared_ptr<sf::RectangleShape> box : mapBoxes)
    {
        box->move(offset);
    }
}

std::shared_ptr<sf::RectangleShape> Maps::AddMapBox(sf::Vector2i position)
{
    std::shared_ptr<sf::RectangleShape> mapBox = 
        std::make_shared<sf::RectangleShape>( sf::Vector2f{20, 20} );
    mapBox->setPosition(sf::Vector2f{ position + sf::Vector2i{ 3, 24 } });
    mapBox->setFillColor(sf::Color::Transparent);
    mapBox->setOutlineThickness(3.0f);
    mapBox->setOutlineColor(sf::Color::Red);
    mapBoxes.push_back(mapBox);
    return mapBox;
}

int Maps::ConvertLarge(int small, bool width)
{
    return small / (width ? widthRatio : heightRatio);
}

int Maps::ConvertSmall(int large, bool width)
{
    return large * (width ? widthRatio : heightRatio);
}