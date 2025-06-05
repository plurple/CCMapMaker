#include "ContinentPage.h"
#include "UI.h"
#include "../XML/Continent.h"

ContinentPage::ContinentPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	selectedView{ ContinentView::Basic }
{
	std::shared_ptr<Button> Basic = 
		std::make_shared<Button>(sf::Vector2f{ 1076, 170 }, 
		sf::Vector2f{ 85, 30 }, "Basic", true);
	continentViews.push_back(Basic);
	std::shared_ptr<Button> overrides = 
		std::make_shared<Button>(sf::Vector2f{ 1208, 170 }, 
			sf::Vector2f{ 130, 30 }, "Overides");
	continentViews.push_back(overrides);
	std::shared_ptr<Button> advanced =
		std::make_shared<Button>(sf::Vector2f{ 1381, 170 }, 
			sf::Vector2f{ 145, 30 }, "Advanced");
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
			continentViews[i]->Draw(window);
		}
	}
}

void ContinentPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
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
		AddContinent(xmlData);
	}
	for (int i = 0; i < (int)ContinentView::NumViews; i++)
	{
		if (UI::CheckMouseInBounds(mousePos, continentViews[i]->rect))
		{
			continentViews[(int)selectedView]->Toggle();
			selectedView = (ContinentView)i;
			continentViews[(int)selectedView]->Toggle();

			//todo overides view swap continet list.
			SwapView();
		}
	}	
}

void ContinentPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
}

void ContinentPage::AddContinent(XMLData& xmlData)
{
	std::shared_ptr<ContinentEntry> entry = 
		std::make_shared<ContinentEntry>(selectedView, xmlData.AddContinent());
	UIPage::AddEntry(xmlData, entry);
}

void ContinentPage::SwapView()
{
	/*TODO change the colour of the labels to make them visible or not
	also move the positions and size of the box and such*/
	//need to hide factor on the advanced view when not multiplied.
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		std::dynamic_pointer_cast<ContinentEntry>(entry)->selectedView = selectedView;
	}
} 
//-----------------------------------------------------------

void ContinentEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{580,202 } );/*size*/
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor({ 255, 170, 0 });
	shapes.push_back(border);

	std::shared_ptr<sf::Text> nameLabel = 
		std::make_shared<sf::Text>(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);
	
	std::shared_ptr<sf::Text> bonusesLabel = 
		std::make_shared<sf::Text>(UI::font, "Bonuses:");
	bonusesLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(bonusesLabel);
	
	std::shared_ptr<sf::Text> territoryLabel = 
		std::make_shared<sf::Text>(UI::font, "Territories:");
	territoryLabel->setPosition({ 20, entryTop + 84 });
	labels.push_back(territoryLabel);
	
	std::shared_ptr<sf::Text> continentLabel = 
		std::make_shared<sf::Text>(UI::font, "Continents:");
	continentLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(continentLabel);

	std::shared_ptr<Button> addBonus = 
		std::make_shared<Button>(sf::Vector2f{ 510, entryTop + 50 }/*position*/, 
			sf::Vector2f{ 30, 30 }/*size*/, "+");
	buttons.push_back(addBonus);

	std::shared_ptr<Button> removeBonus = 
		std::make_shared<Button>(sf::Vector2f{ 550, entryTop + 50 }/*position*/, 
			sf::Vector2f{ 30, 30 }/*size*/, "-");
	buttons.push_back(removeBonus);

	std::shared_ptr<Continent> data = xmlData.continents.at(xmlKey);
	std::shared_ptr<TextBox> nameBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 120, entryTop + 12 }/*position*/,
			sf::Vector2f{ 450, 30 }/*size*/);
	nameBox->text = &data->name;
	boxes.push_back(nameBox);

	std::shared_ptr<AdvancedTerritory> advance = 
		std::make_shared<AdvancedTerritory>(xmlKey);
	advance->CreateEntry(xmlData, entryTop);
	entries.push_back(advance);

	std::shared_ptr<sf::Text> continent = 
		std::make_shared<sf::Text>(UI::font, "Continent");
	continent->setPosition({ 180, entryTop + 120 });
	continents.push_back(continent);

	std::shared_ptr<BonusLine> bonus = 
		std::make_shared<BonusLine>(xmlKey, bonuses.size());
	bonus->CreateEntry(xmlData, entryTop);
	bonuses.push_back(bonus);
}

void ContinentEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->Draw(window);
	}

	if (selectedView == ContinentView::Basic)
	{
		//todo draw the territory name of advanced Data
	}
	if (selectedView != ContinentView::Advanced)
	{
		for (std::shared_ptr<sf::Text> continent : continents)
		{
			window.draw(*continent);
		}
	}
}

void ContinentEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	std::shared_ptr<sf::Shape> borderBox = shapes[(int)ShapeTypes::Border];
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

	std::shared_ptr<Button> addBonus = buttons[(int)ButtonTypes::AddBonus];
	if (mouseOnPage && addBonus && UI::CheckMouseInBounds(mousePos, addBonus->rect))
	{
		//todo add a bonus line.
	}
	std::shared_ptr<Button> removeBonus = buttons[(int)ButtonTypes::RemoveBonus];
	if (mouseOnPage && removeBonus && UI::CheckMouseInBounds(mousePos, removeBonus->rect))
	{
		//todo remove a bonus line.
	}
	for (std::shared_ptr<UIEntry> bonus : bonuses)
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
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->Update(window, timePassed, input, showCursor);
	}
}

void ContinentEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);	
	for (std::shared_ptr<sf::Text> continent : continents)
	{
		continent->move(offset);
	}
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->MoveEntry(offset);
	}
}

//-----------------------------------------------------------

void BonusLine::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::Text> bonusLabel = 
		std::make_shared<sf::Text>(UI::font, "Bonus:");
	bonusLabel->setPosition({ 150, entryTop + 46 });
	labels.push_back(bonusLabel);

	std::shared_ptr<sf::Text> requiredLabel = 
		std::make_shared<sf::Text>(UI::font, "Required:");
	requiredLabel->setPosition({ 310, entryTop + 46 });
	labels.push_back(requiredLabel);

	std::shared_ptr<Continent> data = xmlData.continents.at(xmlKey);
	std::shared_ptr<TextBox> bonusBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 250, entryTop + 50 }, 
			sf::Vector2f{ 50, 30 });
	bonusBox->number = &data->bonuses[bonusNum].bonusAmount;
	boxes.push_back(bonusBox);

	std::shared_ptr<TextBox> requiredBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 450, entryTop + 50 }, 
			sf::Vector2f{ 50, 30 });
	requiredBox->number = &data->bonuses[bonusNum].numRequired;
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

void AdvancedTerritory::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{576,78 } );/*size*/
	border->setPosition({ 12,entryTop + 84 });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Cyan);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> territory = 
		std::make_shared<sf::Text>(UI::font, "TerritoryName");
	territory->setPosition({ 20, entryTop + 84 });
	labels.push_back(territory);

	std::shared_ptr<sf::Text> factorLabel = 
		std::make_shared<sf::Text>(UI::font, "Factor:");
	factorLabel->setPosition({ 410, entryTop + 84 });
	labels.push_back(factorLabel);

	std::shared_ptr<Button> mandatory  = 
		std::make_shared<Button>(sf::Vector2f{ 30, entryTop + 120 }/*position*/,
			sf::Vector2f{ 150, 30 }/*size*/, "Mandatory");
	buttons.push_back(mandatory);

	std::shared_ptr<Button> blocker = 
		std::make_shared<Button>(sf::Vector2f{ 200, entryTop + 120 }/*position*/, 
			sf::Vector2f{ 100, 30 }/*size*/, "Blocker");
	buttons.push_back(blocker);

	std::shared_ptr<Button> multiplier = 
		std::make_shared<Button>(sf::Vector2f{ 350, entryTop + 120 }/*position*/,
			sf::Vector2f{ 150, 30 }/*size*/, "Multiplier");
	buttons.push_back(multiplier);

	std::shared_ptr<TextBox> factor = 
		std::make_shared<TextBox>(sf::Vector2f{ 530, entryTop + 88 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/, new std::string("1.0"));
	boxes.push_back(factor);
}

void AdvancedTerritory::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void AdvancedTerritory::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	std::shared_ptr<Button> mandatory = buttons[(int)ButtonTypes::Mandatory];
	std::shared_ptr<Button> multiplier = buttons[(int)ButtonTypes::Multiplier];
	std::shared_ptr<Button> blocker = buttons[(int)ButtonTypes::Blocker];
			
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
	std::shared_ptr<TextBox> factor = boxes[(int)BoxTypes::FactorBox];
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