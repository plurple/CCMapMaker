#include "ContinentPanel.h"
#include "UI.h"
#include "../XML/Continent.h"

ContinentPanel::ContinentPanel() :
	panel{ {592, 200} },
    closeButton{ {panel.getSize().x - 38.0f, 8.0f}, {30, 30}, "X"},
    showPanel{false},
    scrollBar(sf::View{ panel.getGlobalBounds() }, { panel.getSize().x - 38, 80 }/*position*/,
        { 30, panel.getSize().y - 130 }/*size*/)
{
    panel.setPosition({ 0, 0 });
    panel.setFillColor({ 192, 192, 192});
    panel.setOutlineThickness(2.0f);
    panel.setOutlineColor(sf::Color::Blue);

    scrollBar.scrollWindow = sf::View{ panel.getGlobalBounds() };
    float hRatio = (panel.getSize().y) / UI::windowSize.y;
    float wRatio = (panel.getSize().x) / UI::windowSize.x;
    scrollBar.scrollWindow.setViewport(sf::FloatRect({ 1.0f - wRatio, 0.0f }, { wRatio, hRatio }));
    scrollBar.minScroll.y = 10.0f;
}

void ContinentPanel::Draw(sf::RenderWindow& window)
{
    if (showPanel)
    {
        window.setView(scrollBar.scrollWindow);
        window.draw(panel);
        closeButton.Draw(window);
        scrollBar.Draw(window);
        for (auto continent : continents)
        {
            continent->box.Draw(window);
        }
        window.setView(window.getDefaultView());
    }
}

void ContinentPanel::Update(XMLData& xmlData, sf::RenderWindow& window, 
    sf::Time timePassed, UserInput& input)
{
    if (continents.size() < xmlData.continents.size())
    {
        continents.clear();
        for (auto continent : xmlData.continents)
        {
            AddContinent(continent.first, continent.second, continents.size());
        }
    }

    for (int i = 0; i < continents.size(); i++)
    {
        if (xmlData.continents.find(continents[i]->xmlKey) != xmlData.continents.end())
        {
            continents[i]->box.label->setString(*continents[i]->name);
            if(continents[i]->box.rect->getOutlineColor() == sf::Color{ 200, 120, 0 })
                continents[i]->box.rect->setOutlineColor(sf::Color::White);
        }
        else
        {
            if (continents.size()) continents.erase(continents.begin() + i);
            i--;
        }
    }

    if (continents.size())
    {
        bool mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), panel);

        float scroll = input.scroll;

        if (!mouseOnPage)
        {
            scroll = 0.0f;
        }
        else
        {
            scroll *= 7;
        }
        scrollBar.MoveBar(sf::Vector2f{ 0, contentSize });
        scrollBar.Scroll({ 0,scroll });
        scroll = scrollBar.currentScroll.y - continents[0]->box.rect->getPosition().y;
        Move({ 0,scroll });
    }
}

void ContinentPanel::AddContinent(int xmlKey, std::shared_ptr<Continent> continent, int num)
{
    std::shared_ptr<ContinentBox> box = std::make_shared<ContinentBox>();
    box->xmlKey = xmlKey;
    sf::Vector2f gridPos =  sf::Vector2f{(num % 5) * 105.0f, (num / 5) * 25.0f};
    box->box.SetPosition(sf::Vector2f{ 20.0f, 10.0f } + gridPos);
    box->box.rect->setSize({ 100, 20 });
    box->box.label->setCharacterSize(20);
    box->name = &continent->name;
    continents.push_back(box);
    contentSize = (((num+1) / 5) + (int)((bool)((num+1) % 5))) * 25.0f;
}

void ContinentPanel::Move(sf::Vector2f offset)
{
    for (auto continent : continents)
    {
        continent->box.Move(offset);
    }
}