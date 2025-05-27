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
	Button addTransform;
	Button testTransforms;
	std::vector<TransformEntry> entries;
};

