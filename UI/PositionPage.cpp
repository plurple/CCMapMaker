#include "PositionPage.h"
#include "UI.h"
#include "../XML/Position.h"
#include "../XML/Territory.h"

PositionPage::PositionPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize, 
	bool& continentPanel) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize, continentPanel),
    maxBox({ 1470, 170 }, { 50, 30 })
{
    maxLabel = std::make_shared<sf::Text>(UI::font, "Max Positions:");
    maxLabel->setPosition({ 1270, 165 });

	addEntry.SetPosition({ 1070, 170 });
	addEntry.rect->setSize({ 185, 30 });
	addEntry.label->setString("Add Position");

	maxBox.number = &xmlData.maxPositions;
}

void PositionPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		maxBox.Draw(window);
		if(maxLabel) 
			window.draw(*maxLabel);
	}
}

void PositionPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
	if (!showContinents.selected && UI::CheckMouseInBounds(mousePos, *addEntry.rect))
	{
		AddPosition(xmlData);
	}
	maxBox.Toggle(!showContinents.selected && UI::CheckMouseInBounds(mousePos, maxBox.box));
}

bool PositionPage::MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	if (UIPage::MapClick(ui, xmlData, maps, mousePos, boxIndex))
	{
		if (selectedEntry == -1)
		{
			for (int i = 0; i < entries.size(); i++)
			{
				auto position = std::dynamic_pointer_cast<PositionEntry>(entries[i]);
				for (int j = 0; j < position->positionPairs.size(); j++)
				{
					auto pair = std::dynamic_pointer_cast<PositionPair>(position->positionPairs[j]);
					if (pair->uiIndex == boxIndex)
					{
						SwapEntry(selectedEntry, i);
						selectedEntry = i;
					}
				}
			}
			if (selectedEntry == -1)
			{
				AddPosition(xmlData);
				auto position = std::dynamic_pointer_cast<PositionEntry>(entries[selectedEntry]);
				position->AddPositionPair(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
				std::dynamic_pointer_cast<sf::RectangleShape>(entries[selectedEntry]->shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 530, 10 + position->positionPairs.size() * 40.0f });
			}
		}
		else
		{
			auto position = std::dynamic_pointer_cast<PositionEntry>(entries[selectedEntry]);
			int i;
			bool removed = false;
			for (i = 0; i < position->positionPairs.size(); i++)
			{
				auto pair = std::dynamic_pointer_cast<PositionPair>(position->positionPairs[i]);
				if (pair->uiIndex == boxIndex)
				{
					maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::White);
					xmlData.positions.at(position->xmlKey)->positions.erase(std::dynamic_pointer_cast<PositionPair>(position->positionPairs[i])->otherXMLKey);
					position->positionPairs.erase(position->positionPairs.begin() + i);
					removed = true;
					break;
				}
			}
			if (removed)
			{
				for (int j = i; j < position->positionPairs.size(); j++)
				{
					position->positionPairs[j]->MoveEntry({ 0, -40 });
				}
			}
			else
			{
				maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::Blue);
				position->AddPositionPair(xmlData, maps, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
			}
			std::dynamic_pointer_cast<sf::RectangleShape>(position->shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 530, 10 + position->positionPairs.size() * 40.0f });
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

void PositionPage::Update(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Time timePassed, UserInput input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, 
		pageType);
	maxBox.Update(window, timePassed, input, showCursor);
}

void PositionPage::AddPosition(XMLData& xmlData)
{
	std::shared_ptr<PositionEntry> entry = 
		std::make_shared<PositionEntry>(xmlData.AddPosition());
	UIPage::AddEntry(xmlData, entry);
}

void PositionPage::SelectPage()
{
	UIPage::SelectPage();
	for (auto entry : entries)
	{
		for (auto pair : std::dynamic_pointer_cast<PositionEntry>(entry)->positionPairs)
		{
			std::dynamic_pointer_cast<PositionPair>(pair)->mapBox->border->setOutlineColor(pair->baseColor);
		}
	}
}

void PositionPage::UnselectPage()
{
	UIPage::UnselectPage();
	for (auto entry : entries)
	{
		for (auto pair : std::dynamic_pointer_cast<PositionEntry>(entry)->positionPairs)
		{
			std::dynamic_pointer_cast<PositionPair>(pair)->mapBox->border->setOutlineColor(sf::Color::White);
		}
	}
}

//-----------------------------------------------------------

void PositionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	baseColor = sf::Color::Green;
	selectedColor = sf::Color{ 26, 176, 26 };
	pairsPos = entryTop;

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{530, 20} );
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Green);
	shapes.push_back(border);

	Select();
}

void PositionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	for (std::shared_ptr<UIEntry> entry : positionPairs)
	{
		entry->Draw(window);
	}
}

void PositionEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);
	
	for (std::shared_ptr<UIEntry> entry : positionPairs)
	{
		entry->MouseClick(xmlData, mousePos, mouseOnPage, select);
	}
}

void PositionEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
	pairsPos = shapes[0]->getPosition().y;
	for (int i = 0; i < positionPairs.size(); i++)
	{
		if (xmlData.territories.find(std::dynamic_pointer_cast<PositionPair>(positionPairs[i])->otherXMLKey) != xmlData.territories.end())
		{
			positionPairs[i]->Update(xmlData, window, timePassed, input, showCursor);
		}
		else
		{
			Unselect();
			if (positionPairs.size()) positionPairs.erase(positionPairs.begin() + i);
			i--;
		}
	}
}

void PositionEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
	for (std::shared_ptr<UIEntry> entry : positionPairs)
	{
		entry->MoveEntry(offset);
	}
}

void PositionEntry::AddPositionPair(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey)
{
	std::shared_ptr<PositionPair> pair = 
		std::make_shared<PositionPair>(xmlKey);
	xmlData.positions[xmlKey]->positions.insert({ otherXMLKey, 3 });
	pair->uiIndex = boxIndex;
	pair->mapBox = maps.mapBoxes[boxIndex];
	pair->xmlKey = xmlKey;
	pair->otherXMLKey = otherXMLKey;
	pair->CreateEntry(xmlData,  pairsPos + positionPairs.size() * 40.0f);
	positionPairs.push_back(pair);
}

void PositionEntry::Select()
{
	UIEntry::Select();
	for (auto position : positionPairs)
	{
		position->selected = selected;
	}
}

void PositionEntry::Unselect()
{
	UIEntry::Unselect();
	for (auto position : positionPairs)
	{
		position->selected = selected;
	}
}

//-------------------------------------------------------------------------

PositionPair::~PositionPair()
{
	mapBox->border->setOutlineColor(sf::Color::White);
}

void PositionPair::CreateEntry(XMLData& xmlData, float entryTop)
{
	baseColor = sf::Color::Green;
	selectedColor = sf::Color{ 26, 176, 26 };

	std::shared_ptr<TextBox> nameLabel = std::make_shared<TextBox>(sf::Vector2f{ 30, entryTop + 12 },
		sf::Vector2f{ 250, 30 });
	nameLabel->baseColor = sf::Color::Green;
	nameLabel->text = &xmlData.territories.at(otherXMLKey)->name;
	boxes.push_back(nameLabel);

	std::shared_ptr<sf::Text> startLabel =
		std::make_shared<sf::Text>(UI::font, "Start Size:");
	startLabel->setPosition({ 330, entryTop + 8 });
	labels.push_back(startLabel);

	std::shared_ptr<Position> data = xmlData.positions.at(xmlKey);
	std::shared_ptr<TextBox> startBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 475, entryTop + 12 }/*position*/, 
			sf::Vector2f{ 50, 30 }/*size*/);
	startBox->number = &data->positions.at(otherXMLKey);
	boxes.push_back(startBox);
}

void PositionPair::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void PositionPair::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);
}

void PositionPair::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	mapBox->border->setOutlineColor(selected ? selectedColor : baseColor);
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void PositionPair::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}