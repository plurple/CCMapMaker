#include "UIPage.h"

UIPage::UIPage(sf::Font& font, sf::Vector2f pos, std::string lab) :
	tab{{50,50}}
{
	tab.setPosition(pos);
	tab.setFillColor(sf::Color::Black);
	tab.setOutlineThickness(2.0f);
	tab.setOutlineColor(sf::Color::White);

	label = new sf::Text(font, lab);
	label->setPosition(pos);
}

void UIPage::Draw(sf::RenderWindow& window)
{
	window.draw(tab);
	window.draw(*label);
}

void UIPage::SelectTab()
{
	tab.setOutlineThickness(5.0f);
	tab.setOutlineColor(sf::Color::Red);
	label->setFillColor(sf::Color::Red);
}

void UIPage::UnselectTab()
{
	tab.setOutlineThickness(2.0f);
	tab.setOutlineColor(sf::Color::White);
	label->setFillColor(sf::Color::White);
}