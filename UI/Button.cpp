#include "Button.h"
#include "UI.h"

Button::Button(sf::Vector2f pos, sf::Vector2f dimensions,
	std::string lab, bool select) :
	rect{dimensions},
	selected{select}
{
	rect.setPosition(pos);
	rect.setFillColor(sf::Color::Black);

	label = new sf::Text(UI::font, lab);
	label->setPosition({ pos.x +5, pos.y-5 });

	selected ? Select() : Unselect();
}

void Button::Draw(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(*label);
}

void Button::Select()
{
	selected = true;
	if (xmlLink) *xmlLink = true;
	rect.setOutlineThickness(5.0f);
	rect.setOutlineColor(sf::Color::Red);
	label->setFillColor(sf::Color::Red);
}

void Button::Unselect()
{
	selected = false;
	if (xmlLink) *xmlLink = false;
	rect.setOutlineThickness(2.0f);
	rect.setOutlineColor(sf::Color::White);
	label->setFillColor(sf::Color::White);
}

void Button::Toggle()
{
	selected ? Unselect() : Select();
}

void Button::Move(sf::Vector2f offset)
{
	rect.move(offset);
	label->move(offset);
}

void Button::SetPosition(sf::Vector2f pos)
{
	rect.setPosition(pos);
	label->setPosition({ pos.x + 5, pos.y - 5 });
}

void Button::Hide(bool show)
{
	rect.setScale({ (float)show,(float)show });
	label->setScale({ (float)show, (float)show });
}