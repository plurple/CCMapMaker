#include "UIPage.h"

UIPage::UIPage(sf::Font& font, sf::Vector2f pos, std::string lab) :
	tabButton(font, pos, lab),
	page{{598,798}}
{
	page.setPosition({ 1000, 200 });
	page.setFillColor(sf::Color(192, 192, 192, 0));
	page.setOutlineThickness(2.0f);
	page.setOutlineColor(sf::Color::White);
}

void UIPage::Draw(sf::RenderWindow& window)
{
	tabButton.Draw(window);
	window.draw(page);
}

