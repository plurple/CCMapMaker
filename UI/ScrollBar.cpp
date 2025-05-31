#include "ScrollBar.h"

ScrollBar::ScrollBar(sf::View view, sf::Vector2f verticlePos,
	sf::Vector2f verticleSize, sf::Vector2f horizontalPos,
	sf::Vector2f horizontalSize, bool vert, bool horiz) :
	upArrow{ 15 }/*radius*/,
	downArrow{ 15 }/*radius*/,
	leftArrow{ 15 }/*radius*/,
	rightArrow{ 15 }/*radius*/,
	upButton({ verticlePos.x, verticlePos.y - 40 }, { 30, 30 }/*size*/, ""),
	downButton({ verticlePos.x, verticlePos.y + verticleSize.y + 10 }, { 30, 30 }/*size*/, ""),
	leftButton({ horizontalPos.x - 40, horizontalPos.y }, { 30, 30 }/*size*/, ""),
	rightButton({ horizontalPos.x + horizontalSize.x + 10, horizontalPos.y }, { 30, 30 }/*size*/, ""),
	verticle{vert},
	verticleBar{verticleSize},
	verticleTrack{ verticleSize },
	horizontal{ horiz },
	horizontalBar{ horizontalSize },
	horizontalTrack{ horizontalSize },
	scrollWindow{view}
{
	upArrow.setPointCount(3);
	upArrow.setPosition({ verticlePos.x, verticlePos.y-33});
	upArrow.setFillColor(sf::Color::White);

	downArrow.setPointCount(3);
	downArrow.setPosition({ verticlePos.x+30, verticlePos.y + verticleSize.y +37 });
	downArrow.setFillColor(sf::Color::White);
	downArrow.setRotation(sf::degrees(180));

	leftArrow.setPointCount(3);
	leftArrow.setPosition({ horizontalPos.x-35, horizontalPos.y });
	leftArrow.setFillColor(sf::Color::White);
	leftArrow.setRotation(sf::degrees(90));
	leftArrow.setScale({ 1.0f, -1.0f });

	rightArrow.setPointCount(3);
	rightArrow.setPosition({ horizontalPos.x + horizontalSize.x +37, horizontalPos.y });
	rightArrow.setFillColor(sf::Color::White);
	rightArrow.setRotation(sf::degrees(90));

	verticleBar.setPosition(verticlePos);
	verticleBar.setFillColor(sf::Color(120, 120, 120));

	verticleTrack.setPosition(verticlePos);
	verticleTrack.setFillColor(sf::Color(230, 230, 230));
	verticleTrack.setOutlineThickness(2.0f);
	verticleTrack.setOutlineColor(sf::Color::White);

	horizontalBar.setPosition(horizontalPos);
	horizontalBar.setFillColor(sf::Color(120, 120, 120));

	horizontalTrack.setPosition(horizontalPos);
	horizontalTrack.setFillColor(sf::Color(230, 230, 230));
	horizontalTrack.setOutlineThickness(2.0f);
	horizontalTrack.setOutlineColor(sf::Color::White);
}

void ScrollBar::Draw(sf::RenderWindow& window)
{

	if (verticle)
	{
		window.draw(verticleTrack);
		window.draw(verticleBar);
		upButton.Draw(window);
		downButton.Draw(window);
		window.draw(upArrow);
		window.draw(downArrow);
	}
	if(horizontal)
	{
		window.draw(horizontalTrack);
		window.draw(horizontalBar);
		leftButton.Draw(window);
		rightButton.Draw(window);
		window.draw(leftArrow);
		window.draw(rightArrow);
	}
}