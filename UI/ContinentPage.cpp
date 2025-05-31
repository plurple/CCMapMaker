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
	scrollBar.MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)));
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)), selectedView);
	}
}

void ContinentPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	std::string keyPressed, bool backspace, bool enter, bool showCursor, 
	bool verticle, float scrolled)
{
	//TODO add scrolled to scrollbar to track placement and such
	mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), page);

	if (!verticle || !mouseOnPage)
	{
		scrolled = 0.0f;
	}
	else
	{
		scrolled *= 7;
	}

	if (enter)
	{
		scrolled += 50;
	}
	if (backspace)
	{
		scrolled -= 50;
	}
	if (scrolled == 0.0f)
	{
		float topBoxY = entries.size() ? entries[0].borderBox.getPosition().y : scrollBar.currentScroll.y;
		if (scrollBar.currentScroll.y != topBoxY)
			scrolled = scrollBar.currentScroll.y - topBoxY;
	}
	else
	{
		scrollBar.currentScroll.y += scrolled;
	}

	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, 
			enter, showCursor, selectedView, scrolled);
	}
}


void ContinentPage::AddContinent()
{
	int numEntries = entries.size();
	float topBoxY = numEntries ? entries[0].borderBox.getPosition().y : 10.0f;
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	ContinentEntry pos{topBoxY + (boxSize + 6) * numEntries };
	entries.push_back(pos);
}

//-----------------------------------------------------------

ContinentEntry::ContinentEntry(float entryTop) :
	borderBox{ {580,202} /*size*/},
	addBonus({ 510, entryTop + 50 }/*position*/, { 30, 30 }/*size*/, "+"),
	removeBonus({ 550, entryTop + 50 }/*position*/, { 30, 30 }/*size*/, "-"),
	nameBox({ 120, entryTop + 12 }/*position*/, { 450, 30 }/*size*/, "Continent"),
	selected{false}
{
	borderBox.setPosition({ 10,entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor({ 255, 170, 0 });

	nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	bonusesLabel = new sf::Text(UI::font, "Bonuses:");
	bonusesLabel->setPosition({ 20, entryTop + 46 });
	territoryLabel = new sf::Text(UI::font, "Territories:");
	territoryLabel->setPosition({ 20, entryTop + 84 });
	continentLabel = new sf::Text(UI::font, "Continents:");
	continentLabel->setPosition({ 20, entryTop + 120 });

	TextBox bonusBox = TextBox({ 250, entryTop + 50 }, { 50, 30 }, "5");
	bonusBoxs.push_back(bonusBox);
	TextBox requiredBox = TextBox({ 450, entryTop + 50 }, { 50, 30 }, "");
	requiredBoxs.push_back(requiredBox);

	sf::Text* continent = new sf::Text(UI::font, "Continent");
	continent->setPosition({ 180, entryTop + 120 });
	continents.push_back(continent);
	sf::Text* territory = new sf::Text(UI::font, "Territory");
	territory->setPosition({ 180, entryTop + 84 });
	territories.push_back(territory);
	sf::Text* bonusLabel = new sf::Text(UI::font, "Bonus:");
	bonusLabel->setPosition({ 150, entryTop + 46 });
	bonusLabels.push_back(bonusLabel);
	sf::Text* requiredLabel = new sf::Text(UI::font, "Required:");
	requiredLabel->setPosition({ 310, entryTop + 46 });
	requiredLabels.push_back(requiredLabel);

	advanced.push_back(AdvancedTerritory(entryTop));
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
	bool showCursor, ContinentView selectedView, float scrolled)
{
	MoveEntry({ 0, scrolled });
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

void ContinentEntry::MoveEntry(sf::Vector2f offset)
{
	borderBox.move(offset);
	nameLabel->move(offset);
	nameBox.MoveBox(offset);
	bonusesLabel->move(offset);
	addBonus.MoveButton(offset);
	removeBonus.MoveButton(offset);
	for (int i = 0; i < bonusLabels.size(); i++)
	{
		bonusLabels[i]->move(offset);
	}
	for (int i = 0; i < bonusBoxs.size(); i++)
	{
		bonusBoxs[i].MoveBox(offset);
	}
	for (int i = 0; i < requiredLabels.size(); i++)
	{
		requiredLabels[i]->move(offset);
	}
	for (int i = 0; i < requiredBoxs.size(); i++)
	{
		requiredBoxs[i].MoveBox(offset);
	}
	territoryLabel->move(offset);
	for (int i = 0; i < territories.size(); i++)
	{
		territories[i]->move(offset);
	}
	for (int i = 0; i < advanced.size(); i++)
	{
		advanced[i].Move(offset);
	}
	continentLabel->move(offset);
	for (int i = 0; i < continents.size(); i++)
	{
		continents[i]->move(offset);
	}
}

//-----------------------------------------------------------

AdvancedTerritory::AdvancedTerritory(float entryTop) :
	borderBox{ {576,78}/*size*/ },
	mandatory({ 350, entryTop + 160 }/*position*/, { 30, 30 }/*size*/, " "),
	blocker({ 130, entryTop + 160 }/*position*/, { 30, 30 }/*size*/, " "),
	multiplier({ 550, entryTop + 160 }/*position*/, { 30, 30 }/*size*/, " "),
	factor({ 530, entryTop + 124 }/*position*/, { 50, 30 }/*size*/, "1.0")
{
	borderBox.setPosition({ 12,entryTop+120 });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Cyan);

	territory = new sf::Text(UI::font, "TerritoryName");
	territory->setPosition({ 20, entryTop + 120 });
	mandatoryLabel = new sf::Text(UI::font, "Mandatory");
	mandatoryLabel->setPosition({ 200, entryTop + 156 });
	blockerLabel = new sf::Text(UI::font, "Blocker");
	blockerLabel->setPosition({ 20, entryTop + 156 });
	multiplierLabel = new sf::Text(UI::font, "Multiplier");
	multiplierLabel->setPosition({ 410, entryTop + 156 });
	factorLabel = new sf::Text(UI::font, "Factor:");
	factorLabel->setPosition({ 410, entryTop + 120 });
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

void AdvancedTerritory::Move(sf::Vector2f offset)
{
	borderBox.move(offset);
	territory->move(offset);
	mandatory.MoveButton(offset);
	mandatoryLabel->move(offset);
	blocker.MoveButton(offset);
	blockerLabel->move(offset);
	multiplier.MoveButton(offset);
	multiplierLabel->move(offset);
	factor.MoveBox(offset);
	factorLabel->move(offset);
}