#pragma once
#include <vector>
#include <memory>
#include "Button.h"
#include <SFML/Graphics.hpp>
#include "../XML/XMLData.h"
#include "../UserInput.h"

class ContinentPanel
{
public:
	sf::RectangleShape panel;
	std::vector<std::shared_ptr<Button>> continents;
	Button closeButton;
	bool showPanel;

	ContinentPanel();
	void Draw(sf::RenderWindow& window);
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input);
};

