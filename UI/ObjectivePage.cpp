#include "ObjectivePage.h"
#include "UI.h"

ObjectivePage::ObjectivePage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
    isObjective{ true }
{
    isObjective = tabLabel != "Requirements";
	addEntry.SetPosition({ 1051, 170 });
	addEntry.rect.setSize(isObjective ? sf::Vector2f{ 200, 30 } : sf::Vector2f{245, 30});
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

void ObjectivePage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(xmlData, window, mousePos);
    if (UI::CheckMouseInBounds(mousePos, showContinents.rect))
    {
        showContinents.Toggle();
        //TODO show continents stuff
        //TODO unselect show continent on tab change
    }
    if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
    {
		AddObjective(xmlData);
    }
}

void ObjectivePage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
    UserInput input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
}

void ObjectivePage::AddObjective(XMLData& xmlData)
{
	ObjectiveEntry* entry = new ObjectiveEntry{isObjective, 0};
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void ObjectiveEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ {580, 165} };
	border->setPosition({ 10,entryTop });
	border->setFillColor({ 192, 192, 192, 0 });
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta);
	shapes.push_back(border);

	sf::Text* nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);

	sf::Text* territoryLabel = new sf::Text(UI::font, "Territories:");
	territoryLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(territoryLabel);

	sf::Text* territories = new sf::Text(UI::font, "Territory");
	territories->setPosition({ 180, entryTop + 46 });
	labels.push_back(territories);
	
	sf::Text* continentLabel = new sf::Text(UI::font, "Continents:");
	continentLabel->setPosition({ 20, entryTop + 84 });
	labels.push_back(continentLabel);
	
	sf::Text* continents = new sf::Text(UI::font, "Continent");
	continents->setPosition({ 180, entryTop + 84 });
	labels.push_back(continents);
	
	sf::Text* requiredLabel = new sf::Text(UI::font, "Num Required:");
	requiredLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(requiredLabel);

	TextBox* nameBox = new TextBox({ 120, entryTop + 12 }/*position*/, 
		{ 450, 30 }/*size*/, new std::string("Obective Name"));
	boxes.push_back(nameBox);

	TextBox* numRequiredBox = new TextBox({ 235, entryTop + 124 }/*position*/, 
		{ 50, 30 }/*size*/, new std::string(isObjective ? "all" : "1"));
	boxes.push_back(numRequiredBox);
}

void ObjectiveEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void ObjectiveEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);

	sf::Shape* borderBox = shapes[(int)ShapeTypes::Border];
	if (mouseOnPage && borderBox)
	{
		if (UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()))
		{
			selected = true;
			borderBox->setOutlineThickness(4.0f);
			borderBox->setOutlineColor(isObjective ? sf::Color{ 250, 170, 180 } : sf::Color{ 175, 0, 175 });
			//TODO be able to pick a territory from the map
		}
		else
		{
			selected = false;
			borderBox->setOutlineThickness(2.0f);
			borderBox->setOutlineColor(isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta);
		}
	}
}

void ObjectiveEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll});
}

void ObjectiveEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}