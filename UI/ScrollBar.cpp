#include "ScrollBar.h"

ScrollBar::ScrollBar(sf::View view, sf::Vector2f position, sf::Vector2f size, 
	bool vert) :
	upButton({ position.x, position.y }, { 30, 30 }, ""),
	downButton({ position.x, position.y }, { 30, 30 }, ""),
	leftButton({ position.x, position.y }, { 30, 30 }, ""),
	rightButton({ position.x, position.y }, { 30, 30 }, ""),
	verticle{vert},
	bar{size},
	track{size},
	scrollWindow{view}
{
	upArrow.setPointCount(3);
	upArrow.setPosition({ position.x, position.y });
	upArrow.setFillColor(sf::Color::White);
	upArrow.setRotation(sf::degrees(90));

	downArrow.setPointCount(3);
	downArrow.setPosition({ position.x, position.y });
	downArrow.setFillColor(sf::Color::White);
	downArrow.setRotation(sf::degrees(180));

	leftArrow.setPointCount(3);
	leftArrow.setPosition({ position.x, position.y });
	leftArrow.setFillColor(sf::Color::White);
	leftArrow.setRotation(sf::degrees(90));
	leftArrow.setScale({ 1.0f, -1.0f });

	rightArrow.setPointCount(3);
	rightArrow.setPosition({ position.x, position.y });
	rightArrow.setFillColor(sf::Color::White);
	rightArrow.setRotation(sf::degrees(90));

	bar.setPosition(position);
	bar.setFillColor(sf::Color(192, 192, 192, 0));
	bar.setOutlineThickness(4.0f);
	bar.setOutlineColor(sf::Color::White);

	track.setPosition(position);
	track.setFillColor(sf::Color(192, 192, 192, 0));
	track.setOutlineThickness(4.0f);
	track.setOutlineColor(sf::Color::White);
}

void ScrollBar::Draw(sf::RenderWindow& window)
{
	if (verticle)
	{
		upButton.Draw(window);
		window.draw(upArrow);
		downButton.Draw(window);
		window.draw(downArrow);
	}
	else
	{
		leftButton.Draw(window);
		window.draw(leftArrow);
		rightButton.Draw(window);
		window.draw(rightArrow);
	}
	window.draw(bar);
	window.draw(track);
}