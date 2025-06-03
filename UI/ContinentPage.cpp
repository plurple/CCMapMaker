#include "ContinentPage.h"
#include "UI.h"

ContinentPage::ContinentPage(sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	selectedView{ ContinentView::Basic }
{
	Button Basic({ 1076, 170 }, { 85, 30 }, "Basic", true);
	continentViews.push_back(Basic);
	Button overrides({ 1208, 170 }, { 130, 30 }, "Overides");
	continentViews.push_back(overrides);
	Button advanced({ 1381, 170 }, { 145, 30 }, "Advanced");
	continentViews.push_back(advanced);

	addEntry.SetPosition({ 1076, 215 });
	addEntry.rect.setSize({ 200, 30 });
	addEntry.label->setString("Add Continent");

	showContinents.SetPosition({ 1286, 215 });
}

void ContinentPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		if(selectedView != ContinentView::Advanced)
			showContinents.Draw(window);
		for (int i = 0; i < (int)ContinentView::NumViews; i++)
		{
			continentViews[i].Draw(window);
		}
	}
}

void ContinentPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(window, mousePos);
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

	if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
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
			SwapView();
		}
	}	
}

void ContinentPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	UserInput input, bool showCursor)
{
	UIPage::Update(window, timePassed, input, showCursor);
}

void ContinentPage::AddContinent()
{
	ContinentEntry* entry = new ContinentEntry{selectedView};
	UIPage::AddEntry(entry);
}

void ContinentPage::SwapView()
{
	/*TODO change the colour of the labels to make them visible or not
	also move the positions and size of the box and such*/
	//need to hide factor on the advanced view when not multiplied.
	for (UIEntry* entry : entries)
	{
		dynamic_cast<ContinentEntry*>(entry)->selectedView = selectedView;
	}
}
//-----------------------------------------------------------

void ContinentEntry::CreateEntry(float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ {580,202 } };/*size*/
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor({ 255, 170, 0 });
	shapes.push_back(border);

	sf::Text* nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);
	
	sf::Text* bonusesLabel = new sf::Text(UI::font, "Bonuses:");
	bonusesLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(bonusesLabel);
	
	sf::Text* territoryLabel = new sf::Text(UI::font, "Territories:");
	territoryLabel->setPosition({ 20, entryTop + 84 });
	labels.push_back(territoryLabel);
	
	sf::Text* continentLabel = new sf::Text(UI::font, "Continents:");
	continentLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(continentLabel);

	Button* addBonus = new Button({ 510, entryTop + 50 }/*position*/, { 30, 30 }/*size*/, "+");
	buttons.push_back(addBonus);

	Button* removeBonus = new Button({ 550, entryTop + 50 }/*position*/, { 30, 30 }/*size*/, "-");
	buttons.push_back(removeBonus);

	TextBox* nameBox = new TextBox({ 120, entryTop + 12 }/*position*/, { 450, 30 }/*size*/, "Continent");
	boxes.push_back(nameBox);

	AdvancedTerritory* advance = new AdvancedTerritory();
	advance->CreateEntry(entryTop);
	entries.push_back(advance);

	sf::Text* territory = new sf::Text(UI::font, "Territory");
	territory->setPosition({ 180, entryTop + 84 });
	territories.push_back(territory);

	sf::Text* continent = new sf::Text(UI::font, "Continent");
	continent->setPosition({ 180, entryTop + 120 });
	continents.push_back(continent);

	BonusLine* bonus = new BonusLine();
	bonus->CreateEntry(entryTop);
	bonuses.push_back(bonus);
}

void ContinentEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	
	for (UIEntry* bonus : bonuses)
	{
		bonus->Draw(window);
	}

	if (selectedView == ContinentView::Basic)
	{
		for (sf::Text* territory : territories)
		{
			window.draw(*territory);
		}
	}
	if (selectedView != ContinentView::Advanced)
	{
		for (sf::Text* continent : continents)
		{
			window.draw(*continent);
		}
	}
}

void ContinentEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	sf::Shape* borderBox = shapes[(int)ShapeTypes::Border];
	if (mouseOnPage && borderBox)
	{
		if (UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()))
		{
			selected = true;
			borderBox->setOutlineThickness(4.0f);
			borderBox->setOutlineColor({ 230, 150, 0 });
			//TODO be able to pick a territory from the map
		}
		else
		{
			selected = false;
			borderBox->setOutlineThickness(2.0f);
			borderBox->setOutlineColor({ 255, 170, 0 });
		}
	}

	Button* addBonus = buttons[(int)ButtonTypes::AddBonus];
	if (mouseOnPage && addBonus && UI::CheckMouseInBounds(mousePos, addBonus->rect))
	{
		//todo add a bonus line.
	}
	Button* removeBonus = buttons[(int)ButtonTypes::RemoveBonus];
	if (mouseOnPage && removeBonus && UI::CheckMouseInBounds(mousePos, removeBonus->rect))
	{
		//todo remove a bonus line.
	}
	for (UIEntry* bonus : bonuses)
	{
		bonus->MouseClick(mousePos, mouseOnPage);
	}
}

void ContinentEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
	//TODO make sure that you only care about numbers entered;
	for (UIEntry* bonus : bonuses)
	{
		bonus->Update(window, timePassed, input, showCursor);
	}
}

void ContinentEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);	
	for (sf::Text* territory : territories)
	{
		territory->move(offset);
	}
	for (sf::Text* continent : continents)
	{
		continent->move(offset);
	}
	for (UIEntry* bonus : bonuses)
	{
		bonus->MoveEntry(offset);
	}
}

//-----------------------------------------------------------

void BonusLine::CreateEntry(float entryTop) 
{
	sf::Text* bonusLabel = new sf::Text(UI::font, "Bonus:");
	bonusLabel->setPosition({ 150, entryTop + 46 });
	labels.push_back(bonusLabel);

	sf::Text* requiredLabel = new sf::Text(UI::font, "Required:");
	requiredLabel->setPosition({ 310, entryTop + 46 });
	labels.push_back(requiredLabel);

	TextBox* bonusBox = new TextBox({ 250, entryTop + 50 }, { 50, 30 }, "5");
	boxes.push_back(bonusBox);

	TextBox* requiredBox = new TextBox({ 450, entryTop + 50 }, { 50, 30 }, "");
	boxes.push_back(requiredBox);	
}

void BonusLine::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void BonusLine::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
}

void BonusLine::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
}

void BonusLine::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}

//-----------------------------------------------------------

void AdvancedTerritory::CreateEntry(float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ {576,78 } };/*size*/
	border->setPosition({ 12,entryTop + 120 });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Cyan);
	shapes.push_back(border);

	sf::Text* territory = new sf::Text(UI::font, "TerritoryName");
	territory->setPosition({ 20, entryTop + 120 });
	labels.push_back(territory);

	sf::Text* factorLabel = new sf::Text(UI::font, "Factor:");
	factorLabel->setPosition({ 410, entryTop + 120 });
	labels.push_back(factorLabel);

	Button* mandatory  = new Button({ 350, entryTop + 160 }/*position*/, { 30, 30 }/*size*/, "Mandatory");
	buttons.push_back(mandatory);

	Button* blocker    = new Button({ 130, entryTop + 160 }/*position*/, { 30, 30 }/*size*/, "Blocker");
	buttons.push_back(blocker);

	Button* multiplier = new Button({ 550, entryTop + 160 }/*position*/, { 30, 30 }/*size*/, "Multiplier");
	buttons.push_back(multiplier);

	TextBox* factor = new TextBox({ 530, entryTop + 124 }/*position*/, { 50, 30 }/*size*/, "1.0");
	boxes.push_back(factor);
}

void AdvancedTerritory::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void AdvancedTerritory::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	Button* mandatory = buttons[(int)ButtonTypes::Mandatory];
	Button* multiplier = buttons[(int)ButtonTypes::Multiplier];
	Button* blocker = buttons[(int)ButtonTypes::Blocker];
			
	if (mouseOnPage && mandatory && multiplier && blocker)
	{
		if (UI::CheckMouseInBounds(mousePos, mandatory->rect))
		{
			mandatory->Toggle();
			blocker->Unselect();
			multiplier->Unselect();
		}
		else if (UI::CheckMouseInBounds(mousePos, blocker->rect))
		{
			blocker->Toggle();
			mandatory->Unselect();
			multiplier->Unselect();
		}
		else if (UI::CheckMouseInBounds(mousePos, multiplier->rect))
		{
			multiplier->Toggle();
			mandatory->Unselect();
			blocker->Unselect();
		}
	}
	TextBox* factor = boxes[(int)BoxTypes::FactorBox];
	if (factor)
	{
		factor->active = multiplier && multiplier->selected &&
			mouseOnPage && UI::CheckMouseInBounds(mousePos, factor->box);
	}
}

void AdvancedTerritory::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
}

void AdvancedTerritory::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}