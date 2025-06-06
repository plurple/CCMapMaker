#pragma once
#include "UIPage.h"

class ReinforcementEntry : public UIEntry
{
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
public:
	~ReinforcementEntry() {};
	ReinforcementEntry(int insertedKey) : UIEntry{ insertedKey } {};
	void CreateEntry(XMLData& xmlData, float entryTop) override;

	void Draw(sf::RenderWindow& window) override;
	void MouseClick(sf::Vector2i mousePos, bool mouseOnPage) override;
	void Update(sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor) override;

	void MoveEntry(sf::Vector2f offset) override;
};

class ReinforcementPage : public UIPage
{
public:
	std::shared_ptr<sf::Text> minLabel;
	TextBox minReinforcements;

	~ReinforcementPage() {};
	ReinforcementPage(XMLData& xmlData, sf::Vector2f tabPos, 
		sf::Vector2f tabSize, std::string tabLabel, 
		sf::Vector2f buttonBoxSize);

	void Draw(sf::RenderWindow& window, bool selected) override;
	void MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
		sf::Vector2i mousePos, Maps& maps) override;
	void Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
		UserInput input, bool showCursor, UIPageType pageType) override;

	void AddReinforcement(XMLData& xmlData);
};

