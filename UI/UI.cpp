#include "UI.h"
#include "UIPage.h"
#include "ContinentPage.h"
#include "TerritoryPage.h"
#include "ObjectivePage.h"
#include "ReinforcementPage.h"
#include "TransformPage.h"
#include "PositionPage.h"
#include "../EnumOperators.hpp"

sf::Font UI::font;
sf::Vector2u UI::windowSize{ 1600, 900 };
bool UI::isLarge{ true };

UI::UI(XMLData& xmlData) :
    selectedPage{UIPageType::Territory},
    uiPanel{ {600,1000} },
    mapPanel{ {370,45} },
    tabPanel{ {592,142} },
    mapCheckBox({ 1238,13 }, { 30,30 }),
    mapArrow{ (15) },
    largeArrowPos{ 1240, 13 },
    smallArrowPos{ 1265, 13 },
    maps(),
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

    mapSizeLabel = std::make_shared<sf::Text>(font, "Map Size:");
    mapSizeLabel->setPosition({ 1010, 8 });
    largeLabel = std::make_shared < sf::Text>(font, "Large");
    largeLabel->setPosition({ 1150, 8 });
    largeLabel->setFillColor(sf::Color::Red);
    smallLabel = std::make_shared < sf::Text>(font, "Small");
    smallLabel->setPosition({ 1275, 8 });

    std::shared_ptr<TerritoryPage> territoryPage =
        std::make_shared<TerritoryPage>(xmlData,
            sf::Vector2f{ 1010, 60 }, sf::Vector2f{ 150, 30 },
            "Territories", sf::Vector2f{ 592, 142 }, continentPanel.showPanel);
    uiPages.push_back(territoryPage);
    std::shared_ptr<ContinentPage> continentPage = 
        std::make_shared < ContinentPage>(xmlData,
            sf::Vector2f{ 1175, 60 }, sf::Vector2f{ 150, 30 }, 
            "Continents", sf::Vector2f{ 592, 97 }, continentPanel.showPanel);
    uiPages.push_back(continentPage);
    std::shared_ptr < PositionPage> positionPage = 
        std::make_shared < PositionPage>(xmlData,
        sf::Vector2f{ 1010, 100 }, sf::Vector2f{ 130, 30 }, 
            "Positions", sf::Vector2f{ 592, 52 }, continentPanel.showPanel);
    uiPages.push_back(positionPage);
    std::shared_ptr < ObjectivePage> requirementPage =
        std::make_shared < ObjectivePage>(xmlData,
        sf::Vector2f{ 1340, 100 }, sf::Vector2f{ 200, 30 },
            "Requirements", sf::Vector2f{ 592, 52 }, continentPanel.showPanel);
    uiPages.push_back(requirementPage);
    std::shared_ptr < ObjectivePage> objectivePage = 
        std::make_shared < ObjectivePage>(xmlData,
        sf::Vector2f{ 1175, 100 }, sf::Vector2f{ 150, 30 }, 
            "Objectives", sf::Vector2f{ 592, 52 }, continentPanel.showPanel);
    uiPages.push_back(objectivePage);
    std::shared_ptr < ReinforcementPage> reinforcementPage =
        std::make_shared < ReinforcementPage>(xmlData,
            sf::Vector2f{ 1340, 60 }, sf::Vector2f{ 220, 30 },
            "Reinforcements", sf::Vector2f{ 592, 52 }, continentPanel.showPanel);
    uiPages.push_back(reinforcementPage);
    std::shared_ptr < TransformPage> transformPage = 
        std::make_shared < TransformPage>(xmlData,
        sf::Vector2f{ 1400, 20 }, sf::Vector2f{ 160, 30 }, 
            "Transforms", sf::Vector2f{ 592, 52 }, continentPanel.showPanel);
    uiPages.push_back(transformPage);
    uiPages[(int)selectedPage]->tabButton.Select();
}

void UI::Draw(sf::RenderWindow& window)
{
    window.draw(uiPanel);
    window.draw(tabPanel);
    maps.Draw(window, isLarge);
    for (int i = 0; i < (int)UIPageType::COUNT; i++)
    {
        uiPages[i]->Draw(window, (int)selectedPage == i);
    }
    window.draw(mapPanel);
    mapCheckBox.Draw(window);
    window.draw(mapArrow);
    window.draw(*mapSizeLabel);
    window.draw(*largeLabel);
    window.draw(*smallLabel);
    continentPanel.Draw(window);
}

bool UI::CheckMouseInBounds(sf::Vector2i mousePos, sf::RectangleShape rect)
{
    if (rect.getScale() == sf::Vector2f{ 0,0 }) 
        return false;

    float outlineThickness = rect.getOutlineThickness();
    sf::Vector2f rectPos = rect.getPosition() - sf::Vector2f{outlineThickness, outlineThickness};
    sf::Vector2f rectSize = rect.getSize() + sf::Vector2f{ outlineThickness * 2, outlineThickness * 2 };

    return mousePos.x > rectPos.x && mousePos.x < rectPos.x + rectSize.x &&
        mousePos.y > rectPos.y && mousePos.y < rectPos.y + rectSize.y;
}

bool UI::CheckMouseInBounds(sf::Vector2i mousePos, sf::FloatRect rect)
{
    sf::Vector2f rectPos = rect.position;
    sf::Vector2f rectSize = rect.size;

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

void UI::MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos)
{
    if (!continentPanel.showPanel && CheckMouseInBounds(mousePos, *mapCheckBox.rect))
    {
        SwapMaps();
    }
    if (CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(mousePos, continentPanel.scrollBar.scrollWindow)), *continentPanel.closeButton.rect))
    {
        continentPanel.showPanel = false;
    }
    sf::Vector2i mapMouse = sf::Vector2i(window.mapPixelToCoords(mousePos, maps.scrollBar.scrollWindow));
    maps.scrollBar.MouseClick(mapMouse);
    for (int i = 0; i < (int)UIPageType::COUNT; i++)
    {
        if (!continentPanel.showPanel && CheckMouseInBounds(mousePos, *uiPages[i]->tabButton.rect))
        {
            SwapPage((UIPageType)i);
        }
    }
    if (CheckMouseInBounds(mapMouse, maps.largeMap.mapSprite->getGlobalBounds()))
    {
        int index;
        maps.clicked = true;
        uiPages[(int)selectedPage]->MapClick(*this, xmlData, maps, mapMouse, index);
    }
    sf::Vector2i continentMouse = sf::Vector2i(window.mapPixelToCoords(mousePos, continentPanel.scrollBar.scrollWindow));
    continentPanel.scrollBar.MouseClick(continentMouse);
    if (continentPanel.showPanel && CheckMouseInBounds(continentMouse, continentPanel.panel))
    {
        int index;
        maps.clicked = true;
        uiPages[(int)selectedPage]->ContinentClick(*this, xmlData, continentPanel, continentMouse, index);
    }
    uiPages[(int)selectedPage]->MouseClick(xmlData, window, mousePos, maps);  
    maps.clicked = false;
}

void UI::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
    UserInput& input)
{
    if (input.up && input.shift)
    {
        continentPanel.showPanel = !continentPanel.showPanel;
    }
    if (input.down && input.shift)
    {
        SwapMaps();
    }
    if (*input.keyPressed.c_str() == '+' && input.shift)
    {
        xmlData.SaveXML();
    }
    if (*input.keyPressed.c_str() == '*' && input.shift)
    {
        LoadXML(xmlData, maps, continentPanel);
    }
    if (input.ctrl)
    {
        UIPageType newPage = selectedPage;
        input.shift ? newPage-- : newPage++;
        SwapPage(newPage);
    }

    static sf::Time text_effect_time;
    text_effect_time += timePassed;

    if (text_effect_time >= sf::seconds(0.05f))
    {
        showCursor = !showCursor;
        text_effect_time = sf::Time::Zero;
    }

    maps.Update(window, timePassed, input);
    uiPages[(int)selectedPage]->Update(xmlData, window, timePassed, input, showCursor, selectedPage);
    continentPanel.Update(xmlData, window, timePassed, input);
    if (selectedPage == UIPageType::Continent && continentPanel.continents.size())
    {
        int selectedContinent = uiPages[(int)selectedPage]->selectedEntry;
        if(selectedContinent != -1 && selectedContinent < continentPanel.continents.size())
            continentPanel.continents[selectedContinent]->box.rect->setOutlineColor(sf::Color{ 200, 120, 0 });
    }
}

void UI::SwapPage(UIPageType newPage)
{
    uiPages[(int)selectedPage]->tabButton.Toggle();
    uiPages[(int)selectedPage]->UnselectPage();
    selectedPage = newPage;
    uiPages[(int)selectedPage]->tabButton.Toggle();
    uiPages[(int)selectedPage]->SelectPage();
}

void UI::LoadXML(XMLData& xmlData, Maps& maps, ContinentPanel& panel)
{
    xmlData.minReinforcements = 3;
    xmlData.maxPositions = -1;
    for (int i = 0; i < uiPages.size(); i++)
    {
        auto page = uiPages[i];
        for (int j = page->entries.size() - 1; j >= 0; j--)
        {
            page->DeleteEntry(xmlData, (UIPageType)i, j);
        }
        page->selectedEntry = -1;
    }
    for (int i = maps.mapBoxes.size() - 1; i >= 0;i--)
    {
        maps.mapBoxes.erase(maps.mapBoxes.begin() + i);
    }
    xmlData.LoadXML(*this, maps, panel);
}