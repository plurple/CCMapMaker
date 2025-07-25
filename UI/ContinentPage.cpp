#include "ContinentPage.h"
#include "UI.h"
#include "../XML/Continent.h"
#include "../EnumOperators.hpp"
#include "../XML/Territory.h"

ContinentPage::ContinentPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize,
	bool& continentPanel) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize, continentPanel),
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
	addEntry.rect->setSize({ 200, 30 });
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
	if (showContinents.selected || selectedView != ContinentView::Advanced)
	{
		if (UI::CheckMouseInBounds(mousePos, *showContinents.rect))
		{
			showContinents.Toggle();
		}
	}
	else
	{
		showContinents.Unselect();
	}

	if (UI::CheckMouseInBounds(mousePos, *addEntry.rect))
	{
		AddContinent(xmlData);
	}
	for (int i = 0; i < (int)ContinentView::COUNT; i++)
	{
		if (!showContinents.selected && UI::CheckMouseInBounds(mousePos, *continentViews[i]->rect))
		{
			continentViews[(int)selectedView]->Toggle();
			selectedView = (ContinentView)i;
			continentViews[(int)selectedView]->Toggle();
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
			if (selectedView != ContinentView::Overrides && selectedEntry == -1)
			{
				AddContinent(xmlData);
				auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[selectedEntry]);
				entry->AddTerritory(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
				entry->BorderBoxSize();
			}
		}
		else if(selectedView != ContinentView::Overrides)
		{
			auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[selectedEntry]);
			int i;
			bool removed = false;
			for (i = 0; i < entry->entries.size(); i++)
			{
				auto pair = std::dynamic_pointer_cast<AdvancedTerritory>(entry->entries[i]);
				if (pair->uiIndex == boxIndex)
				{
					maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::White);
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
				maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::Blue);
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

bool ContinentPage::ContinentClick(UI& ui, XMLData& xmlData,
	ContinentPanel& panel, sf::Vector2i mousePos, int& continentIndex)
{
	if (UIPage::ContinentClick(ui, xmlData, panel, mousePos, continentIndex))
	{
		if (selectedEntry == -1)
		{
			SwapEntry(selectedEntry, continentIndex);
			selectedEntry = continentIndex;
		}
		else
		{
			if (selectedEntry == continentIndex)
			{
				SwapEntry(selectedEntry, -1);
				selectedEntry = -1;
			}
			else
			{
				switch (selectedView)
				{
				case ContinentView::Basic:
				{
					auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[selectedEntry]);
					int i;
					bool removed = false;
					for (i = 0; i < entry->continents.size(); i++)
					{
						auto continent = std::dynamic_pointer_cast<LinkedData>(entry->continents[i]);
						if (continent->uiIndex == continentIndex)
						{
							panel.continents[continentIndex]->box.rect->setOutlineColor(sf::Color::White);
							xmlData.continents.at(entry->xmlKey)->continents.erase(xmlData.continents.at(entry->xmlKey)->continents.begin() + i);
							entry->continents.erase(entry->continents.begin() + i);
							removed = true;
							break;
						}
					}
					if (removed)
					{
						for (int j = i; j < entry->continents.size(); j++)
						{
							entry->continents[j]->nameLabel->Move({ 0, -40 });
						}
					}
					else
					{
						panel.continents[continentIndex]->box.rect->setOutlineColor(sf::Color::Blue);
						entry->AddContinent(xmlData, panel, continentIndex, ui.uiPages[(int)UIPageType::Continent]->entries[continentIndex]->xmlKey, false);
					}
					entry->BorderBoxSize();
					break;
				}
				case ContinentView::Overrides:
				{
					auto entry = std::dynamic_pointer_cast<ContinentEntry>(entries[selectedEntry]);
					int i;
					bool removed = false;
					for (i = 0; i < entry->overrides.size(); i++)
					{
						auto over = std::dynamic_pointer_cast<LinkedData>(entry->overrides[i]);
						if (over->uiIndex == continentIndex)
						{
							panel.continents[continentIndex]->box.rect->setOutlineColor(sf::Color::White);
							xmlData.continents.at(entry->xmlKey)->overrides.erase(xmlData.continents.at(entry->xmlKey)->overrides.begin() + i);
							entry->overrides.erase(entry->overrides.begin() + i);
							removed = true;
							break;
						}
					}
					if (removed)
					{
						for (int j = i; j < entry->overrides.size(); j++)
						{
							entry->overrides[j]->nameLabel->Move({ 0, -40 });
						}
					}
					else
					{
						panel.continents[continentIndex]->box.rect->setOutlineColor(sf::Color::Blue);
						entry->AddContinent(xmlData, panel, continentIndex, ui.uiPages[(int)UIPageType::Continent]->entries[continentIndex]->xmlKey, true);
					}
					entry->BorderBoxSize();
					break;
				}
				}
			}
		}		
		SwapView();
	}
	return true;
}

void ContinentPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor, UIPageType pageType)
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
	selectedColor = sf::Color{ 200, 120, 0 };

	territoryPos = { 20, entryTop + 122 };
	continentPos = { 280, entryTop + 122 };
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

	std::shared_ptr<sf::Text> requiredLabel =
		std::make_shared<sf::Text>(UI::font, "Required:");
	requiredLabel->setPosition({ 20, entryTop + 84 });
	labels.push_back(requiredLabel);
	
	std::shared_ptr<sf::Text> territoryLabel = 
		std::make_shared<sf::Text>(UI::font, "Territories:");
	territoryLabel->setPosition(territoryPos);
	territoryLabel->setFillColor(sf::Color::Blue);
	labels.push_back(territoryLabel);	
	
	std::shared_ptr<sf::Text> continentLabel = 
		std::make_shared<sf::Text>(UI::font, "Continents:");
	continentLabel->setPosition(continentPos);
	continentLabel->setFillColor(sf::Color::Red);
	labels.push_back(continentLabel);

	std::shared_ptr<sf::Text> factorLabel =
		std::make_shared<sf::Text>(UI::font, "Factor:");
	factorLabel->setPosition({ 370, entryTop + 122 });
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

	std::shared_ptr<TextBox> requiredBox =
		std::make_shared<TextBox>(sf::Vector2f{ 160, entryTop + 88 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	requiredBox->number = &data->required;
	boxes.push_back(requiredBox);

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
		for (std::shared_ptr<LinkedData> continent : continents)
		{
			continent->nameLabel->Draw(window);
		}
	}
	if (selectedView == ContinentView::Overrides)
	{
		for (std::shared_ptr<LinkedData> over : overrides)
		{
			over->nameLabel->Draw(window);
		}
	}
}

void ContinentEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, 
	bool mouseOnPage, bool& select, bool mapClicked)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);

	std::shared_ptr<Button> addBonus = buttons[(int)ButtonTypes::AddBonus];
	if (mouseOnPage && addBonus && UI::CheckMouseInBounds(mousePos, *addBonus->rect))
	{
		AddBonus(xmlData);
		BorderBoxSize();
	}
	std::shared_ptr<Button> removeBonus = buttons[(int)ButtonTypes::RemoveBonus];
	if (mouseOnPage && removeBonus && UI::CheckMouseInBounds(mousePos, *removeBonus->rect))
	{
		RemoveBonus(xmlData);
		BorderBoxSize();
	}
	for (std::shared_ptr<UIEntry> bonus : bonuses)
	{
		bonus->MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);
	}
	if (selectedView == ContinentView::Basic)
	{
		for (std::shared_ptr<LinkedData> continent : continents)
		{
			continent->nameLabel->Toggle(mouseOnPage && UI::CheckMouseInBounds(mousePos, continent->nameLabel->box));
		}
	}
	if (selectedView == ContinentView::Overrides)
	{
		for (std::shared_ptr<LinkedData> over : overrides)
		{
			over->nameLabel->Toggle(mouseOnPage && UI::CheckMouseInBounds(mousePos, over->nameLabel->box));
		}
	}
}

void ContinentEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	for (int i = 0; i < entries.size(); i++)
	{
		if (xmlData.territories.find(std::dynamic_pointer_cast<AdvancedTerritory>(entries[i])->otherXMLKey) == xmlData.territories.end())
		{
			if (entries.size()) entries.erase(entries.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < continents.size(); i++)
	{
		if (xmlData.continents.find(continents[i]->xmlKey) != xmlData.continents.end())
		{
			continents[i]->nameLabel->Update(window, timePassed, input, showCursor);
		}
		else
		{
			if (continents.size()) continents.erase(continents.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < overrides.size(); i++)
	{
		if (xmlData.continents.find(overrides[i]->xmlKey) != xmlData.continents.end())
		{
			overrides[i]->nameLabel->Update(window, timePassed, input, showCursor);
		}
		else
		{
			if (overrides.size()) overrides.erase(overrides.begin() + i);
			i--;
		}
	}
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
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

	for (std::shared_ptr<LinkedData> continent : continents)
	{
		continent->nameLabel->Move(offset);
	}
	for (std::shared_ptr<LinkedData> over : overrides)
	{
		over->nameLabel->Move(offset);
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
	labels[(int)LabelTypes::ContinentsLabel]->setFillColor(overrideView ? sf::Color::Green : sf::Color::Red);
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
	for (auto entry : entries)
	{
		entry->Select();
	}
	if (selectedView != ContinentView::Overrides)
	{
		for (auto continent : continents)
		{
			continent->mapBox->border->setOutlineColor(sf::Color::Red);
		}
	}
	else
	{
		for (auto continent : overrides)
		{
			continent->mapBox->border->setOutlineColor(sf::Color::Green);
		}
	}
}

void ContinentEntry::Unselect(bool white)
{
	UIEntry::Unselect();
	for (auto entry : entries)
	{
		entry->Unselect(white);
	}	
	for (auto continent : continents)
	{
		continent->mapBox->border->setOutlineColor(sf::Color::White);
	}
	for (auto continent : overrides)
	{
		continent->mapBox->border->setOutlineColor(sf::Color::White);
	}	
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
	territory->selectedColor = selectedColor;
	territory->baseColor = baseColor;
	territory->CreateEntry(xmlData, territoryPos.y + entries.size() * 35.0f);
	entries.push_back(territory);
}

void ContinentEntry::AddContinent(XMLData& xmlData, ContinentPanel& panel, int continentIndex, int otherXMLKey, bool over)
{
	std::shared_ptr<LinkedData> continent =	std::make_shared<LinkedData>();
	sf::Vector2f pos = over ? 
		territoryPos + sf::Vector2f{ 0, 40 + overrides.size() * 35.0f } : 
		continentPos + sf::Vector2f{ 0, 40 + continents.size() * 35.0f };
	continent->nameLabel = std::make_shared<TextBox>(pos,
		sf::Vector2f{ 250, 20 });
	continent->nameLabel->displayText->setCharacterSize(20);
	continent->nameLabel->baseColor = over ? sf::Color::Green : sf::Color::Red;
	continent->uiIndex = continentIndex;
	continent->mapBox = std::make_shared<MapBox>();
	continent->mapBox->border = panel.continents[continentIndex]->box.rect;
	continent->xmlKey = otherXMLKey;
	continent->nameLabel->text = &xmlData.continents[otherXMLKey]->name;
	if (over)
	{
		overrides.push_back(continent);
		xmlData.continents[xmlKey]->overrides.push_back(otherXMLKey);
	}
	else
	{
		continents.push_back(continent);
		xmlData.continents[xmlKey]->continents.push_back(otherXMLKey);
	}
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
	labels[(int)LabelTypes::RequiredLabel]->move(offset);
	boxes[(int)BoxTypes::RequiredBox]->Move(offset);

	for (auto continent : continents)
	{
		continent->nameLabel->Move(offset);
	}
	for (auto over : overrides)
	{
		over->nameLabel->Move(offset);
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
	float borderHeight = 36.0f + 120.0f + (bonuses.size() - 1) * 40.0f;
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
	bonusBox->allowNegative = true;
	bonusBox->RemoveNumber();
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

void BonusLine::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
	bool& select, bool mapClicked)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);
}

void BonusLine::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
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
	//factor->allowNegative = true;
	boxes.push_back(factor);
}

void AdvancedTerritory::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void AdvancedTerritory::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
	bool& select, bool mapClicked)
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
			if (UI::CheckMouseInBounds(mousePos, *mandatory->rect))
			{
				mandatory->Toggle();
				blocker->Unselect();
				multiplier->Unselect();
			}
			else if (UI::CheckMouseInBounds(mousePos, *blocker->rect))
			{
				blocker->Toggle();
				mandatory->Unselect();
				multiplier->Unselect();
			}
			else if (UI::CheckMouseInBounds(mousePos, *multiplier->rect))
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
	UserInput& input, bool showCursor)
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

void AdvancedTerritory::Select()
{
	mapBox->border->setOutlineColor(selectedColor);
}

void AdvancedTerritory::Unselect(bool white)
{
	mapBox->border->setOutlineColor(white ? sf::Color::White : baseColor);
}