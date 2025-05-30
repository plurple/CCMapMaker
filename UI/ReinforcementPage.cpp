#include "ReinforcementPage.h"
#include "UI.h"

ReinforcementPage::ReinforcementPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	addReinforcement({ 1020, 170 }, { 270, 30 }, "Add Reinforcement"),
	minReinforcements({ 1530, 170 }, { 50, 30 }, "3")
{
	minLabel = new sf::Text(UI::font, "Minium Troops:");
	minLabel->setPosition({ 1310, 165 });
}

void ReinforcementPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addReinforcement.Draw(window);
		window.draw(*minLabel);
		minReinforcements.Draw(window);
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window);
		}
	}
}

void ReinforcementPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
{
    if (UI::CheckMouseInBounds(mousePos, addReinforcement.rect))
    {
		AddReinforcement();
    }
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(mousePos);
	}
	minReinforcements.active = UI::CheckMouseInBounds(mousePos, minReinforcements.box);
}

void ReinforcementPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	minReinforcements.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}

void ReinforcementPage::AddReinforcement()
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	ReinforcementEntry pos{ pageTop + (boxSize + 6) * numEntries };
	entries.push_back(pos);
}

//-----------------------------------------------------------

ReinforcementEntry::ReinforcementEntry(float entryTop) :
	borderBox{ {580,155} },
	lowerBox({ 1160, entryTop + 12 }, { 50, 30 }, "1"),
	upperBox({ 1320, entryTop + 12 }, { 50, 30 }, "300"),
	divisorBox({ 1490, entryTop + 12 }, { 50, 30 }, "3")
{
	borderBox.setPosition({ 1010,entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Yellow);

	lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 1060, entryTop + 8 });
	upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 1220, entryTop + 8 });
	divisorLabel = new sf::Text(UI::font, "Divisor:");
	divisorLabel->setPosition({ 1380, entryTop + 8 });
	explanation = new sf::Text(UI::font, "1 troop for every divisor regions up to \nmax of (upper-lower)/divisor=max troops \nin the range of lower-upper regions.");
	explanation->setPosition({ 1050, entryTop + 48 });
}

void ReinforcementEntry::Draw(sf::RenderWindow& window)
{
	window.draw(borderBox);
	lowerBox.Draw(window);
	upperBox.Draw(window);
	divisorBox.Draw(window);
	window.draw(*lowerLabel);
	window.draw(*upperLabel);
	window.draw(*divisorLabel);
	window.draw(*explanation);
}

void ReinforcementEntry::MouseClick(sf::Vector2i mousePos)
{
	lowerBox.active = UI::CheckMouseInBounds(mousePos, lowerBox.box);
	upperBox.active = UI::CheckMouseInBounds(mousePos, upperBox.box);
	divisorBox.active = UI::CheckMouseInBounds(mousePos, divisorBox.box);
}

void ReinforcementEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	lowerBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	upperBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	divisorBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}