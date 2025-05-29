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
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window);
		}
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
		AddObjective();
    }
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(mousePos, isObjective);
	}
}

void ObjectivePage::Update(sf::RenderWindow& window, sf::Time timePassed, 
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}

void ObjectivePage::AddObjective()
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	ObjectiveEntry pos{ pageTop + (boxSize + 6) * numEntries, isObjective };
	entries.push_back(pos);
}

//-----------------------------------------------------------

ObjectiveEntry::ObjectiveEntry(float entryTop, bool isObjective) :
	borderBox{ {580,165} },
	nameBox({ 1120, entryTop + 12 }, { 450, 30 }, "Obective Name"),
	numRequiredBox({ 1235, entryTop + 124 }, { 50, 30 }, isObjective ? "all":"1"),
	selected{false}
{
	borderBox.setPosition({ 1010,entryTop });
	borderBox.setFillColor({ 192, 192, 192 });
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta);

	nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 1020, entryTop + 8 });
	territoryLabel = new sf::Text(UI::font, "Territories:");
	territoryLabel->setPosition({ 1020, entryTop + 46 });
	territories = new sf::Text(UI::font, "Territory");
	territories->setPosition({ 1180, entryTop + 46 });
	continentLabel = new sf::Text(UI::font, "Continents:");
	continentLabel->setPosition({ 1020, entryTop + 84 });
	continents = new sf::Text(UI::font, "Continent");
	continents->setPosition({ 1180, entryTop + 84 });
	requiredLabel = new sf::Text(UI::font, "Num Required:");
	requiredLabel->setPosition({ 1020, entryTop + 120 });
}

void ObjectiveEntry::Draw(sf::RenderWindow& window)
{
	window.draw(borderBox);
	nameBox.Draw(window);
	numRequiredBox.Draw(window);
	window.draw(*nameLabel);
	window.draw(*territoryLabel);
	window.draw(*territories);
	window.draw(*continentLabel);
	window.draw(*continents);
	window.draw(*requiredLabel);
}

void ObjectiveEntry::MouseClick(sf::Vector2i mousePos, bool isObjective)
{
	if (UI::CheckMouseInBounds(mousePos, borderBox))
	{
		selected = true;
		borderBox.setOutlineThickness(4.0f);
		borderBox.setOutlineColor(isObjective? sf::Color{ 250, 170, 180 } : sf::Color{ 175, 0, 175 });
		//TODO be able to pick a territory from the map
	}
	else
	{
		selected = false;
		borderBox.setOutlineThickness(2.0f);
		borderBox.setOutlineColor(isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta);
	}
	nameBox.active = UI::CheckMouseInBounds(mousePos, nameBox.box);
	numRequiredBox.active = UI::CheckMouseInBounds(mousePos, numRequiredBox.box);
}

void ObjectiveEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	nameBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	numRequiredBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}