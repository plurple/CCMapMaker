#include "UI.h"
#include "UIPage.h"

UI::UI() : 
    selectedPage{UIPageType::Territory}, 
    uiPanel{{600,1000}},
    isLarge{ true },
    mapCheckBox{ {50,50} },
    mapArrow{ (15) },
    largeArrowPos{ 1200, 110 },
    smallArrowPos{ 1250, 110 },
    maps(font)
{
    sf::Color grey(192, 192, 192);
    uiPanel.setPosition({ 1000,0 });
    uiPanel.setFillColor(grey);

    font.openFromFile("C:/Windows/Fonts/Arial.ttf");

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

    for (int i = 0; i < (int)UIPageType::NumPageTypes; i++)
    {
        UIPage* page = new UIPage(font, { 1000 + (float)i * 50, 50 }, std::to_string(i));
        uiPages.push_back(page);
    }
    uiPages[(int)selectedPage]->SelectTab();
}

void UI::Draw(sf::RenderWindow& window)
{
    window.draw(uiPanel);
    maps.Draw(window, isLarge);
    for (int i = 0; i < (int)UIPageType::NumPageTypes; i++)
    {
        uiPages[i]->Draw(window);
    }
    window.draw(mapCheckBox);
    window.draw(mapArrow);
    window.draw(*largeLable);
    window.draw(*smallLable);
}

bool UI::CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect)
{
    sf::Vector2f rectPos = rect.getPosition();
    sf::Vector2f rectSize = rect.getSize();

    return mousePos.x > rectPos.x && mousePos.x < rectPos.x + rectSize.x &&
        mousePos.y > rectPos.y && mousePos.y < rectPos.y + rectSize.y;
}

void UI::SwapMaps()
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

void UI::MouseClick(sf::Vector2i mousePos)
{
    if (CheckMouseInBounds(mousePos, mapCheckBox))
    {
        SwapMaps();
    }
    for (int i = 0; i < (int)UIPageType::NumPageTypes; i++)
    {
        if (CheckMouseInBounds(mousePos, uiPages[i]->tab))
        {
            uiPages[(int)selectedPage]->UnselectTab();
            selectedPage = (UIPageType)i;
            uiPages[(int)selectedPage]->SelectTab();
        }
    }
}