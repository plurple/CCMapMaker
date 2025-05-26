#include "UI.h"

UI::UI() : 
    selectedPage{UIPageType::Territory}, 
    uiPanel{{600,1000}},
    maps(font)
{
    sf::Color grey(192, 192, 192);
    uiPanel.setPosition({ 1000,0 });
    uiPanel.setFillColor(grey);

    font.openFromFile("C:/Windows/Fonts/Arial.ttf");
}

void UI::DrawUI(sf::RenderWindow& window)
{
    window.draw(maps.isLarge ? *maps.largeMap.mapSprite : *maps.smallMap.mapSprite);
    window.draw(uiPanel);
    window.draw(maps.mapCheckBox);
    window.draw(maps.mapArrow);
    window.draw(*maps.largeLable);
    window.draw(*maps.smallLable);
}

bool UI::CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect)
{
    sf::Vector2f rectPos = rect.getPosition();
    sf::Vector2f rectSize = rect.getSize();

    return mousePos.x > rectPos.x && mousePos.x < rectPos.x + rectSize.x &&
        mousePos.y > rectPos.y && mousePos.y < rectPos.y + rectSize.y;
}