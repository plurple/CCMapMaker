#pragma once
#include "UIPage.h"

class TransformOptions
{
public:
	sf::Text* optionLabel;
	Button leftButton;
	sf::CircleShape leftArrow;
	sf::Text* selectedOption;
	Button rightButton;
	sf::CircleShape rightArrow;

	TransformOptions(float yCoord, float labelCoord, float leftCoord,
		float optionCoord, float rightCoord, std::string label);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		std::string keyPressed, bool backspace, bool enter,
		bool showCursor);

	void MoveOption(sf::Vector2f offset);
};

class TransformEntry
{
public:
	sf::RectangleShape borderBox;
	TransformOptions typeOptions;
	TransformOptions applyOptions;
	TransformOptions incOptions;
	sf::Text* amountLabel;
	TextBox amountBox;
	sf::Text* upperLabel;
	TextBox upperBox;
	sf::Text* lowerLabel;
	TextBox lowerBox;
	Button percentage;
	sf::Text* conditionsLabel;
	sf::RectangleShape conditionsBox;
	Button addCondition;
	TransformOptions conditionTypeOptions;
	sf::Text* idLabel;
	TextBox idBox; //this might want to be a territory picker sometimes
	TransformOptions operatorOptions;
	TransformOptions valueOptions;
	sf::Text* valueLabel;
	TextBox valueBox;

	TransformEntry(float entryTop);
	void Draw(sf::RenderWindow& window);
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage);
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		std::string keyPressed, bool backspace, bool enter,
		bool showCursor, float scrolled);

	void MoveEntry(sf::Vector2f offset);
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
	void MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		std::string keyPressed, bool backspace, bool enter, 
		bool showCursor, bool verticle, float scrolled) override;

	void AddTransform();
};

