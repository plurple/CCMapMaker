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
    largeMap("C:/Users/Matthew/Documents/Conquer club/map xml/Fibonacci Snail/FibonacciSnailLarge.png"),
    smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/Fibonacci Snail/FibonacciSnailSmall.png"),
    //largeMap("C:/Users/Matthew/Documents/Conquer club/map xml/Conquistadores/ConquistadoresLarge15.png"),
    //smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/Conquistadores/ConquistadoresSmall15.png"),
    //largeMap("C:/Users/Matthew/Documents/Conquer club/map xml/Feudal Legend/FeudalLegendLarge2.png"),
    //smallMap("C:/Users/Matthew/Documents/Conquer club/map xml/Feudal Legend/FeudalLegendSmall2.png"),
    mapCanvas{ {UI::windowSize.x-600.0f,UI::windowSize.y-0.0f } },
    scrollBar(sf::View{ mapCanvas.getGlobalBounds() }, { mapCanvas.getSize().x - 40, 50 }/*position*/, 
        { 30, mapCanvas.getSize().y - 100 }/*size*/, { 50, mapCanvas.getSize().y - 40 }/*position*/,
        { mapCanvas.getSize().x - 150, 30 }/*size*/, true, true),
    clicked{false},
    boxTexture{ {UI::windowSize.x - 600,UI::windowSize.y - 0 } }
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
    sf::Sprite boxSprite(boxTexture.getTexture());
    window.draw(boxSprite);
    window.setView(window.getDefaultView());
}

void Maps::Update(sf::RenderWindow& window, sf::Time timePassed,
    UserInput& input)
{
    for (int i = mapBoxes.size() - 1; i >= 0;)
    {
        auto borderPos = mapBoxes[i]->border->getPosition();
        if (UI::isLarge ? borderPos != mapBoxes[i]->largePos : borderPos != mapBoxes[i]->smallPos)
        {
            mapBoxes[i]->border->setPosition(UI::isLarge ? mapBoxes[i]->largePos : mapBoxes[i]->smallPos);
            UpdateBoxes();
        }
        if (i < mapBoxes.size() && mapBoxes[i]->border->getScale().lengthSquared() == 0.0f)
        {
            mapBoxes.erase(mapBoxes.begin() + i);
            UpdateBoxes();
        }     
        i--;
    }
    bool mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), mapCanvas);

    float scroll = input.scroll;
    if (!mouseOnPage)
    {
        scroll = 0.0f;
    }
    else
    {
        scroll *= 7;
    }
    scrollBar.MoveBar(sf::Vector2f{largeMap.mapTexture->getSize()});
    sf::Vector2f scrollVec = input.verticle ? 
        sf::Vector2f{ 0, scroll } : 
        sf::Vector2f{ scroll, 0} ;
    scrollBar.Scroll(scrollVec);
    scrollVec.y = scrollBar.currentScroll.y - largeMap.mapSprite->getPosition().y;
    scrollVec.x = scrollBar.currentScroll.x - largeMap.mapSprite->getPosition().x;
    MoveMap(scrollVec);
}

void Maps::MoveMap(sf::Vector2f offset)
{
    largeMap.mapSprite->move(offset);
    smallMap.mapSprite->move(offset);
    for (std::shared_ptr<MapBox> box : mapBoxes)
    {
        box->border->move(offset);
    }
}

std::shared_ptr<MapBox> Maps::AddMapBox(sf::Vector2i position)
{
    std::shared_ptr<MapBox> mapBox = std::make_shared<MapBox>();
    mapBox->border = std::make_shared<sf::RectangleShape>();
    mapBox->border->setSize( sf::Vector2f{20, 20} );
    mapBox->border->setPosition(sf::Vector2f{ position + sf::Vector2i{ 3, 24 } });
    mapBox->border->setFillColor(sf::Color::Transparent);
    mapBox->border->setOutlineThickness(3.0f);
    mapBox->border->setOutlineColor(sf::Color::Red);
    mapBoxes.push_back(mapBox);
    UpdateBoxes();
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

void Maps::UpdateBoxes()
{
    boxTexture.clear(sf::Color::Transparent);
    for (std::shared_ptr<MapBox> box : mapBoxes)
    {
        boxTexture.draw(*box->border);
    }
    boxTexture.display();
}