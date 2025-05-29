#include "TextBox.h"
#include "UI.h"

TextBox::TextBox(sf::Vector2f pos, sf::Vector2f boxSize) :
	box{ boxSize },
	active{false},
	text{""}
{
	box.setPosition(pos);
	box.setFillColor(sf::Color::Black);
	box.setOutlineThickness(2.0f);
	box.setOutlineColor(sf::Color::White);

	displayText = new sf::Text(UI::font, text);
	displayText->setPosition({ pos.x, pos.y-5});
}

void TextBox::Update(sf::RenderWindow& window, sf::Time timePassed, 
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
    if (active)
    {
        text += keyPressed;
        if (backspace && !text.empty())
            text.pop_back();
        if (enter)
            active = false;       
    }

    displayText->setString(text + (active && showCursor ? '|' : ' '));
}

void TextBox::Draw(sf::RenderWindow& window)
{
    window.draw(box);
    window.draw(*displayText);
}