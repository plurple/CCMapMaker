#include "ContinentPage.h"
#include "UI.h"
#include "../XML/Continent.h"
#include "../EnumOperators.hpp"
#include "../XML/Territory.h"

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
		for (int i = 0; i < (int)ContinentView::COUNT; i++)
		{
			continentViews[i]->Draw(window);
		}
	}
}

void ContinentPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
	PositionEntries();
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
	for (int i = 0; i < (int)ContinentView::COUNT; i++)
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

bool ContinentPage::MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	if (UIPage::MapClick(ui, xmlData, maps, mousePos, boxIndex))
	{
		if (selectedEntry == -1)
		{
			for (int i = 0; i < entries.size(); i++)
			{
				auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[i]);
				for (int j = 0; j < entry->entries.size(); j++)
				{
					auto territory = std::dynamic_pointer_cast<AdvancedTerritory>(entry->entries[j]);
					if (territory->uiIndex == boxIndex)
					{
						SwapEntry(selectedEntry, i);
						selectedEntry = i;
					}
				}
			}
			if (selectedEntry == -1)
			{
				AddContinent(xmlData);
				auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[selectedEntry]);
				entry->AddTerritory(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
				entry->BorderBoxSize();
			}
		}
		else
		{
			auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[selectedEntry]);
			int i;
			bool removed = false;
			for (i = 0; i < entry->entries.size(); i++)
			{
				auto pair = std::dynamic_pointer_cast<AdvancedTerritory>(entry->entries[i]);
				if (pair->uiIndex == boxIndex)
				{
					maps.mapBoxes[boxIndex]->setOutlineColor(sf::Color::White);
					xmlData.continents.at(entry->xmlKey)->territories.erase(std::dynamic_pointer_cast<AdvancedTerritory>(entry->entries[i])->otherXMLKey);
					entry->entries.erase(entry->entries.begin() + i);
					removed = true;
					break;
				}
			}
			if (removed)
			{
				for (int j = i; j < entry->entries.size(); j++)
				{
					entry->entries[j]->MoveEntry({ 0, -40 });
				}
			}
			else
			{
				maps.mapBoxes[boxIndex]->setOutlineColor(sf::Color::Blue);
				entry->AddTerritory(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
			}
			entry->BorderBoxSize();
		}
		SwapView();
	}
	else
	{
		SwapEntry(selectedEntry, -1);
		selectedEntry = -1;
	}
	return true;
}

void ContinentPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
	if (input.alt)
	{
		continentViews[(int)selectedView]->Toggle();
		input.shift ? selectedView-- : selectedView++;
		continentViews[(int)selectedView]->Toggle();
		SwapView();
	}
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
		std::dynamic_pointer_cast<ContinentEntry>(entry)->SwapView(selectedView);
	}
	PositionEntries();
} 
//-----------------------------------------------------------

void ContinentEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	baseColor = sf::Color{ 255, 170, 0 };
	selectedColor = sf::Color{ 230, 150, 0 };

	territoryPos = { 20, entryTop + 84 };
	continentPos = { 300, entryTop + 84 };
	bonusPos = { 20, entryTop + 46 };

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
	territoryLabel->setPosition(territoryPos);
	labels.push_back(territoryLabel);	
	
	std::shared_ptr<sf::Text> continentLabel = 
		std::make_shared<sf::Text>(UI::font, "Continents:");
	continentLabel->setPosition(continentPos);
	labels.push_back(continentLabel);

	std::shared_ptr<sf::Text> factorLabel =
		std::make_shared<sf::Text>(UI::font, "Factor:");
	factorLabel->setPosition({ 370, entryTop + 84 });
	labels.push_back(factorLabel);

	std::shared_ptr<Button> addBonus = 
		std::make_shared<Button>(sf::Vector2f{ 460, entryTop + 50 }/*position*/, 
			sf::Vector2f{ 30, 30 }/*size*/, "+");
	buttons.push_back(addBonus);

	std::shared_ptr<Button> removeBonus = 
		std::make_shared<Button>(sf::Vector2f{ 500, entryTop + 50 }/*position*/, 
			sf::Vector2f{ 30, 30 }/*size*/, "-");
	buttons.push_back(removeBonus);

	std::shared_ptr<Continent> data = xmlData.continents.at(xmlKey);
	std::shared_ptr<TextBox> nameBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 120, entryTop + 12 }/*position*/,
			sf::Vector2f{ 410, 30 }/*size*/);
	nameBox->text = &data->name;
	boxes.push_back(nameBox);

	AddBonus(xmlData);

	BorderBoxSize();
	SwapView(selectedView);
	Select();
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
		for (std::shared_ptr<TextBox> continent : continents)
		{
			continent->Draw(window);
		}
	}
	if (selectedView == ContinentView::Overrides)
	{
		for (std::shared_ptr<TextBox> continent : overrides)
		{
			continent->Draw(window);
		}
	}
}

void ContinentEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);

	std::shared_ptr<Button> addBonus = buttons[(int)ButtonTypes::AddBonus];
	if (mouseOnPage && addBonus && UI::CheckMouseInBounds(mousePos, addBonus->rect))
	{
		//todo add a bonus line.
		AddBonus(xmlData);
		BorderBoxSize();
	}
	std::shared_ptr<Button> removeBonus = buttons[(int)ButtonTypes::RemoveBonus];
	if (mouseOnPage && removeBonus && UI::CheckMouseInBounds(mousePos, removeBonus->rect))
	{
		//todo remove a bonus line.
		RemoveBonus(xmlData);
		BorderBoxSize();
	}
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->MouseClick(xmlData, mousePos, mouseOnPage, select);
	}
}

void ContinentEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	for (int i = 0; i < entries.size(); i++)
	{
		if (xmlData.territories.find(std::dynamic_pointer_cast<AdvancedTerritory>(entries[i])->otherXMLKey) == xmlData.territories.end())
		{
			if (entries.size()) entries.erase(entries.begin() + i);
			i--;
		}
	}
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
	//TODO make sure that you only care about numbers entered;
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->Update(xmlData, window, timePassed, input, showCursor);
	}
}

void ContinentEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);	

	territoryPos += offset;
	continentPos += offset;
	bonusPos += offset;

	for (std::shared_ptr<TextBox> continent : continents)
	{
		continent->Move(offset);
	}
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->MoveEntry(offset);
	}
}

void ContinentEntry::SwapView(ContinentView view)
{
	selectedView = view;
	float basicView = selectedView == ContinentView::Basic;
	float overrideView = selectedView == ContinentView::Overrides;
	float advancedView = selectedView == ContinentView::Advanced;

	BorderBoxSize();

	labels[(int)LabelTypes::ContinentsLabel]->setPosition(overrideView ? territoryPos : continentPos);
	labels[(int)LabelTypes::ContinentsLabel]->setString(overrideView ? "Overides:" : "Continents:");
	labels[(int)LabelTypes::ContinentsLabel]->setScale({ (float)!advancedView,(float)!advancedView });
	labels[(int)LabelTypes::TerritoryLabel]->setScale({ (float)!overrideView,(float)!overrideView });
	labels[(int)LabelTypes::FactorLabel]->setScale({ advancedView,advancedView });

	for (auto advance : entries)
	{
		std::dynamic_pointer_cast<AdvancedTerritory>(advance)->SwapView(view);
	}
	if (selected)
	{
		Unselect();
		Select();
	}
}

void ContinentEntry::Select()
{
	UIEntry::Select();
}

void ContinentEntry::Unselect()
{
	UIEntry::Unselect();
}

void ContinentEntry::AddTerritory(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey)
{
	std::shared_ptr<AdvancedTerritory> territory =
		std::make_shared<AdvancedTerritory>(xmlKey);
	AdvancedData data;
	xmlData.continents[xmlKey]->territories.insert({otherXMLKey, data});
	territory->uiIndex = boxIndex;
	territory->mapBox = maps.mapBoxes[boxIndex];
	territory->xmlKey = xmlKey;
	territory->otherXMLKey = otherXMLKey;
	territory->CreateEntry(xmlData, territoryPos.y + entries.size() * 35.0f);
	entries.push_back(territory);
}

void ContinentEntry::AddBonus(XMLData& xmlData)
{
	int bonusNum = xmlData.continents[xmlKey]->AddBonus();
	std::shared_ptr<BonusLine> bonus =
		std::make_shared<BonusLine>(xmlKey, bonusNum);
	bonus->xmlKey = xmlKey;
	float offset = bonuses.size() ? 40.0f : 0.0f;
	bonus->CreateEntry(xmlData, bonusPos.y + bonuses.size() * offset);
	bonuses.push_back(bonus);

	BonusMove({ 0, offset });
}

void ContinentEntry::RemoveBonus(XMLData& xmlData)
{
	if (bonuses.size() > 1)
	{
		auto continent = xmlData.continents.at(xmlKey);
		auto num = std::dynamic_pointer_cast<BonusLine>(bonuses[bonuses.size() - 1])->bonusNum;
		continent->bonuses.erase(num);
		bonuses.pop_back();
		BonusMove({ 0, -40.0f });
	}
}

void ContinentEntry::BonusMove(sf::Vector2f offset)
{
	labels[(int)LabelTypes::TerritoryLabel]->move(offset);
	labels[(int)LabelTypes::ContinentsLabel]->move(offset);
	labels[(int)LabelTypes::FactorLabel]->move(offset);

	for (auto continent : continents)
	{
		continent->Move(offset);
	}
	for (auto over : overrides)
	{
		over->Move(offset);
	}
	for (auto entry : entries)
	{
		entry->MoveEntry(offset);
	}

	territoryPos += offset;
	continentPos += offset;
}

void ContinentEntry::BorderBoxSize()
{
	float borderHeight = 36.0f + 80.0f + (bonuses.size() - 1) * 40.0f;
	if (selectedView == ContinentView::Overrides)
	{
		int numOverrides = overrides.size();
		borderHeight += numOverrides ? numOverrides * 35.0f : 10.0f;
	}
	else
	{
		int numTerritories = entries.size() > continents.size() ? entries.size() : continents.size();
		borderHeight += numTerritories ? numTerritories * 35.0f : 10.0f;
	}
	std::dynamic_pointer_cast<sf::RectangleShape>(shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 530, borderHeight});
}

//-----------------------------------------------------------

void BonusLine::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::Text> bonusLabel = 
		std::make_shared<sf::Text>(UI::font, "Bonus:");
	bonusLabel->setCharacterSize(23);
	bonusLabel->setPosition({ 150, entryTop + 4 });
	labels.push_back(bonusLabel);

	std::shared_ptr<sf::Text> requiredLabel = 
		std::make_shared<sf::Text>(UI::font, "Required:");
	requiredLabel->setCharacterSize(23);
	requiredLabel->setPosition({ 290, entryTop + 4 });
	labels.push_back(requiredLabel);

	std::shared_ptr<Continent> data = xmlData.continents.at(xmlKey);
	std::shared_ptr<TextBox> bonusBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 230, entryTop + 4 }, 
			sf::Vector2f{ 50, 30 });
	bonusBox->number = &data->bonuses.at(bonusNum).bonusAmount;
	boxes.push_back(bonusBox);

	std::shared_ptr<TextBox> requiredBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 400, entryTop + 4 }, 
			sf::Vector2f{ 50, 30 });
	requiredBox->number = &data->bonuses[bonusNum].numRequired;
	boxes.push_back(requiredBox);
}

void BonusLine::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void BonusLine::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);
}

void BonusLine::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void BonusLine::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}

//-----------------------------------------------------------

void AdvancedTerritory::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{526,30 } );/*size*/
	border->setPosition({ 12,entryTop + 35 });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Cyan);
	shapes.push_back(border);

	std::shared_ptr<Continent> data = xmlData.continents.at(xmlKey);
	std::shared_ptr<Button> mandatory  = 
		std::make_shared<Button>(sf::Vector2f{ 230, entryTop + 40 }/*position*/,
			sf::Vector2f{ 105, 20 }/*size*/, "Mandatory");
	mandatory->label->setCharacterSize(20);
	mandatory->xmlLink = &data->territories.at(otherXMLKey).mandatory;
	buttons.push_back(mandatory);

	std::shared_ptr<Button> blocker = 
		std::make_shared<Button>(sf::Vector2f{ 340, entryTop + 40 }/*position*/, 
			sf::Vector2f{ 75, 20 }/*size*/, "Blocker");
	blocker->label->setCharacterSize(20);
	blocker->xmlLink = &data->territories.at(otherXMLKey).blocker;
	buttons.push_back(blocker);

	std::shared_ptr<Button> multiplier = 
		std::make_shared<Button>(sf::Vector2f{ 420, entryTop + 40 }/*position*/,
			sf::Vector2f{ 90, 20 }/*size*/, "Multiplier");
	multiplier->label->setCharacterSize(20);
	multiplier->xmlLink = &data->territories.at(otherXMLKey).multiplier;
	buttons.push_back(multiplier);

	std::shared_ptr<TextBox> territoryBox =
		std::make_shared<TextBox>(sf::Vector2f{ 20, entryTop+40 }, 
			sf::Vector2f{ 200, 20 });
	territoryBox->displayText->setCharacterSize(20);
	territoryBox->text = &xmlData.territories.at(otherXMLKey)->name;
	boxes.push_back(territoryBox);

	std::shared_ptr<TextBox> factor = 
		std::make_shared<TextBox>(sf::Vector2f{ 480, entryTop + 40 }/*position*/,
			sf::Vector2f{ 60, 20 }/*size*/, new std::string("1.0"));
	factor->displayText->setCharacterSize(20);
	//factor->number = &data->territories.at(otherXMLKey).factor;
	boxes.push_back(factor);
}

void AdvancedTerritory::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void AdvancedTerritory::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	if (selectedView == ContinentView::Advanced)
	{
		std::shared_ptr<Button> mandatory = buttons[(int)ButtonTypes::Mandatory];
		std::shared_ptr<Button> multiplier = buttons[(int)ButtonTypes::Multiplier];
		std::shared_ptr<Button> blocker = buttons[(int)ButtonTypes::Blocker];
		std::shared_ptr<TextBox> factor = boxes[(int)BoxTypes::FactorBox];
		if (factor)
		{
			factor->Toggle(multiplier && multiplier->selected &&
				mouseOnPage && UI::CheckMouseInBounds(mousePos, factor->box));
		}
		if (!factor->active && mouseOnPage && mandatory && multiplier && blocker)
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
	}
	std::shared_ptr<TextBox> name = boxes[(int)BoxTypes::TerritoryName];
	if (name)
	{
		name->Toggle(mouseOnPage && UI::CheckMouseInBounds(mousePos, name->box));
	}
}

void AdvancedTerritory::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	float mult = buttons[(int)ButtonTypes::Multiplier]->selected && shapes[0]->getScale().y;
	boxes[(int)BoxTypes::FactorBox]->Hide(mult);
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void AdvancedTerritory::MoveEntry(sf::Vector2f offset)
{	
	UIEntry::MoveEntry(offset);
}

void AdvancedTerritory::SwapView(ContinentView view)
{
	selectedView = view;
	float overrideView = view == ContinentView::Overrides;
	float advancedView = view == ContinentView::Advanced;

	shapes[(int)ShapeTypes::Border]->setScale({ advancedView, advancedView });
	buttons[(int)ButtonTypes::Mandatory]->Hide(advancedView);
	buttons[(int)ButtonTypes::Blocker]->Hide(advancedView);
	buttons[(int)ButtonTypes::Multiplier]->Hide(advancedView);
	boxes[(int)BoxTypes::TerritoryName]->Hide(!overrideView);
	boxes[(int)BoxTypes::FactorBox]->Hide(advancedView);
}