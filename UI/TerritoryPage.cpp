#include "TerritoryPage.h"
#include "UI.h"

TerritoryPage::TerritoryPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize):
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	selectedView{ TerritoryView::Borders },
	addTerritory({ 1066, 260 }, { 190, 30 }, "Add Territory"),
	showContinents({ 1296, 215 }, { 240, 30 }, "Show Continents"),
	linkCoordinates({ 1296, 260 }, { 240, 30 }, "Link Coordinates", true)
{
	Button borders({ 1066, 170 }, { 115, 30 }, "Borders", true);
	territoryViews.push_back(borders);
	Button bombardments({ 1201, 170 }, { 215, 30 }, "Bombardments");
	territoryViews.push_back(bombardments);
	Button conditions({ 1066, 215 }, { 150, 30 }, "Conditions");
	territoryViews.push_back(conditions);
	Button extras({ 1436, 170 }, { 100, 30 }, "Extras");
	territoryViews.push_back(extras);
}

void TerritoryPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addTerritory.Draw(window);
		if(selectedView == TerritoryView::Conditions)
			showContinents.Draw(window);
		linkCoordinates.Draw(window);
		for (int i = 0; i < (int)TerritoryView::NumViews; i++)
		{
			territoryViews[i].Draw(window);
		}
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window, selectedView);
		}
	}
}

void TerritoryPage::MouseClick(sf::Vector2i mousePos)
{
	if (UI::CheckMouseInBounds(mousePos, linkCoordinates.rect))
	{
		linkCoordinates.Toggle();
	}
	if (selectedView == TerritoryView::Conditions)
	{
		if (UI::CheckMouseInBounds(mousePos, showContinents.rect))
		{
			showContinents.Toggle();
			//TODO open continents page
			//TODO unselect show continent on tab change
		}
	}
	else
	{
		showContinents.Unselect();
		//TODO close continents page
	}
		
	if (UI::CheckMouseInBounds(mousePos, addTerritory.rect))
	{
		AddTerritory();
	}
	for (int i = 0; i < (int)TerritoryView::NumViews; i++)
	{
		if (UI::CheckMouseInBounds(mousePos, territoryViews[i].rect))
		{
			territoryViews[(int)selectedView].Toggle();
			selectedView = (TerritoryView)i;
			territoryViews[(int)selectedView].Toggle();
		}
	}
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(mousePos, selectedView);
	}
}

void TerritoryPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor, selectedView);
	}
}


void TerritoryPage::AddTerritory()
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	TerritoryEntry pos{ pageTop + (boxSize + 6) * numEntries, selectedView };
	entries.push_back(pos);
}

//-----------------------------------------------------------

TerritoryEntry::TerritoryEntry(float entryTop, TerritoryView selectedView) :
	borderBox{ {580,200} },
	killer({ 1390, entryTop + 124 }, { 100, 30 }, "Killer"),
	nameBox({ 1120, entryTop + 12 }, { 450, 30 }, "Name"),
	xSmallBox({ 1330, entryTop + 50 }, { 50, 30 }, "0"),
	ySmallBox({ 1440, entryTop + 50 }, { 50, 30 }, "0"),
	xLargeBox({ 1330, entryTop + 88 }, { 50, 30 }, "0"),
	yLargeBox({ 1440, entryTop + 88 }, { 50, 30 }, "0"),
	neutralBox({ 1330, entryTop + 124 }, { 50, 30 }, ""),
	bonusBox({ 1120, entryTop + 124 }, { 50, 30 }, ""),
	selected{ false }
{
	borderBox.setPosition({ 1010,entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor({ 150, 60, 255 });

	nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 1020, entryTop + 8 });
	coordinateLabel = new sf::Text(UI::font, "Coordinates:");
	coordinateLabel->setPosition({ 1020, entryTop + 46 });
	smallLabel = new sf::Text(UI::font, "Small:");
	smallLabel->setPosition({ 1200, entryTop + 46 });
	xSmallLabel = new sf::Text(UI::font, "x:");
	xSmallLabel->setPosition({ 1290, entryTop + 44 });
	ySmallLabel = new sf::Text(UI::font, "y:");
	ySmallLabel->setPosition({ 1400, entryTop + 44 });
	largeLabel = new sf::Text(UI::font, "Large:");
	largeLabel->setPosition({ 1200, entryTop + 84 });
	xLargeLabel = new sf::Text(UI::font, "X:");
	xLargeLabel->setPosition({ 1290, entryTop + 84 });
	yLargeLabel = new sf::Text(UI::font, "Y:");
	yLargeLabel->setPosition({ 1400, entryTop + 84 });
	connectionLabel = new sf::Text(UI::font, "Territories:");
	connectionLabel->setPosition({ 1020, entryTop + 120 });
	conditionLabel = new sf::Text(UI::font, "Condition:");
	conditionLabel->setPosition({ 1020, entryTop + 156 });
	neutralLabel = new sf::Text(UI::font, "Neutral:");
	neutralLabel->setPosition({ 1200, entryTop + 120 });
	bonusLabel = new sf::Text(UI::font, "Bonus:");
	bonusLabel->setPosition({ 1020, entryTop + 120 });

	sf::Text* territory = new sf::Text(UI::font, "Territory");
	territory->setPosition({ 1170, entryTop + 120 });
	territories.push_back(territory);
	sf::Text* condition = new sf::Text(UI::font, "Condition");
	condition->setPosition({ 1170, entryTop + 156 });
	conditions.push_back(condition);
	sf::Text* bombardment = new sf::Text(UI::font, "Bombardment");
	bombardment->setPosition({ 1250, entryTop + 120 });
	bombardments.push_back(bombardment);
}

void TerritoryEntry::Draw(sf::RenderWindow& window, TerritoryView selectedView)
{
	window.draw(borderBox);
	window.draw(*nameLabel);
	nameBox.Draw(window);
	window.draw(*coordinateLabel);
	window.draw(*smallLabel);
	window.draw(*largeLabel);
	window.draw(*xSmallLabel);
	xSmallBox.Draw(window);
	window.draw(*ySmallLabel);
	ySmallBox.Draw(window);
	window.draw(*xLargeLabel);
	xLargeBox.Draw(window);
	window.draw(*yLargeLabel);
	yLargeBox.Draw(window);
	if (selectedView == TerritoryView::Bombardments)
	{
		connectionLabel->setString("Bombardments:");
		window.draw(*connectionLabel);
		for (int i = 0; i < bombardments.size(); i++)
		{
			window.draw(*bombardments[i]);
		}
	}
	else if (selectedView == TerritoryView::Extras)
	{
		killer.Draw(window);
		window.draw(*neutralLabel);
		neutralBox.Draw(window);
		window.draw(*bonusLabel);
		bonusBox.Draw(window);
	}
	else
	{
		connectionLabel->setString("Territories:");
		window.draw(*connectionLabel);
		if (selectedView == TerritoryView::Conditions)
			window.draw(*conditionLabel);
		for (int i = 0; i < territories.size(); i++)
		{
			window.draw(*territories[i]);
			if (selectedView == TerritoryView::Conditions)
			{
				window.draw(*conditions[i]);
			}
		}
	}
}	

void TerritoryEntry::MouseClick(sf::Vector2i mousePos, TerritoryView selectedView)
{
	if (UI::CheckMouseInBounds(mousePos, borderBox))
	{
		selected = true;
		borderBox.setOutlineThickness(4.0f);
		borderBox.setOutlineColor({ 120, 0, 255 });
		//TODO be able to pick a territory from the map
	}
	else
	{
		selected = false;
		borderBox.setOutlineThickness(2.0f);
		borderBox.setOutlineColor({ 150, 60, 255 });
	}
	if (selectedView == TerritoryView::Extras && UI::CheckMouseInBounds(mousePos, killer.rect))
	{
		killer.Toggle();
	}
	nameBox.active = UI::CheckMouseInBounds(mousePos, nameBox.box);
	xSmallBox.active = UI::CheckMouseInBounds(mousePos, xSmallBox.box);
	ySmallBox.active = UI::CheckMouseInBounds(mousePos, ySmallBox.box);
	xLargeBox.active = UI::CheckMouseInBounds(mousePos, xLargeBox.box);
	yLargeBox.active = UI::CheckMouseInBounds(mousePos, yLargeBox.box);
	if (selectedView == TerritoryView::Extras)
	{
		neutralBox.active = UI::CheckMouseInBounds(mousePos, neutralBox.box);
		bonusBox.active = UI::CheckMouseInBounds(mousePos, bonusBox.box);
	}
}

void TerritoryEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter,
	bool showCursor, TerritoryView selectedView)
{
	//TODO make sure that you only care about numbers entered;
	nameBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	xSmallBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	ySmallBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	xLargeBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	yLargeBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	if (selectedView == TerritoryView::Extras)
	{
		neutralBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
		bonusBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}