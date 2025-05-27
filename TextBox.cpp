#include "TextBox.h"

TextBox::TextBox(sf::Vector2f pos, sf::Font& font) :
	position{pos},
	box{ {400, 32} },
	active{false},
	text{""}
{
	box.setPosition(position);
	box.setFillColor(sf::Color::Black);
	box.setOutlineThickness(2.0f);
	box.setOutlineColor(sf::Color::White);

	displayText = new sf::Text(font, text);
	displayText->setPosition({ position.x, position.y-5});
}

void TextBox::Update(sf::RenderWindow& window, sf::Time timePassed)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        if (active)
        {
            if (const auto* TextEntered = event->getIf<sf::Event::TextEntered>())
            {
                if (std::isprint(TextEntered->unicode))
                    text += TextEntered->unicode;
            }
            if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Backspace)
                {
                    if (!text.empty())
                        text.pop_back();
                }
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Enter)
                {
                    active = false;
                }
            }
        }
    }

    static sf::Time text_effect_time;
    static bool show_cursor;
    text_effect_time += timePassed;

    if (text_effect_time >= sf::seconds(0.5f))
    {
        show_cursor = !show_cursor;
        text_effect_time = sf::Time::Zero;
    }
    if (!active) show_cursor = false;

    displayText->setString(text + (show_cursor ? '|' : ' '));
}

void TextBox::Draw(sf::RenderWindow& window)
{
    window.draw(box);
    window.draw(*displayText);
}