#include "PositionPage.h"
#include "UI.h"
#include "../XML/Position.h"

PositionPage::PositionPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
    maxBox({ 1470, 170 }, { 50, 30 })
{
    maxLabel = std::make_shared<sf::Text>(UI::font, "Max Positions:");
    maxLabel->setPosition({ 1270, 165 });

	addEntry.SetPosition({ 1070, 170 });
	addEntry.rect.setSize({ 185, 30 });
	addEntry.label->setString("Add Position");

	maxBox.number = std::shared_ptr<int>(&xmlData.maxPositions);
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
	std::shared_ptr<PositionEntry> entry = 
		std::make_shared<PositionEntry>(xmlData.AddPosition());
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void PositionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{580, 50} );
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Green);
	shapes.push_back(border);

	std::shared_ptr<Button> add = 
		std::make_shared<Button>(sf::Vector2f{ 220, entryTop + 12 }/*position*/,
		sf::Vector2f{ 200, 30 }/*size*/, "Add Position");
	buttons.push_back(add);

	std::shared_ptr<PositionPair> posPair =
		std::make_shared<PositionPair>(xmlKey, positionPairs.size());
	posPair->CreateEntry(xmlData, entryTop);
	positionPairs.push_back(posPair);
}

void PositionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	for (std::shared_ptr<UIEntry> entry : positionPairs)
	{
		entry->Draw(window);
	}
}

void PositionEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	std::shared_ptr<sf::Shape> borderBox = shapes[(int)ShapeTypes::Border];
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
	for (std::shared_ptr<UIEntry> entry : positionPairs)
	{
		entry->MouseClick(mousePos, mouseOnPage);
	}
}

void PositionEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
	for (std::shared_ptr<UIEntry> entry : positionPairs)
	{
		entry->Update(window, timePassed, input, showCursor);
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

void PositionPair::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::Text> territoryName = 
		std::make_shared<sf::Text>(UI::font, "Territory Name");
	territoryName->setPosition({ 30, entryTop + 8 });
	labels.push_back(territoryName);

	std::shared_ptr<sf::Text> startLabel =
		std::make_shared<sf::Text>(UI::font, "Start Size:");
	startLabel->setPosition({ 380, entryTop + 8 });
	labels.push_back(startLabel);

	std::shared_ptr<Position> data = xmlData.positions.at(xmlKey);
	std::shared_ptr<TextBox> startBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 525, entryTop + 12 }/*position*/, 
			sf::Vector2f{ 50, 30 }/*size*/);
	startBox->number = std::shared_ptr<int>(&data->positions[pairNum].startSize);
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