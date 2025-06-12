#pragma once
#include <vector>
#include <memory>
#include "Button.h"
#include <SFML/Graphics.hpp>
#include "../XML/XMLData.h"
#include "../UserInput.h"

struct ContinentBox
{
	Button box;
	std::string* name;
	int xmlKey;

	ContinentBox() : box{ { 0,0 } } {};
};

class ContinentPanel
{
public:
	sf::RectangleShape panel;
	std::vector<std::shared_ptr<ContinentBox>> continents;
	Button closeButton;
	bool showPanel;

	ContinentPanel();
	void Draw(sf::RenderWindow& window);
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input);

	void AddContinent(int xmlKey, std::shared_ptr<Continent> continent, int num);
};

