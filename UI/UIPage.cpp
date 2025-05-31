#include "UIPage.h"

UIPage::UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
	std::string tabLabel, sf::Vector2f buttonBoxSize) :
	tabButton(tabPos, tabSize, tabLabel),
	page{{592,900-158-8-buttonBoxSize.y}},
	buttonBox{ buttonBoxSize },
	pageTop{158+4+4+buttonBoxSize.y},
	scrollBar(sf::View{ page.getGlobalBounds() }, { 555, 50 }/*position*/, { 30, page.getSize().y - 100 }/*size*/)
{
	page.setPosition({ 0, 0 });
	page.setFillColor(sf::Color(192, 192, 192, 0));
	page.setOutlineThickness(4.0f);
	page.setOutlineColor(sf::Color::White);

	scrollBar.scrollWindow = sf::View{ page.getGlobalBounds() };
	float bob = (page.getSize().y + 8.0f) / 900.0f;
	float bob2 = (page.getSize().x + 8.0f) / 1600.0f;
	scrollBar.scrollWindow.setViewport(sf::FloatRect({ 1.0f - bob2, 1.0f-bob }, { bob2, bob }));

	buttonBox.setPosition({ 1004, 154 });
	buttonBox.setFillColor(sf::Color(192, 192, 192, 0));
	buttonBox.setOutlineThickness(4.0f);
	buttonBox.setOutlineColor(sf::Color::Cyan);
}

void UIPage::Draw(sf::RenderWindow& window, bool selected)
{
	tabButton.Draw(window);
	if(selected) window.draw(buttonBox);
}

