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
		//TODO add a territory
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
}

void TerritoryPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{

}