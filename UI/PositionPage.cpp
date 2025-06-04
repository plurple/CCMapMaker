#include "PositionPage.h"
#include "UI.h"
#include "../XML/Position.h"

PositionPage::PositionPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
    maxBox({ 1470, 170 }, { 50, 30 })
{
    maxLabel = new sf::Text(UI::font, "Max Positions:");
    maxLabel->setPosition({ 1270, 165 });

	addEntry.SetPosition({ 1070, 170 });
	addEntry.rect.setSize({ 185, 30 });
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

void PositionPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(xmlData, window, mousePos);
	if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
	{
		AddPosition(xmlData);
	}
	maxBox.active = UI::CheckMouseInBounds(mousePos, maxBox.box);	
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
	PositionEntry* entry = new PositionEntry{xmlData.AddPosition()};
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void PositionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ {580, 50} };
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Green);
	shapes.push_back(border);

	Button* add = new Button({ 220, entryTop + 12 }/*position*/, { 200, 30 }/*size*/, "Add Position");
	buttons.push_back(add);

	PositionPair* posPair = new PositionPair(xmlKey, positionPairs.size());
	posPair->CreateEntry(xmlData, entryTop);
	positionPairs.push_back(posPair);
}

void PositionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	for (UIEntry* entry : positionPairs)
	{
		entry->Draw(window);
	}
}

void PositionEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	sf::Shape* borderBox = shapes[(int)ShapeTypes::Border];
	if (mouseOnPage && borderBox)
	{
		if (UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()))
		{
			selected = true;
			borderBox->setOutlineThickness(4.0f);
			borderBox->setOutlineColor({ 26, 176, 26 });
			//TODO be able to pick a territory from the map
		}
		else
		{
			selected = false;
			borderBox->setOutlineThickness(2.0f);
			borderBox->setOutlineColor(sf::Color::Green);
		}
	}
	for (UIEntry* entry : positionPairs)
	{
		entry->MouseClick(mousePos, mouseOnPage);
	}
}

void PositionEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
	for (UIEntry* entry : positionPairs)
	{
		entry->Update(window, timePassed, input, showCursor);
	}
}

void PositionEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
	for (UIEntry* entry : positionPairs)
	{
		entry->MoveEntry(offset);
	}
}

void PositionPair::CreateEntry(XMLData& xmlData, float entryTop)
{
	sf::Text* territoryName = new sf::Text(UI::font, "Territory Name");
	territoryName->setPosition({ 30, entryTop + 8 });
	labels.push_back(territoryName);

	sf::Text* startLabel = new sf::Text(UI::font, "Start Size:");
	startLabel->setPosition({ 380, entryTop + 8 });
	labels.push_back(startLabel);

	Position* data = xmlData.positions.at(xmlKey);
	TextBox* startBox = new TextBox({ 525, entryTop + 12 }/*position*/, { 50, 30 }/*size*/);
	startBox->number = &data->positions[pairNum].startSize;
	boxes.push_back(startBox);
}

void PositionPair::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void PositionPair::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
}

void PositionPair::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
}

void PositionPair::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}