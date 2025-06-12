#include "ContinentPanel.h"
#include "UI.h"
#include "../XML/Continent.h"

ContinentPanel::ContinentPanel() :
	panel{ {592, 200} },
    closeButton{ {UI::windowSize.x - 38.0f, 8.0f}, {30, 30}, "X"},
    showPanel{false}
{
    panel.setPosition({ 1004,4 });
    panel.setFillColor({ 192, 192, 192 });
    panel.setOutlineThickness(2.0f);
    panel.setOutlineColor(sf::Color::Blue);
}

void ContinentPanel::Draw(sf::RenderWindow& window)
{
    if (showPanel)
    {
        window.draw(panel);
        closeButton.Draw(window);
        for (auto continent : continents)
        {
            continent->box.Draw(window);
        }
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
        }
        else
        {
            if (continents.size()) continents.erase(continents.begin() + i);
            i--;
        }

    }
}

void ContinentPanel::AddContinent(int xmlKey, std::shared_ptr<Continent> continent, int num)
{
    std::shared_ptr<ContinentBox> box = std::make_shared<ContinentBox>();
    box->xmlKey = xmlKey;
    sf::Vector2f gridPos =  sf::Vector2f{(num % 5) * 105.0f, num / 5 * 25.0f};
    box->box.SetPosition(sf::Vector2f{ 1020.0f, 10.0f } + gridPos);
    box->box.rect.setSize({ 100, 20 });
    box->box.label->setCharacterSize(20);
    box->name = &continent->name;
    continents.push_back(box);
}