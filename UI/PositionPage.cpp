#include "PositionPage.h"
#include "UI.h"

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

void PositionPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	UserInput input, bool showCursor)
{
	UIPage::Update(window, timePassed, input, showCursor);
	//TODO make sure that you only care about numbers entered;
	maxBox.Update(window, timePassed, input, showCursor);
}

void PositionPage::AddPosition(XMLData& xmlData)
{
	PositionEntry* entry = new PositionEntry{};
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

	sf::Text* territoryName = new sf::Text(UI::font, "Territory Name");
	territoryName->setPosition({ 30, entryTop + 8 });
	labels.push_back(territoryName);

	sf::Text* startLabel = new sf::Text(UI::font, "Start Size:");
	startLabel->setPosition({ 380, entryTop + 8 });
	labels.push_back(startLabel);

	TextBox* startBox = new TextBox({ 525, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "3");
	boxes.push_back(startBox);
}

void PositionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
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
}

void PositionEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
}

void PositionEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}