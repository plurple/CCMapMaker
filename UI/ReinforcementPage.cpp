#include "ReinforcementPage.h"
#include "UI.h"

ReinforcementPage::ReinforcementPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	addReinforcement({ 1020, 170 }, { 270, 30 }, "Add Reinforcement")
{}

void ReinforcementPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addReinforcement.Draw(window);
	}
}

void ReinforcementPage::MouseClick(sf::Vector2i mousePos)
{
    if (UI::CheckMouseInBounds(mousePos, addReinforcement.rect))
    {
        //TODO add reinforcement entry
    }
}

void ReinforcementPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{

}