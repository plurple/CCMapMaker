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
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window, selectedView);
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
		AddContinent();
	}
	for (int i = 0; i < (int)ContinentView::NumViews; i++)
	{
		if (UI::CheckMouseInBounds(mousePos, continentViews[i].rect))
		{
			continentViews[(int)selectedView].Toggle();
			selectedView = (ContinentView)i;
			continentViews[(int)selectedView].Toggle();

			//todo overides view swap continet list.
		}
	}
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(mousePos, selectedView);
	}
}

void ContinentPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor, selectedView);
	}
}


void ContinentPage::AddContinent()
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	ContinentEntry pos{ pageTop + (boxSize + 6) * numEntries };
	entries.push_back(pos);
}

//-----------------------------------------------------------

ContinentEntry::ContinentEntry(float entryTop) :
	borderBox{ {580,202} },
	addBonus({ 1410, entryTop + 50 }, { 30, 30 }, "+"),
	removeBonus({ 1450, entryTop + 50 }, { 30, 30 }, "-"),
	nameBox({ 1120, entryTop + 12 }, { 450, 30 }, "Continent"),
	bonusBox({ 1150, entryTop + 50 }, { 50, 30 }, "5"),
	requiredBox({ 1350, entryTop + 50 }, { 50, 30 }, "all"),
	selected{false}
{
	borderBox.setPosition({ 1010,entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor({ 255, 170, 0 });

	nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 1020, entryTop + 8 });
	bonusLabel = new sf::Text(UI::font, "Bonuses:");
	bonusLabel->setPosition({ 1020, entryTop + 46 });
	requiredLabel = new sf::Text(UI::font, "Required:");
	requiredLabel->setPosition({ 1210, entryTop + 46 });
	territoryLabel = new sf::Text(UI::font, "Territories:");
	territoryLabel->setPosition({ 1020, entryTop + 84 });
	territories = new sf::Text(UI::font, "Territory");
	territories->setPosition({ 1180, entryTop + 84 });
	continentLabel = new sf::Text(UI::font, "Continents:");
	continentLabel->setPosition({ 1020, entryTop + 120 });
	continents = new sf::Text(UI::font, "Continent");
	continents->setPosition({ 1180, entryTop + 120 }); 

	advanced.push_back(AdvancedTerritory(entryTop));
}

void ContinentEntry::Draw(sf::RenderWindow& window, ContinentView selectedView)
{
	window.draw(borderBox);
	window.draw(*nameLabel);
	nameBox.Draw(window);
	window.draw(*bonusLabel);
	bonusBox.Draw(window);
	window.draw(*requiredLabel);
	requiredBox.Draw(window);
	addBonus.Draw(window);
	removeBonus.Draw(window);	
	
	if (selectedView == ContinentView::Basic)
	{
		window.draw(*territoryLabel);
		window.draw(*territories);
	}
	if(selectedView == ContinentView::Advanced)
	{
		window.draw(*territoryLabel);
		for (int i = 0; i < advanced.size(); i++)
		{
			advanced[i].Draw(window);
		}
	}
	else
	{
		window.draw(*continentLabel);
		window.draw(*continents);
	}
}

void ContinentEntry::MouseClick(sf::Vector2i mousePos, ContinentView selectedView)
{
	if (UI::CheckMouseInBounds(mousePos, borderBox))
	{
		selected = true;
		borderBox.setOutlineThickness(4.0f);
		borderBox.setOutlineColor({ 230, 150, 0 });
		//TODO be able to pick a territory from the map
	}
	else
	{
		selected = false;
		borderBox.setOutlineThickness(2.0f);
		borderBox.setOutlineColor({ 255, 170, 0 });
	}
	if (UI::CheckMouseInBounds(mousePos, addBonus.rect))
	{
		//todo add a bonus line.
	}
	if (UI::CheckMouseInBounds(mousePos, removeBonus.rect))
	{
		//todo remove a bonus line.
	}
	nameBox.active = UI::CheckMouseInBounds(mousePos, nameBox.box);
	bonusBox.active = UI::CheckMouseInBounds(mousePos, bonusBox.box);
	requiredBox.active = UI::CheckMouseInBounds(mousePos, requiredBox.box);
	for (int i = 0; i < advanced.size(); i++)
	{
		advanced[i].MouseClick(mousePos);
	}
}

void ContinentEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter, 
	bool showCursor, ContinentView selectedView)
{
	//TODO make sure that you only care about numbers entered;
	nameBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	bonusBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	requiredBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	for (int i = 0; i < advanced.size(); i++)
	{
		advanced[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}

//-----------------------------------------------------------

AdvancedTerritory::AdvancedTerritory(float entryTop) :
	borderBox{ {576,78} },
	mandatory({ 1350, entryTop + 160 }, { 30, 30 }, " "),
	blocker({ 1130, entryTop + 160 }, { 30, 30 }, " "),
	multiplier({ 1550, entryTop + 160 }, { 30, 30 }, " "),
	factor({ 1530, entryTop + 124 }, { 50, 30 }, "1.0")
{
	borderBox.setPosition({ 1012,entryTop+120 });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Cyan);

	territory = new sf::Text(UI::font, "TerritoryName");
	territory->setPosition({ 1020, entryTop + 120 });
	mandatoryLabel = new sf::Text(UI::font, "Mandatory");
	mandatoryLabel->setPosition({ 1200, entryTop + 156 });
	blockerLabel = new sf::Text(UI::font, "Blocker");
	blockerLabel->setPosition({ 1020, entryTop + 156 });
	multiplierLabel = new sf::Text(UI::font, "Multiplier");
	multiplierLabel->setPosition({ 1410, entryTop + 156 });
	factorLabel = new sf::Text(UI::font, "Factor:");
	factorLabel->setPosition({ 1410, entryTop + 120 });
}

void AdvancedTerritory::Draw(sf::RenderWindow& window)
{
	window.draw(borderBox);
	window.draw(*territory);
	mandatory.Draw(window);
	window.draw(*mandatoryLabel);
	blocker.Draw(window);
	window.draw(*blockerLabel);
	multiplier.Draw(window);
	window.draw(*multiplierLabel);
	if (multiplier.selected)
	{
		factor.Draw(window);
		window.draw(*factorLabel);
	}
}

void AdvancedTerritory::MouseClick(sf::Vector2i mousePos)
{
	if (UI::CheckMouseInBounds(mousePos, mandatory.rect))
	{
		mandatory.Toggle();
		blocker.Unselect();
		multiplier.Unselect();
	}
	else if (UI::CheckMouseInBounds(mousePos, blocker.rect))
	{
		blocker.Toggle();
		mandatory.Unselect();
		multiplier.Unselect();
	}
	else if (UI::CheckMouseInBounds(mousePos, multiplier.rect))
	{
		multiplier.Toggle();
		mandatory.Unselect();
		blocker.Unselect();
	}
	if(multiplier.selected)
		factor.active = UI::CheckMouseInBounds(mousePos, factor.box);
}

void AdvancedTerritory::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	if(multiplier.selected)
		factor.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}