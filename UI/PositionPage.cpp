#include "PositionPage.h"
#include "UI.h"

PositionPage::PositionPage(sf::Font& font, sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(font, tabPos, tabSize, tabLabel, buttonBoxSize),
	addPosition(font, { 1070, 170 }, { 185, 30 }, "Add Position"),
    maxBox(font, { 1470, 170 }, { 50, 30 })
{
    maxLabel = new sf::Text(font, "Max Positions:");
    maxLabel->setPosition({ 1270, 165 });
}

void PositionPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addPosition.Draw(window);
		maxBox.Draw(window);
		if(maxLabel) window.draw(*maxLabel);
	}
}

void PositionPage::MouseClick(sf::Vector2i mousePos)
{
	maxBox.active = UI::CheckMouseInBounds(mousePos, maxBox.box);
}

void PositionPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	maxBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}
