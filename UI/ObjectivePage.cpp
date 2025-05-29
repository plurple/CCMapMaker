#include "ObjectivePage.h"
#include "UI.h"

ObjectivePage::ObjectivePage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
    isObjective{ true },
	addObjective({ 1051, 170 }, { 200, 30 }, "Add Objective"),
	showContinents({ 1311, 170 }, { 240, 30 }, "Show Continents")
{
    if (tabLabel == "Requirements")
    {
        isObjective = false;
        addObjective.label->setString("Add Requirement");
        addObjective.rect.setSize({ 245, 30 });
    }
    
}

void ObjectivePage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addObjective.Draw(window);
		showContinents.Draw(window);
	}
}

void ObjectivePage::MouseClick(sf::Vector2i mousePos)
{
    if (UI::CheckMouseInBounds(mousePos, showContinents.rect))
    {
        showContinents.Toggle();
        //TODO show continents stuff
        //TODO unselect show continent on tab change
    }
    if (UI::CheckMouseInBounds(mousePos, addObjective.rect))
    {
        //TODO add objective/reinforcement
    }
}

void ObjectivePage::Update(sf::RenderWindow& window, sf::Time timePassed, 
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{

}