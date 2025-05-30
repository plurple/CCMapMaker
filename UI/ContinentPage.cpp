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

void ContinentPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
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
		AddContinent(window);
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
		entries[i].MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)), selectedView);
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


void ContinentPage::AddContinent(sf::RenderWindow& window)
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	ContinentEntry pos{window, scrollBar.scrollWindow, 60+ (boxSize + 6) * numEntries };
	entries.push_back(pos);
}

//-----------------------------------------------------------

ContinentEntry::ContinentEntry(sf::RenderWindow& window, sf::View& view, float entryTop) :
	borderBox{ {580,202} /*size*/},
	addBonus(sf::Vector2f(window.mapCoordsToPixel({ 1510, entryTop + 50 }, view))/*position*/, { 30, 30 }/*size*/, "+"),
	removeBonus(sf::Vector2f(window.mapCoordsToPixel({ 1550, entryTop + 50 }, view))/*position*/, { 30, 30 }/*size*/, "-"),
	nameBox(sf::Vector2f(window.mapCoordsToPixel({ 1120, entryTop + 12 }, view))/*position*/, { 450, 30 }/*size*/, "Continent"),
	selected{false}
{
	borderBox.setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1010,entryTop }, view)));
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor({ 255, 170, 0 });

	nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 8 }, view)));
	bonusesLabel = new sf::Text(UI::font, "Bonuses:");
	bonusesLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 46 }, view)));
	territoryLabel = new sf::Text(UI::font, "Territories:");
	territoryLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 84 }, view)));
	continentLabel = new sf::Text(UI::font, "Continents:");
	continentLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 120 }, view)));

	TextBox bonusBox = TextBox(sf::Vector2f(window.mapCoordsToPixel({ 1250, entryTop + 50 }, view)), { 50, 30 }, "5");
	bonusBoxs.push_back(bonusBox);
	TextBox requiredBox = TextBox(sf::Vector2f(window.mapCoordsToPixel({ 1450, entryTop + 50 }, view)), { 50, 30 }, "");
	requiredBoxs.push_back(requiredBox);

	sf::Text* continent = new sf::Text(UI::font, "Continent");
	continent->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1180, entryTop + 120 }, view)));
	continents.push_back(continent);
	sf::Text* territory = new sf::Text(UI::font, "Territory");
	territory->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1180, entryTop + 84 }, view)));
	territories.push_back(territory);
	sf::Text* bonusLabel = new sf::Text(UI::font, "Bonus:");
	bonusLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1150, entryTop + 46 }, view)));
	bonusLabels.push_back(bonusLabel);
	sf::Text* requiredLabel = new sf::Text(UI::font, "Required:");
	requiredLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1310, entryTop + 46 }, view)));
	requiredLabels.push_back(requiredLabel);

	advanced.push_back(AdvancedTerritory(window, view, entryTop));
}

void ContinentEntry::Draw(sf::RenderWindow& window, ContinentView selectedView)
{
	window.draw(borderBox);
	window.draw(*nameLabel);
	window.draw(*bonusesLabel);
	nameBox.Draw(window);
	for (int i = 0; i < bonusBoxs.size(); i++)
	{
		window.draw(*bonusLabels[i]);
		bonusBoxs[i].Draw(window);
		window.draw(*requiredLabels[i]);
		requiredBoxs[i].Draw(window);
	}
	addBonus.Draw(window);
	removeBonus.Draw(window);	
	
	if (selectedView == ContinentView::Basic)
	{
		window.draw(*territoryLabel);
		for (int i = 0; i < territories.size(); i++)
		{
			window.draw(*territories[i]);
		}
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
		for (int i = 0; i < continents.size(); i++)
		{
			window.draw(*continents[i]);
		}
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
	for (int i = 0; i < bonusBoxs.size(); i++)
	{
		bonusBoxs[i].active = UI::CheckMouseInBounds(mousePos, bonusBoxs[i].box);
		requiredBoxs[i].active = UI::CheckMouseInBounds(mousePos, requiredBoxs[i].box);
	}
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
	for (int i = 0; i < bonusBoxs.size(); i++)
	{
		bonusBoxs[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
		requiredBoxs[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
	for (int i = 0; i < advanced.size(); i++)
	{
		advanced[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}

//-----------------------------------------------------------

AdvancedTerritory::AdvancedTerritory(sf::RenderWindow& window, sf::View& view, float entryTop) :
	borderBox{ {576,78}/*size*/ },
	mandatory(sf::Vector2f(window.mapCoordsToPixel({ 1350, entryTop + 160 }, view))/*position*/, { 30, 30 }/*size*/, " "),
	blocker(sf::Vector2f(window.mapCoordsToPixel({ 1130, entryTop + 160 }, view))/*position*/, { 30, 30 }/*size*/, " "),
	multiplier(sf::Vector2f(window.mapCoordsToPixel({ 1550, entryTop + 160 }, view))/*position*/, { 30, 30 }/*size*/, " "),
	factor(sf::Vector2f(window.mapCoordsToPixel({ 1530, entryTop + 124 }, view))/*position*/, { 50, 30 }/*size*/, "1.0")
{
	borderBox.setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1012,entryTop+120 }, view)));
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Cyan);

	territory = new sf::Text(UI::font, "TerritoryName");
	territory->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 120 }, view)));
	mandatoryLabel = new sf::Text(UI::font, "Mandatory");
	mandatoryLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1200, entryTop + 156 }, view)));
	blockerLabel = new sf::Text(UI::font, "Blocker");
	blockerLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 156 }, view)));
	multiplierLabel = new sf::Text(UI::font, "Multiplier");
	multiplierLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1410, entryTop + 156 }, view)));
	factorLabel = new sf::Text(UI::font, "Factor:");
	factorLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1410, entryTop + 120 }, view)));
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