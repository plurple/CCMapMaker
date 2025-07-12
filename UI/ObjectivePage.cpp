#include "ObjectivePage.h"
#include "UI.h"
#include "../XML/Objective.h"
#include "../XML/Territory.h"
#include "../XML/Continent.h"

ObjectivePage::ObjectivePage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize, 
	bool& continentPanel) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize, continentPanel),
    isObjective{ true }
{
    isObjective = tabLabel != "Requirements";
	addEntry.SetPosition({ 1051, 170 });
	addEntry.rect->setSize(isObjective ? sf::Vector2f{ 200, 30 } : sf::Vector2f{245, 30});
	addEntry.label->setString(isObjective ? "Add Objective" : "Add Requirement");

	showContinents.SetPosition({ 1311, 170 });    
}

void ObjectivePage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		showContinents.Draw(window);
	}
}

void ObjectivePage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
    if (!showContinents.selected && UI::CheckMouseInBounds(mousePos, *showContinents.rect))
    {
        showContinents.Toggle();
    }
    if (!showContinents.selected && UI::CheckMouseInBounds(mousePos, *addEntry.rect))
    {
		AddObjective(xmlData);
    }
}

bool ObjectivePage::MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	if (UIPage::MapClick(ui, xmlData, maps, mousePos, boxIndex))
	{
		if (selectedEntry == -1)
		{
			for (int i = 0; i < entries.size(); i++)
			{
				auto entry = std::dynamic_pointer_cast<ObjectiveEntry>(entries[i]);
				for (int j = 0; j < entry->territories.size(); j++)
				{
					auto territory = entry->territories[j];
					if (territory->uiIndex == boxIndex)
					{
						SwapEntry(selectedEntry, i);
						selectedEntry = i;
					}
				}
			}
			if (selectedEntry == -1)
			{
				AddObjective(xmlData);
				auto entry = std::dynamic_pointer_cast<ObjectiveEntry>(entries[selectedEntry]);
				entry->AddTerritory(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
				entry->BorderBoxSize();
			}
		}
		else
		{
			auto entry = std::dynamic_pointer_cast<ObjectiveEntry>(entries[selectedEntry]);
			int i;
			bool removed = false;
			for (i = 0; i < entry->territories.size(); i++)
			{
				auto territory = entry->territories[i];
				if (territory->uiIndex == boxIndex)
				{
					maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::White);
					if(isObjective)
						xmlData.objectives.at(entry->xmlKey)->territories.erase(xmlData.objectives.at(entry->xmlKey)->territories.begin() + i);
					else
						xmlData.requirements.at(entry->xmlKey)->territories.erase(xmlData.requirements.at(entry->xmlKey)->territories.begin() + i);

					entry->territories.erase(entry->territories.begin() + i);
					removed = true;
					break;
				}
			}
			if (removed)
			{
				for (int j = i; j < entry->territories.size(); j++)
				{
					entry->territories[j]->nameLabel->Move({ 0, -25 });
				}
			}
			else
			{
				maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::Blue);
				entry->AddTerritory(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
			}
			entry->BorderBoxSize();
		}
		PositionEntries();
	}
	else
	{
		SwapEntry(selectedEntry, -1);
		selectedEntry = -1;
	}
	return true;
}

bool ObjectivePage::ContinentClick(UI& ui, XMLData& xmlData, ContinentPanel& panel, sf::Vector2i mousePos, int& continentIndex)
{
	if (UIPage::ContinentClick(ui, xmlData, panel, mousePos, continentIndex))
	{
		if (selectedEntry == -1)
		{
			for (int i = 0; i < entries.size(); i++)
			{
				auto entry = std::dynamic_pointer_cast<ObjectiveEntry>(entries[i]);
				for (int j = 0; j < entry->continents.size(); j++)
				{
					auto continent = entry->continents[j];
					if (continent->uiIndex == continentIndex)
					{
						SwapEntry(selectedEntry, i);
						selectedEntry = i;
					}
				}
			}
			if (selectedEntry == -1)
			{
				AddObjective(xmlData);
				auto entry = std::dynamic_pointer_cast<ObjectiveEntry>(entries[selectedEntry]);
				entry->AddContinent(xmlData, panel, continentIndex, ui.uiPages[(int)UIPageType::Continent]->entries[continentIndex]->xmlKey);
				entry->BorderBoxSize();
			}
		}
		else
		{
			auto entry = std::dynamic_pointer_cast<ObjectiveEntry>(entries[selectedEntry]);
			int i;
			bool removed = false;
			for (i = 0; i < entry->continents.size(); i++)
			{
				auto continent = entry->continents[i];
				if (continent->uiIndex == continentIndex)
				{
					panel.continents[continentIndex]->box.rect->setOutlineColor(sf::Color::White);
					if (isObjective)
						xmlData.objectives.at(entry->xmlKey)->continents.erase(xmlData.objectives.at(entry->xmlKey)->continents.begin() + i);
					else
						xmlData.requirements.at(entry->xmlKey)->continents.erase(xmlData.requirements.at(entry->xmlKey)->continents.begin() + i);

					entry->continents.erase(entry->continents.begin() + i);
					removed = true;
					break;
				}
			}
			if (removed)
			{
				for (int j = i; j < entry->continents.size(); j++)
				{
					entry->continents[j]->nameLabel->Move({ 0, -25 });
				}
			}
			else
			{
				panel.continents[continentIndex]->box.rect->setOutlineColor(sf::Color::Blue);
				entry->AddContinent(xmlData, panel, continentIndex, ui.uiPages[(int)UIPageType::Continent]->entries[continentIndex]->xmlKey);
			}
			entry->BorderBoxSize();
		}
		PositionEntries();
	}
	else
	{
		SwapEntry(selectedEntry, -1);
		selectedEntry = -1;
	}
	return true;
}

void ObjectivePage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
    UserInput& input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
}

void ObjectivePage::AddObjective(XMLData& xmlData)
{
	std::shared_ptr<ObjectiveEntry> entry = 
		std::make_shared<ObjectiveEntry>( isObjective,
		isObjective ? xmlData.AddObjective() : xmlData.AddRequirement() );
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void ObjectiveEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	baseColor = isObjective ? sf::Color{ 250, 170, 180 } : sf::Color::Magenta;
	selectedColor = isObjective ? sf::Color(230, 100, 110) : sf::Color{ 175, 0, 175 };

	territoryPos = { 20, entryTop + 120 };
	continentPos = { 280, entryTop + 120 };

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{530, 165} );
	border->setPosition({ 10,entryTop });
	border->setFillColor({ sf::Color::Transparent });
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> nameLabel = 
		std::make_shared<sf::Text>(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);

	std::shared_ptr<sf::Text> requiredLabel =
		std::make_shared<sf::Text>(UI::font, "Num Required:");
	requiredLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(requiredLabel);

	std::shared_ptr<sf::Text> territoryLabel = 
		std::make_shared<sf::Text>(UI::font, "Territories:");
	territoryLabel->setPosition({ 20, entryTop + 84 });
	territoryLabel->setFillColor(sf::Color::Blue);
	labels.push_back(territoryLabel);
	
	std::shared_ptr<sf::Text> continentLabel = 
		std::make_shared<sf::Text>(UI::font, "Continents:");
	continentLabel->setPosition({ 280, entryTop + 84 });
	continentLabel->setFillColor(sf::Color{ 230, 150, 0 });
	labels.push_back(continentLabel);	

	std::shared_ptr<Objective> data = isObjective ? xmlData.objectives.at(xmlKey) :
		xmlData.requirements.at(xmlKey);
	std::shared_ptr<TextBox> nameBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 110, entryTop + 12 }/*position*/,
		sf::Vector2f{ 420, 30 }/*size*/);
	nameBox->text = &data->name;
	boxes.push_back(nameBox);

	std::shared_ptr<TextBox> numRequiredBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 235, entryTop + 50 }/*position*/,
		sf::Vector2f{ 50, 30 }/*size*/);
	numRequiredBox->number = &data->numRequired;
	boxes.push_back(numRequiredBox);

	Select();
	BorderBoxSize();
}

void ObjectiveEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	for (auto territory : territories)
	{
		territory->nameLabel->Draw(window);
	}
	for (auto continent : continents)
	{
		continent->nameLabel->Draw(window);
	}
}

void ObjectiveEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, 
	bool mouseOnPage, bool& select, bool mapClicked)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);
	for (auto territory : territories)
	{
		territory->nameLabel->Toggle(mouseOnPage && UI::CheckMouseInBounds(mousePos, territory->nameLabel->box));
	}
	for (auto continent : continents)
	{
		continent->nameLabel->Toggle(mouseOnPage && UI::CheckMouseInBounds(mousePos, continent->nameLabel->box));
	}
}

void ObjectiveEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll});
	for (int i = 0; i< territories.size(); i++)
	{
		if (xmlData.territories.find(territories[i]->xmlKey) != xmlData.territories.end())
		{
			territories[i]->nameLabel->Update(window, timePassed, input, showCursor);
		}
		else
		{
			if (territories.size()) territories.erase(territories.begin() + i);
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

	int numRequired = *boxes[(int)BoxTypes::NumRequired]->number;
	int potentialRequired = territories.size() + continents.size();
	if (numRequired > potentialRequired)
		*boxes[(int)BoxTypes::NumRequired]->number = (territories.size() + continents.size());
}

void ObjectiveEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);

	territoryPos += offset;
	continentPos += offset;

	for (auto territory : territories)
	{
		territory->nameLabel->Move(offset);
	}
	for (auto continent : continents)
	{
		continent->nameLabel->Move(offset);
	}
}

void ObjectiveEntry::Select()
{
	UIEntry::Select();
	for (auto territory : territories)
	{
		territory->mapBox->border->setOutlineColor(selectedColor);
	}
	for (auto continent : continents)
	{
		continent->mapBox->border->setOutlineColor(selectedColor);
	}
}

void ObjectiveEntry::Unselect(bool white)
{
	UIEntry::Unselect();
	for (auto territory : territories)
	{
		territory->mapBox->border->setOutlineColor(white ? sf::Color::White : baseColor);
	}
	for (auto continent : continents)
	{
		continent->mapBox->border->setOutlineColor(white ? sf::Color::White : baseColor);
	}
}

void ObjectiveEntry::AddTerritory(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey)
{
	std::shared_ptr<LinkedData> territory = std::make_shared<LinkedData>();
	territory->nameLabel = std::make_shared<TextBox>(territoryPos + sf::Vector2f{ 0, territories.size() * 25.0f },
		sf::Vector2f{ 250, 20 });
	territory->nameLabel->displayText->setCharacterSize(20);
	territory->nameLabel->baseColor = sf::Color::Blue;
	territory->uiIndex = boxIndex;
	territory->mapBox = maps.mapBoxes[boxIndex];
	territory->xmlKey = otherXMLKey;
	territory->nameLabel->text = &xmlData.territories[otherXMLKey]->name;
	territories.push_back(territory);
	isObjective ? xmlData.objectives[xmlKey]->territories.push_back(otherXMLKey) : 
		xmlData.requirements[xmlKey]->territories.push_back(otherXMLKey);
	if (isObjective && (*boxes[(int)BoxTypes::NumRequired]->number + 1) ==
		(territories.size() + continents.size()))
	{
		*boxes[(int)BoxTypes::NumRequired]->number += 1;
	}
	else if(*boxes[(int)BoxTypes::NumRequired]->number == 0)
		*boxes[(int)BoxTypes::NumRequired]->number = 1;
}

void ObjectiveEntry::AddContinent(XMLData& xmlData, ContinentPanel& panel, int continentIndex, int otherXMLKey)
{
	std::shared_ptr<LinkedData> continent = std::make_shared<LinkedData>();
	continent->nameLabel = std::make_shared<TextBox>(continentPos + sf::Vector2f{ 0, continents.size() * 25.0f },
		sf::Vector2f{ 250, 20 });
	continent->nameLabel->displayText->setCharacterSize(20);
	continent->nameLabel->baseColor = sf::Color{ 230, 150, 0 };
	continent->uiIndex = continentIndex;
	continent->mapBox = std::make_shared<MapBox>();
	continent->mapBox->border = panel.continents[continentIndex]->box.rect;
	continent->xmlKey = otherXMLKey;
	continent->nameLabel->text = &xmlData.continents[otherXMLKey]->name;
	continents.push_back(continent);
	isObjective ? xmlData.objectives[xmlKey]->continents.push_back(otherXMLKey) :
		xmlData.requirements[xmlKey]->continents.push_back(otherXMLKey);
	if (isObjective && (*boxes[(int)BoxTypes::NumRequired]->number + 1) ==
		(territories.size() + continents.size()))
	{
		*boxes[(int)BoxTypes::NumRequired]->number += 1;
	}
	else if (*boxes[(int)BoxTypes::NumRequired]->number == 0)
		*boxes[(int)BoxTypes::NumRequired]->number = 1;
}

void ObjectiveEntry::BorderBoxSize()
{
	float borderHeight = 120.0f;

	int numTerritories = territories.size() > continents.size() ? territories.size() : continents.size();
	borderHeight += numTerritories ? numTerritories * 25.0f : 10.0f;

	std::dynamic_pointer_cast<sf::RectangleShape>(shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 530, borderHeight });
}