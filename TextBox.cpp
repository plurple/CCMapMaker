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
