#include "Button.h"

Button::Button(sf::Font& font, sf::Vector2f pos, std::string lab) :
	rect{{50,50}}
{
	rect.setPosition(pos);
	rect.setFillColor(sf::Color::Black);
	rect.setOutlineThickness(2.0f);
	rect.setOutlineColor(sf::Color::White);

	label = new sf::Text(font, lab);
	label->setPosition(pos);
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(*label);
}

void Button::Select()
{
	rect.setOutlineThickness(5.0f);
	rect.setOutlineColor(sf::Color::Red);
	label->setFillColor(sf::Color::Red);
}

void Button::Unselect()
{
	rect.setOutlineThickness(2.0f);
	rect.setOutlineColor(sf::Color::White);
	label->setFillColor(sf::Color::White);
}