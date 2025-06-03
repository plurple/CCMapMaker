#include "TextBox.h"
#include "UI.h"

TextBox::TextBox(sf::Vector2f pos, sf::Vector2f boxSize, std::string defaultText) :
	box{ boxSize },
	active{false},
	text{defaultText}
{
	box.setPosition(pos);
	box.setFillColor(sf::Color::Black);
	box.setOutlineThickness(2.0f);
	box.setOutlineColor(sf::Color::White);

	displayText = new sf::Text(UI::font, text);
	displayText->setPosition({ pos.x, pos.y-5});
}

void TextBox::Update(sf::RenderWindow& window, sf::Time timePassed, 
    UserInput input, bool showCursor)
{
    if (active)
    {
        text += input.keyPressed;
        if (input.backSpace && !text.empty())
            text.pop_back();
        if (input.enter)
            active = false;       
    }

    displayText->setString(text + (active && showCursor ? '|' : ' '));
}

void TextBox::Draw(sf::RenderWindow& window)
{
    window.draw(box);
    window.draw(*displayText);
}

void TextBox::Move(sf::Vector2f offset)
{
    box.move(offset);
    displayText->move(offset);
}

void TextBox::Hide(bool show)
{
    box.setScale({ (float)show,(float)show });
    displayText->setScale({ (float)show, (float)show });
}