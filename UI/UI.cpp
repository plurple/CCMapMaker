#include "UI.h"
#include "UIPage.h"
#include "ContinentPage.h"
#include "TerritoryPage.h"
#include "ObjectivePage.h"
#include "ReinforcementPage.h"
#include "TransformPage.h"
#include "PositionPage.h"

UI::UI() : 
    selectedPage{UIPageType::Territory}, 
    uiPanel{ {600,1000} },
    mapPanel{ {370,45} },
    tabPanel{ {592,142} },
    isLarge{ true },
    mapCheckBox(font, { 1238,13 }, { 30,30 }),
    mapArrow{ (15) },
    largeArrowPos{ 1240, 13 },
    smallArrowPos{ 1265, 13 },
    maps(font),
    showCursor{false}
{
    sf::Color grey(192, 192, 192);
    uiPanel.setPosition({ 1000,0 });
    uiPanel.setFillColor(grey);

    mapPanel.setPosition({ 1004,4 });
    mapPanel.setFillColor(grey); 
    mapPanel.setOutlineThickness(4.0f);
    mapPanel.setOutlineColor(sf::Color::Blue);

    tabPanel.setPosition({ 1004,4 });
    tabPanel.setFillColor(grey);
    tabPanel.setOutlineThickness(4.0f);
    tabPanel.setOutlineColor(sf::Color::Blue);

    if (!font.openFromFile("C:/Windows/Fonts/Arial.ttf"))
    {
        //error message
    }

    mapArrow.setPointCount(3);
    mapArrow.setPosition(largeArrowPos);
    mapArrow.setFillColor(sf::Color::White);
    mapArrow.setRotation(sf::degrees(90));
    mapArrow.setScale({ 1.0f, -1.0f });

    mapSizeLabel = new sf::Text(font, "Map Size:");
    mapSizeLabel->setPosition({ 1010, 8 });
    largeLabel = new sf::Text(font, "Large");
    largeLabel->setPosition({ 1150, 8 });
    largeLabel->setFillColor(sf::Color::Red);
    smallLabel = new sf::Text(font, "Small");
    smallLabel->setPosition({ 1275, 8 });

    TerritoryPage* territoryPage = new TerritoryPage(font, { 1010, 60 }, { 150, 30 }, "Territories", { 584, 142 });
    uiPages.push_back(territoryPage);
    ContinentPage* continentPage = new ContinentPage(font, { 1175, 60 }, { 150, 30 }, "Continents", { 584, 97 });
    uiPages.push_back(continentPage);
    ReinforcementPage* reinforcementPage = new ReinforcementPage(font, { 1340, 60 }, { 220, 30 }, "Reinforcements", { 584, 52 });
    uiPages.push_back(reinforcementPage);
    PositionPage* positionPage = new PositionPage(font, { 1010, 100 }, { 130, 30 }, "Positions", { 584, 52 });
    uiPages.push_back(positionPage);
    ObjectivePage* requirementPage = new ObjectivePage(font, { 1340, 100 }, { 200, 30 }, "Requirements", { 584, 52 });
    uiPages.push_back(requirementPage);
    ObjectivePage* objectivePage = new ObjectivePage(font, { 1175, 100 }, { 150, 30 }, "Objectives", { 584, 52 });
    uiPages.push_back(objectivePage);
    TransformPage* transformPage = new TransformPage(font, { 1400, 20 }, { 160, 30 }, "Transforms", { 584, 52 });
    uiPages.push_back(transformPage);
    uiPages[(int)selectedPage]->tabButton.Select();
}

void UI::Draw(sf::RenderWindow& window)
{
    window.draw(uiPanel);
    window.draw(tabPanel);
    maps.Draw(window, isLarge);
    for (int i = 0; i < (int)UIPageType::NumPageTypes; i++)
    {
        uiPages[i]->Draw(window, (int)selectedPage == i);
    }
    window.draw(mapPanel);
    mapCheckBox.Draw(window);
    window.draw(mapArrow);
    window.draw(*mapSizeLabel);
    window.draw(*largeLabel);
    window.draw(*smallLabel);
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
        largeLabel->setFillColor(sf::Color::White);
        smallLabel->setFillColor(sf::Color::Red);
        mapArrow.setPosition(smallArrowPos);
    }
    else
    {
        smallLabel->setFillColor(sf::Color::White);
        largeLabel->setFillColor(sf::Color::Red);
        mapArrow.setPosition(largeArrowPos);
    }
    mapArrow.scale({ 1, -1 });
    isLarge = !isLarge;
}

void UI::MouseClick(sf::Vector2i mousePos)
{
    if (CheckMouseInBounds(mousePos, mapCheckBox.rect))
    {
        SwapMaps();
    }
    for (int i = 0; i < (int)UIPageType::NumPageTypes; i++)
    {
        if (CheckMouseInBounds(mousePos, uiPages[i]->tabButton.rect))
        {
            uiPages[(int)selectedPage]->tabButton.Toggle();
            selectedPage = (UIPageType)i;
            uiPages[(int)selectedPage]->tabButton.Toggle();
        }
    }
    uiPages[(int)selectedPage]->MouseClick(mousePos);
}

void UI::Update(sf::RenderWindow& window, sf::Time timePassed, std::string keyPressed, bool backspace, bool enter)
{
    static sf::Time text_effect_time;
    text_effect_time += timePassed;

    if (text_effect_time >= sf::seconds(0.5f))
    {
        showCursor = !showCursor;
        text_effect_time = sf::Time::Zero;
    }

    uiPages[(int)selectedPage]->Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}