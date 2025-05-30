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
		window.setView(scrollBar.scrollWindow);
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window, selectedView);
		}
		window.draw(page);
		scrollBar.Draw(window);
		window.setView(window.getDefaultView());
	}
}

void TerritoryPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
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
		AddTerritory(window);
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
		entries[i].MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)), selectedView);
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


void TerritoryPage::AddTerritory(sf::RenderWindow& window)
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	TerritoryEntry pos{window, scrollBar.scrollWindow, 10+(boxSize + 6) * numEntries, selectedView };
	entries.push_back(pos);
}

//-----------------------------------------------------------

TerritoryEntry::TerritoryEntry(sf::RenderWindow& window, sf::View& view, float entryTop, TerritoryView selectedView) :
	borderBox{ {580,200} /*size*/ },
	killer(sf::Vector2f(window.mapCoordsToPixel({ 1390, entryTop + 124 }, view))/*position*/, { 100, 30 }/*size*/, "Killer"),
	nameBox(sf::Vector2f(window.mapCoordsToPixel({ 1120, entryTop + 12 }, view))/*position*/, { 450, 30 }/*size*/, ""),
	xSmallBox(sf::Vector2f(window.mapCoordsToPixel({ 1330, entryTop + 50 }, view))/*position*/, { 50, 30 }/*size*/, "0"),
	ySmallBox(sf::Vector2f(window.mapCoordsToPixel({ 1440, entryTop + 50 }, view))/*position*/, { 50, 30 }/*size*/, "0"),
	xLargeBox(sf::Vector2f(window.mapCoordsToPixel({ 1330, entryTop + 88 }, view))/*position*/, { 50, 30 }/*size*/, "0"),
	yLargeBox(sf::Vector2f(window.mapCoordsToPixel({ 1440, entryTop + 88 }, view))/*position*/, { 50, 30 }/*size*/, "0"),
	neutralBox(sf::Vector2f(window.mapCoordsToPixel({ 1330, entryTop + 124 }, view))/*position*/, { 50, 30 }/*size*/, ""),
	bonusBox(sf::Vector2f(window.mapCoordsToPixel({ 1120, entryTop + 124 }, view))/*position*/, { 50, 30 }/*size*/, ""),
	selected{ false }
{
	borderBox.setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1010,entryTop }, view)));
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor({ 150, 60, 255 });

	nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 8 }, view)));
	coordinateLabel = new sf::Text(UI::font, "Coordinates:");
	coordinateLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 46 }, view)));
	smallLabel = new sf::Text(UI::font, "Small:");
	smallLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1200, entryTop + 46 }, view)));
	xSmallLabel = new sf::Text(UI::font, "x:");
	xSmallLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1290, entryTop + 44 }, view)));
	ySmallLabel = new sf::Text(UI::font, "y:");
	ySmallLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1400, entryTop + 44 }, view)));
	largeLabel = new sf::Text(UI::font, "Large:");
	largeLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1200, entryTop + 84 }, view)));
	xLargeLabel = new sf::Text(UI::font, "X:");
	xLargeLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1290, entryTop + 84 }, view)));
	yLargeLabel = new sf::Text(UI::font, "Y:");
	yLargeLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1400, entryTop + 84 }, view)));
	connectionLabel = new sf::Text(UI::font, "Territories:");
	connectionLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 120 }, view)));
	conditionLabel = new sf::Text(UI::font, "Condition:");
	conditionLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 156 }, view)));
	neutralLabel = new sf::Text(UI::font, "Neutral:");
	neutralLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1200, entryTop + 120 }, view)));
	bonusLabel = new sf::Text(UI::font, "Bonus:");
	bonusLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 120 }, view)));

	sf::Text* territory = new sf::Text(UI::font, "");
	territory->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1170, entryTop + 120 }, view)));
	territories.push_back(territory);
	sf::Text* condition = new sf::Text(UI::font, "");
	condition->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1170, entryTop + 156 }, view)));
	conditions.push_back(condition);
	sf::Text* bombardment = new sf::Text(UI::font, "");
	bombardment->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1250, entryTop + 120 }, view)));
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