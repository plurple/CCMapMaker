#include "ReinforcementPage.h"
#include "UI.h"

ReinforcementPage::ReinforcementPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	minReinforcements({ 1530, 170 }, { 50, 30 }, "3")
{
	minLabel = new sf::Text(UI::font, "Minium Troops:");
	minLabel->setPosition({ 1310, 165 });

	addEntry.SetPosition({ 1020, 170 });
	addEntry.rect.setSize({ 270, 30 });
	addEntry.label->setString("Add Reinforcement");
}

void ReinforcementPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		window.draw(*minLabel);
		minReinforcements.Draw(window);
	}
}

void ReinforcementPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(window, mousePos);
    if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
    {
		AddReinforcement();
    }
	minReinforcements.active = UI::CheckMouseInBounds(mousePos, minReinforcements.box);
}

void ReinforcementPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
    UserInput input, bool showCursor)
{
	UIPage::Update(window, timePassed, input, showCursor);

	minReinforcements.Update(window, timePassed, input, showCursor);
}

void ReinforcementPage::AddReinforcement()
{
	ReinforcementEntry* entry = new ReinforcementEntry{};
	UIPage::AddEntry(entry);
}

//-----------------------------------------------------------

void ReinforcementEntry::CreateEntry(float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ {580,155}/*size*/ };
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Yellow);
	shapes.push_back(border);

	sf::Text* lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 60, entryTop + 8 });
	labels.push_back(lowerLabel);

	sf::Text* upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 220, entryTop + 8 });
	labels.push_back(upperLabel);

	sf::Text* divisorLabel = new sf::Text(UI::font, "Divisor:");
	divisorLabel->setPosition({ 380, entryTop + 8 });
	labels.push_back(divisorLabel);

	sf::Text* explanation = new sf::Text(UI::font, "1 troop for every divisor regions up to \nmax of (upper-lower)/divisor=max troops \nin the range of lower-upper regions.");
	explanation->setPosition({ 50, entryTop + 48 });
	labels.push_back(explanation);

	TextBox* lowerBox = new TextBox({ 160, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "1");
	boxes.push_back(lowerBox);
	TextBox* upperBox = new TextBox({ 320, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "300");
	boxes.push_back(upperBox);
	TextBox* divisorBox = new TextBox({ 490, entryTop + 12 }/*position*/, { 50, 30 }/*size*/, "3");
	boxes.push_back(divisorBox);
}

void ReinforcementEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void ReinforcementEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	sf::Shape* borderBox = shapes[(int)ShapeTypes::Border];
	if (mouseOnPage && borderBox)
	{
		if (UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()))
		{
			selected = true;
			borderBox->setOutlineThickness(4.0f);
			borderBox->setOutlineColor({ 230, 200, 90 });
		}
		else
		{
			selected = false;
			borderBox->setOutlineThickness(2.0f);
			borderBox->setOutlineColor(sf::Color::Yellow);
		}
	}
}

void ReinforcementEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
}

void ReinforcementEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}