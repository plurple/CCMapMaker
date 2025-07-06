#pragma once
#include "UIPage.h"

class TransformOption : public UIEntry
{
public:
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
	TransformOptionType optionType;
	int selectedOption;
	bool skipAll;

	~TransformOption() {};
	TransformOption(int insertedKey) : UIEntry{ insertedKey }, skipAll{ false }, selectedOption{ 0 }, optionType{ TransformOptionType::ConditionType } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;
	void CreateEntry(XMLData& xmlData, float yCoord, float labelCoord, float leftCoord,
		float optionCoord, float rightCoord, std::string label);

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage,
		bool& select, bool mapClicked) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void Hide(bool show = false);
};

class ConditionEntry : public UIEntry
{
public:
	enum class LabelTypes
	{
		IDLabel,
		ValueLabel,
		NumLabels
	};
	enum class BoxTypes
	{
		ValueBox,
		NumBoxes
	};
	enum class ButtonTypes
	{
		AddButton,
		RemoveButton,
		NumButtons
	};
	enum class EntryTypes
	{
		Type,
		Operator,
		Value,
		NumOptions
	};

	LinkedData territoryID;
	std::vector<LinkedData> territories;
	int conditionNum;
	sf::Vector2f armyBoxPos;
	sf::Vector2f roundBoxPos;
	sf::Vector2f territoriesPos;
	sf::Vector2f territoryIDPos;

	~ConditionEntry() {};
	ConditionEntry(int insertedKey, int condition) : 
		UIEntry{ insertedKey }, conditionNum{ condition } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
		bool& select, bool mapClicked) override;
	bool MapClick(UI& ui, XMLData& xmlData, Maps& maps,
		sf::Vector2i mousePos, int& boxIndex);
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void SwapConditionType(int conditionType);
	void BorderBoxSize(int conditionType);
	void AddTerritoryID(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> border,
		int boxIndex, int otherXMLKey);
	void RemoveTerritoryID(XMLData& xmlData);
};

class TransformEntry : public UIEntry
{
public:
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
		RemoveCondition,
		NumButtons
	};
	enum class EntryTypes
	{
		Type,
		ApplyTo,
		Inc,
		NumOptions
	};
	std::vector<std::shared_ptr<UIEntry>> conditions;
	sf::Vector2f conditionPos;
	int selectedCondition;
	float conditionsHeight;

	~TransformEntry() {};
	TransformEntry(int insertedKey) : UIEntry{ insertedKey }, selectedCondition{ -1 }, conditionsHeight{ 0.0f } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
		bool& select, bool mapClicked) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void Select() override;
	void Unselect(bool white = false) override;
	void AddCondition(XMLData& xmlData);
	void RemoveCondition(XMLData& xmlData);
	void SwapCondition(int previous, int future);
	void BorderBoxSize() override;
	void PositionConditions();
};

class TransformPage : public UIPage
{
public:
	Button testTransforms;

	~TransformPage() {};
	TransformPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize, bool& continentPanel);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window,
		sf::Vector2i mousePos, Maps& maps) override;
	bool MapClick(UI& ui, XMLData& xmlData, Maps& maps,
		sf::Vector2i mousePos, int& boxIndex) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput& input, bool showCursor, UIPageType pageType) override;

	void AddTransform(XMLData& xmlData);
};

