#include "ContinentPage.h"
#include "UI.h"

ContinentPage::ContinentPage(sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	selectedView{ ContinentView::Basic },
	addContinent({ 1076, 215 }, { 200, 30 }, "Add Continent"),
	showContinents({ 1286, 215 }, { 240, 30 }, "Show Continents")
{
	Button Basic({ 1076, 170 }, { 85, 30 }, "Basic", true);
	continentViews.push_back(Basic);
	Button overrides({ 1208, 170 }, { 130, 30 }, "Overides");
	continentViews.push_back(overrides);
	Button advanced({ 1381, 170 }, { 145, 30 }, "Advanced");
	continentViews.push_back(advanced);
}

void ContinentPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addContinent.Draw(window);
		if(selectedView != ContinentView::Advanced)
			showContinents.Draw(window);
		for (int i = 0; i < (int)ContinentView::NumViews; i++)
		{
			continentViews[i].Draw(window);
		}
	}
}

void ContinentPage::MouseClick(sf::Vector2i mousePos)
{
	if (selectedView != ContinentView::Advanced)
	{
		if (UI::CheckMouseInBounds(mousePos, showContinents.rect))
		{
			showContinents.Toggle();
			//open continents page
			//TODO unselect show continent on tab change
		}
	}
	else
	{
		showContinents.Unselect();
		//close continents page
	}

	if (UI::CheckMouseInBounds(mousePos, addContinent.rect))
	{
		//add a continent
	}
	for (int i = 0; i < (int)ContinentView::NumViews; i++)
	{
		if (UI::CheckMouseInBounds(mousePos, continentViews[i].rect))
		{
			continentViews[(int)selectedView].Toggle();
			selectedView = (ContinentView)i;
			continentViews[(int)selectedView].Toggle();
		}
	}
}

void ContinentPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{

}