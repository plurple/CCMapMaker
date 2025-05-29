#include "PositionPage.h"
#include "UI.h"

PositionPage::PositionPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	addPosition({ 1070, 170 }, { 185, 30 }, "Add Position"),
    maxBox({ 1470, 170 }, { 50, 30 })
{
    maxLabel = new sf::Text(UI::font, "Max Positions:");
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
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window, selected);
		}
	}
}

void PositionPage::MouseClick(sf::Vector2i mousePos)
{
	if (UI::CheckMouseInBounds(mousePos, addPosition.rect))
	{
		AddPosition();
	}
	maxBox.active = UI::CheckMouseInBounds(mousePos, maxBox.box);
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(mousePos);
	}
}

void PositionPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	maxBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}

void PositionPage::AddPosition()
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	PositionEntry pos{pageTop + (boxSize + 6) * numEntries};
	entries.push_back(pos);
}

//-----------------------------------------------------------

PositionEntry::PositionEntry(float entryTop) :
	borderBox{{580,50}},
	startBox({ 1525, entryTop+12 }, { 50, 30 }, "3"),
	selected{false}
{
	borderBox.setPosition({ 1010,entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Green);

	territoryName = new sf::Text(UI::font, "Territory Name");
	territoryName->setPosition({ 1030, entryTop+8 });
	startLabel = new sf::Text(UI::font, "Start Size:");
	startLabel->setPosition({ 1380, entryTop+8 });
}

void PositionEntry::Draw(sf::RenderWindow& window, bool selected)
{
	window.draw(borderBox);
	startBox.Draw(window);
	window.draw(*startLabel);
	window.draw(*territoryName);
}

void PositionEntry::MouseClick(sf::Vector2i mousePos)
{
	if (UI::CheckMouseInBounds(mousePos, borderBox))
	{
		selected = true;
		borderBox.setOutlineThickness(4.0f);
		borderBox.setOutlineColor({ 26, 176, 26 });
		//TODO be able to pick a territory from the map
	}
	else
	{
		selected = false;
		borderBox.setOutlineThickness(2.0f);
		borderBox.setOutlineColor(sf::Color::Green);
	}
	startBox.active = UI::CheckMouseInBounds(mousePos, startBox.box);
}

void PositionEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	startBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}