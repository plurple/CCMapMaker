#include "ObjectivePage.h"
#include "UI.h"
#include "../XML/Objective.h"

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

void ObjectivePage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
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
	std::shared_ptr<ObjectiveEntry> entry = 
		std::make_shared<ObjectiveEntry>( isObjective,
		isObjective ? xmlData.AddObjective() : xmlData.AddRequirement() );
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void ObjectiveEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	baseColor = isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta;
	selectedColor = isObjective ? sf::Color{ 250, 170, 180 } : sf::Color{ 175, 0, 175 };

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{580, 165} );
	border->setPosition({ 10,entryTop });
	border->setFillColor({ sf::Color::Transparent });
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(isObjective ? sf::Color(230, 100, 110) : sf::Color::Magenta);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> nameLabel = 
		std::make_shared<sf::Text>(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);

	std::shared_ptr<sf::Text> territoryLabel = 
		std::make_shared<sf::Text>(UI::font, "Territories:");
	territoryLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(territoryLabel);

	std::shared_ptr<sf::Text> territories = 
		std::make_shared<sf::Text>(UI::font, "Territory");
	territories->setPosition({ 180, entryTop + 46 });
	labels.push_back(territories);
	
	std::shared_ptr<sf::Text> continentLabel = 
		std::make_shared<sf::Text>(UI::font, "Continents:");
	continentLabel->setPosition({ 20, entryTop + 84 });
	labels.push_back(continentLabel);
	
	std::shared_ptr<sf::Text> continents = 
		std::make_shared<sf::Text>(UI::font, "Continent");
	continents->setPosition({ 180, entryTop + 84 });
	labels.push_back(continents);
	
	std::shared_ptr<sf::Text> requiredLabel = 
		std::make_shared<sf::Text>(UI::font, "Num Required:");
	requiredLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(requiredLabel);

	std::shared_ptr<Objective> data = isObjective ? xmlData.objectives.at(xmlKey) :
		xmlData.requirements.at(xmlKey);
	std::shared_ptr<TextBox> nameBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 120, entryTop + 12 }/*position*/,
		sf::Vector2f{ 450, 30 }/*size*/);
	nameBox->text = &data->name;
	boxes.push_back(nameBox);

	std::shared_ptr<TextBox> numRequiredBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 235, entryTop + 124 }/*position*/,
		sf::Vector2f{ 50, 30 }/*size*/);
	numRequiredBox->number = &data->numRequired;
	boxes.push_back(numRequiredBox);

	Select();
}

void ObjectiveEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void ObjectiveEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(mousePos, mouseOnPage, select);
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