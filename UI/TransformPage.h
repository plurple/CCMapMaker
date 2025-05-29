#pragma once
#include "UIPage.h"

class TransformOptions
{
	sf::Text* label;
	Button left;
	sf::CircleShape leftArrow;
	sf::Text* option;
	Button Right;
	sf::CircleShape rightArrow;
};

class TransformEntry
{
	sf::RectangleShape borderBox;
	TransformOptions typeOptions;
	TransformOptions applyOptions;
	Button addInc;
	TransformOptions incOptions;
	Button addAmount;
	sf::Text* amountLabel;
	TextBox amountBox;
	Button addRange;
	sf::Text* upperLabel;
	TextBox upperBox;
	sf::Text* lowerLabel;
	TextBox lowerBox;
	Button percentage;
	Button addCondition;
	TransformOptions conditionTypeOptions;
	sf::Text* idLabel;
	TextBox idBox; //this might want to be a territory picker
	TransformOptions operatorOptions;
	TransformOptions valueOptions;
	sf::Text* valueLabel;
	TextBox valueBox;
};

class TransformPage : public UIPage
{
public:
	Button addTransform;
	Button testTransforms;
	std::vector<TransformEntry> entries;
	TransformPage(sf::Vector2f tabPos, sf::Vector2f tabSize,
		std::string tabLabel, sf::Vector2f buttonBoxSize);
	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor) override;

};

