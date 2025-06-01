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
		window.setView(scrollBar.scrollWindow);
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window);
		}
		window.draw(page);
		scrollBar.Draw(window);
		window.setView(window.getDefaultView());
	}
}

void ReinforcementPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
{
    if (UI::CheckMouseInBounds(mousePos, addReinforcement.rect))
    {
		AddReinforcement();
    }
	minReinforcements.active = UI::CheckMouseInBounds(mousePos, minReinforcements.box);
	if (mouseOnPage)
	{
		scrollBar.MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)));
	}
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)),
			mouseOnPage);
	}
}

void ReinforcementPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
    UserInput input, bool showCursor)
{
	mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), page);

	if (!input.verticle || !mouseOnPage)
	{
		input.scroll = 0.0f;
	}
	else
	{
		input.scroll *= 7;
	}

	if (input.enter)
	{
		input.scroll += 50;
	}
	if (input.backSpace)
	{
		input.scroll -= 50;
	}
	scrollBar.Scroll({ 0, input.scroll });
	int numEntries = entries.size();
	if (numEntries)
		scrollBar.MoveBar({ 0, 10 + (entries[0].borderBox.getSize().y + 6) * (numEntries) });

	float topBoxY = numEntries ? entries[0].borderBox.getPosition().y : scrollBar.currentScroll.y;
	if (scrollBar.currentScroll.y != topBoxY)
		input.scroll = scrollBar.currentScroll.y - topBoxY;

	minReinforcements.Update(window, timePassed, input, showCursor);
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, input, showCursor);
	}
}

void ReinforcementPage::AddReinforcement()
{
	int numEntries = entries.size();
	float topBoxY = numEntries ? entries[0].borderBox.getPosition().y : 0.0f;
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	ReinforcementEntry pos{topBoxY + (boxSize + 6) * numEntries };
	entries.push_back(pos);
	scrollBar.BarSize({ 0, (entries[0].borderBox.getSize().y + 6) * (numEntries + 1) });
	scrollBar.MoveBar({ 0, 10 + (entries[0].borderBox.getSize().y + 6) * (numEntries + 1) });
	scrollBar.Scroll({ 0, scrollBar.maxScroll.y * -1 });
}

//-----------------------------------------------------------

ReinforcementEntry::ReinforcementEntry(float entryTop) :
	borderBox{ {580,155}/*size*/},
	lowerBox({ 160, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "1"),
	upperBox({ 320, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "300"),
	divisorBox({ 490, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "3")
{
	borderBox.setPosition({ 10,entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Yellow);

	lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 60, entryTop + 8 });
	upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 220, entryTop + 8 });
	divisorLabel = new sf::Text(UI::font, "Divisor:");
	divisorLabel->setPosition({ 380, entryTop + 8 });
	explanation = new sf::Text(UI::font, "1 troop for every divisor regions up to \nmax of (upper-lower)/divisor=max troops \nin the range of lower-upper regions.");
	explanation->setPosition({ 50, entryTop + 48 });
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

void ReinforcementEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	lowerBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, lowerBox.box);
	upperBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, upperBox.box);
	divisorBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, divisorBox.box);
}

void ReinforcementEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	MoveEntry({ 0, input.scroll });

	//TODO make sure that you only care about numbers entered;
	lowerBox.Update(window, timePassed, input, showCursor);
	upperBox.Update(window, timePassed, input, showCursor);
	divisorBox.Update(window, timePassed, input, showCursor);
}

void ReinforcementEntry::MoveEntry(sf::Vector2f offset)
{
	borderBox.move(offset);
	lowerLabel->move(offset);
	lowerBox.MoveBox(offset);
	upperLabel->move(offset);
	upperBox.MoveBox(offset);
	divisorLabel->move(offset);
	divisorBox.MoveBox(offset);
	explanation->move(offset);
}