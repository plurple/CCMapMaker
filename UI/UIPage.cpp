#include "UIPage.h"

UIPage::UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
	std::string tabLabel, sf::Vector2f buttonBoxSize) :
	tabButton(tabPos, tabSize, tabLabel),
	page{{592,842}},
	buttonBox{ buttonBoxSize }
{
	page.setPosition({ 1004, 154 });
	page.setFillColor(sf::Color(192, 192, 192, 0));
	page.setOutlineThickness(4.0f);
	page.setOutlineColor(sf::Color::White);

	buttonBox.setPosition({ 1008, 158 });
	buttonBox.setFillColor(sf::Color(192, 192, 192, 0));
	buttonBox.setOutlineThickness(4.0f);
	buttonBox.setOutlineColor(sf::Color::Cyan);
}

void UIPage::Draw(sf::RenderWindow& window, bool selected)
{
	tabButton.Draw(window);
	window.draw(page);
	if(selected) window.draw(buttonBox);
}

