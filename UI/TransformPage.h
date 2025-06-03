#pragma once
#include "UIPage.h"

class TransformOption : public UIEntry
{
	enum class ShapeTypes
	{
		LeftArrow,
		RightArrow,
		NumShapes
	};
	enum class LabelTypes
	{
		OptionLabel,
		SelectedOption,
		NumLabels
	};
	enum class ButtonTypes
	{
		LeftButton,
		RightButton,
		NumButtons
	};
public:
	TransformOption() {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;
	void CreateEntry(XMLData& xmlData, float yCoord, float labelCoord, float leftCoord,
		float optionCoord, float rightCoord, std::string label);

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class ConditionEntry : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	enum class LabelTypes
	{
		IDLabel,
		ValueLabel,
		NumLabels
	};
	enum class BoxTypes
	{
		IDBox,
		ValueBox,
		NumBoxes
	};
	enum class EntryTypes
	{
		Type,
		Operator,
		Value,
		NumOptions
	};
public:
	ConditionEntry() {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class TransformEntry : public UIEntry
{
	enum class ShapeTypes
	{
		Border,
		NumShapes
	};
	enum class LabelTypes
	{
		AmountLabel,
		UpperLabel,
		LowerLabel,
		ConditionsLabel,
		NumLabels
	};
	enum class BoxTypes
	{
		AmountBox,
		UpperBox,
		LowerBox,
		NumBoxes
	};
	enum class ButtonTypes
	{
		Percentage,
		AddCondition,
		NumButtons
	};
	enum class EntryTypes
	{
		Type,
		ApplyTo,
		Inc,
		NumOptions
	};
public:
	std::vector<UIEntry*> conditions;

	TransformEntry() {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class TransformPage : public UIPage
{
public:
	Button testTransforms;

	TransformPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed, 
		UserInput input, bool showCursor) override;

	void AddTransform(XMLData& xmlData);
};

