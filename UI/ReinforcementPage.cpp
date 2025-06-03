#include "UI.h"
#include "ReinforcementPage.h"
#include "../XML/Reinforcement.h"

#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	std::unique_ptr<char[]> buf(new char[size]);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}


ReinforcementPage::ReinforcementPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	minReinforcements({ 1530, 170 }, { 50, 30 })
{
	minLabel = new sf::Text(UI::font, "Minium Troops:");
	minLabel->setPosition({ 1310, 165 });

	addEntry.SetPosition({ 1020, 170 });
	addEntry.rect.setSize({ 270, 30 });
	addEntry.label->setString("Add Reinforcement");

	minReinforcements.number = &xmlData.minReinforcements;
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

void ReinforcementPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(xmlData, window, mousePos);
    if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
    {
		AddReinforcement(xmlData);
    }
	minReinforcements.active = UI::CheckMouseInBounds(mousePos, minReinforcements.box);
}

void ReinforcementPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
    UserInput input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);

	minReinforcements.Update(window, timePassed, input, showCursor);
}

void ReinforcementPage::AddReinforcement(XMLData& xmlData)
{
	ReinforcementEntry* entry = new ReinforcementEntry{ xmlData.AddReinforcement() };
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void ReinforcementEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ {535,155}/*size*/ };
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Yellow);
	shapes.push_back(border);

	sf::Text* lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 35, entryTop + 8 });
	labels.push_back(lowerLabel);

	sf::Text* upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 195, entryTop + 8 });
	labels.push_back(upperLabel);

	sf::Text* divisorLabel = new sf::Text(UI::font, "Divisor:");
	divisorLabel->setPosition({ 355, entryTop + 8 });
	labels.push_back(divisorLabel);

	sf::Text* explanation = new sf::Text(UI::font, "1 troop for every %d regions up to \nmax of (%d-%d)/%d=%d troops \nin the range of %d-%d regions.");
	explanation->setPosition({ 55, entryTop + 48 });
	labels.push_back(explanation);

	Reinforcement* data = xmlData.reinforcements.at(xmlKey);
	TextBox* lowerBox = new TextBox({ 135, entryTop + 12 }/*position*/, { 50, 30 }/*size*/);
	lowerBox->number = &data->lower;
	boxes.push_back(lowerBox);
	TextBox* upperBox = new TextBox({ 295, entryTop + 12 }/*position*/, { 50, 30 }/*size*/);
	upperBox->number = &data->upper;
	boxes.push_back(upperBox);
	TextBox* divisorBox = new TextBox({ 465, entryTop + 12 }/*position*/, { 50, 30 }/*size*/);
	divisorBox->number = &data->divisor;
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

	std::string explanation = "1 troop for every %d regions up to \n a max of (%d-%d)/%d=%d troops \n in the range of %d-%d regions.";
	int lower = *boxes[(int)BoxTypes::LowerBox]->number;
	int upper = *boxes[(int)BoxTypes::UpperBox]->number;
	int divisor = *boxes[(int)BoxTypes::DivisorBox]->number;
	int max = (upper - lower+1) / divisor;
	labels[(int)LabelTypes::Explanation]->setString(string_format(explanation, divisor, upper, lower-1, divisor, max, lower, upper));

	MoveEntry({ 0, input.scroll });
}

void ReinforcementEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}