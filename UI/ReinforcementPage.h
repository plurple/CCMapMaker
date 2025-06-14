#pragma once
#include "UIPage.h"

class ReinforcementEntry : public UIEntry
{
public:
	enum class LabelTypes
	{
		LowerLabel,
		UpperLabel,
		DivisorLabel,
		Explanation,
		NumLabels
	};
	enum class BoxTypes
	{
		LowerBox,
		UpperBox,
		DivisorBox,
		NumBoxes
	};
	~ReinforcementEntry() {};
	ReinforcementEntry(int insertedKey) : UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
	void Select() override;
	void Unselect(bool white = false) override;
};

class ReinforcementPage : public UIPage
{
public:
	std::shared_ptr<sf::Text> minLabel;
	TextBox minReinforcements;

	~ReinforcementPage() {};
	ReinforcementPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize, bool& continentPanel);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddReinforcement(XMLData& xmlData);
};

