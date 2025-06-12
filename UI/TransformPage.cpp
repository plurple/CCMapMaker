#include "TransformPage.h"
#include "UI.h"
#include "../XML/Transform.h"

TransformPage::TransformPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize,
	bool& continentPanel) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize, continentPanel),
	testTransforms({ 1321, 170 }, { 230, 30 }, "Test Transforms")
{
	addEntry.SetPosition({ 1051, 170 });
	addEntry.rect.setSize({ 210, 30 });
	addEntry.label->setString("Add Transform");
}

void TransformPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		testTransforms.Draw(window);
	}	
}

void TransformPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
    if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
    {
		AddTransform(xmlData);
    }
	if (UI::CheckMouseInBounds(mousePos, testTransforms.rect))
	{
		//TODO add a transform test function
	}	
}

void TransformPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
    UserInput input, bool showCursor, UIPageType pageType) 
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
}

void TransformPage::AddTransform(XMLData& xmlData)
{
	std::shared_ptr<TransformEntry> entry = 
		std::make_shared<TransformEntry>(xmlData.AddTransform());
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void TransformEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	baseColor = sf::Color::Blue;
	selectedColor = sf::Color{ 60, 120, 240 };

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{ 580,288 } );/*size*/
	border->setPosition({ 10, entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Blue);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> amountLabel = 
		std::make_shared<sf::Text>(UI::font, "Amount:");
	amountLabel->setPosition({ 20, entryTop + 92 });
	labels.push_back(amountLabel);
	
	std::shared_ptr<sf::Text> upperLabel = 
		std::make_shared<sf::Text>(UI::font, "Upper:");
	upperLabel->setPosition({ 220, entryTop + 92 });
	labels.push_back(upperLabel);
	
	std::shared_ptr<sf::Text> lowerLabel = 
		std::make_shared<sf::Text>(UI::font, "Lower:");
	lowerLabel->setPosition({ 380, entryTop + 92 });
	labels.push_back(lowerLabel);
	
	std::shared_ptr<sf::Text> conditionsLabel = 
		std::make_shared<sf::Text>(UI::font, "Conditions:");
	conditionsLabel->setPosition({ 20, entryTop + 128 });
	labels.push_back(conditionsLabel);

	std::shared_ptr<Transform> data = xmlData.transforms.at(xmlKey);
	std::shared_ptr<TextBox> amountBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 140, entryTop + 96 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	amountBox->number = &data->amount;
	boxes.push_back(amountBox);
	
	std::shared_ptr<TextBox> upperBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 320, entryTop + 96 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	upperBox->number = &data->upper;
	boxes.push_back(upperBox);
	
	std::shared_ptr<TextBox> lowerBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 480, entryTop + 96 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	lowerBox->number = &data->lower;
	boxes.push_back(lowerBox);

	std::shared_ptr<Button> percentage = 
		std::make_shared<Button>(sf::Vector2f{ 540, entryTop + 96 }/*position*/,
			sf::Vector2f{ 35, 30 }/*size*/, "%");
	percentage->xmlLink = &data->percentage;
	buttons.push_back(percentage);

	std::shared_ptr<Button> addCondition = 
		std::make_shared<Button>(sf::Vector2f{ 200, entryTop + 132 }/*position*/,
			sf::Vector2f{ 205, 30 }/*size*/, "Add Condition");
	buttons.push_back(addCondition);

	std::shared_ptr<TransformOption> typeOptions = 
		std::make_shared<TransformOption>(xmlKey);
	typeOptions->CreateEntry(xmlData, entryTop + 12, 20, 100, 140, 260, "Type:");
	entries.push_back(typeOptions);
	
	std::shared_ptr<TransformOption> applyOptions = 
		std::make_shared<TransformOption>(xmlKey);
	applyOptions->CreateEntry(xmlData, entryTop + 46, 20, 150, 190, 310, "Apply To:");
	entries.push_back(applyOptions);
	
	std::shared_ptr<TransformOption> incOptions = 
		std::make_shared<TransformOption>(xmlKey);
	incOptions->CreateEntry(xmlData, entryTop + 12, 320, 380, 420, 540, "Inc:");
	entries.push_back(incOptions);
	
	std::shared_ptr<ConditionEntry> condition = 
		std::make_shared<ConditionEntry>(xmlKey, conditions.size());
	condition->CreateEntry(xmlData, entryTop);
	conditions.push_back(condition);

	Select();
}

void TransformEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		entry->Draw(window);
	}
}

void TransformEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);

	std::shared_ptr<Button> percentage = buttons[(int)ButtonTypes::Percentage];
	if (mouseOnPage && percentage && UI::CheckMouseInBounds(mousePos, percentage->rect))
	{
		percentage->Toggle();
	}
	std::shared_ptr<Button> addCondition = buttons[(int)ButtonTypes::AddCondition];
	if (mouseOnPage && addCondition && UI::CheckMouseInBounds(mousePos, addCondition->rect))
	{
		//todo add a conditions stuff
	}
	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		entry->MouseClick(xmlData, mousePos, mouseOnPage, select);
	}
}

void TransformEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });

	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		entry->Update(xmlData, window, timePassed, input, showCursor);
	}
}

void TransformEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		entry->MoveEntry(offset);
	}
}

//-----------------------------------------------------------

void ConditionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>(sf::Vector2f{ 572,116 } );/*size*/
	border->setPosition({ 14, entryTop + 168});
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Cyan);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> idLabel = 
		std::make_shared<sf::Text>(UI::font, "ID:");
	idLabel->setPosition({ 310, entryTop + 172 });
	labels.push_back(idLabel);

	std::shared_ptr<sf::Text> valueLabel = 
		std::make_shared<sf::Text>(UI::font, "Value:");
	valueLabel->setPosition({ 400, entryTop + 208 });
	labels.push_back(valueLabel);

	std::shared_ptr<Transform> data = xmlData.transforms.at(xmlKey);
	std::shared_ptr<TextBox> idBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 370, entryTop + 176 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	idBox->number = &data->conditions[conditionNum].index;
	boxes.push_back(idBox);

	std::shared_ptr<TextBox> valueBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 500, entryTop + 208 }/*position*/, 
			sf::Vector2f{ 50, 30 }/*size*/);
	valueBox->number = &data->conditions[conditionNum].values[0];
	boxes.push_back(valueBox);

	std::shared_ptr<TransformOption> typeOptions =
		std::make_shared<TransformOption>(xmlKey);
	typeOptions->CreateEntry(xmlData, entryTop + 172, 20, 100, 140, 260, "Type:");
	entries.push_back(typeOptions);

	std::shared_ptr<TransformOption> operatorOptions =
		std::make_shared<TransformOption>(xmlKey);
	operatorOptions->CreateEntry(xmlData, entryTop + 208, 20, 150, 190, 310, "Operator:");
	entries.push_back(operatorOptions);

	std::shared_ptr<TransformOption> valueOptions = 
		std::make_shared< TransformOption>(xmlKey);
	valueOptions->CreateEntry(xmlData, entryTop + 244, 20, 120, 160, 280, "Value:");
	entries.push_back(valueOptions);
}

void ConditionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void ConditionEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);
}

void ConditionEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void ConditionEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}

//-----------------------------------------------------------

void TransformOption::CreateEntry(XMLData& xmlData, float entryTop)
{}

void TransformOption::CreateEntry(XMLData& xmlData, float yCoord, 
	float labelCoord, float leftCoord, float optionCoord, 
	float rightCoord, std::string label)
{
	std::shared_ptr<sf::CircleShape> leftArrow = 
		std::make_shared<sf::CircleShape>( (15) /*radius*/ );
	leftArrow->setPointCount(3);
	leftArrow->setPosition({ leftCoord + 4, yCoord + 4 });
	leftArrow->setFillColor(sf::Color::White);
	leftArrow->setRotation(sf::degrees(90));
	leftArrow->setScale({ 1.0f, -1.0f });
	shapes.push_back(leftArrow);

	std::shared_ptr<sf::CircleShape> rightArrow = 
		std::make_shared<sf::CircleShape>( (15)/*radius*/ );
	rightArrow->setPointCount(3);
	rightArrow->setPosition({ rightCoord + 26, yCoord + 4 });
	rightArrow->setFillColor(sf::Color::White);
	rightArrow->setRotation(sf::degrees(90));
	shapes.push_back(rightArrow);

	std::shared_ptr<sf::Text> optionLabel = 
		std::make_shared<sf::Text>(UI::font, label);
	optionLabel->setPosition({ labelCoord, yCoord });
	labels.push_back(optionLabel);
	
	std::shared_ptr<sf::Text> selectedOption = 
		std::make_shared<sf::Text>(UI::font, "selected");
	selectedOption->setPosition({ optionCoord, yCoord });
	labels.push_back(selectedOption);

	std::shared_ptr<Button> leftButton  = 
		std::make_shared<Button>(sf::Vector2f{ leftCoord, yCoord + 4 }/*position*/, 
			sf::Vector2f{ 30, 30 }/*size*/);
	buttons.push_back(leftButton);

	std::shared_ptr<Button> rightButton = 
		std::make_shared<Button>(sf::Vector2f{ rightCoord, yCoord + 4 }/*position*/, 
			sf::Vector2f{ 30, 30 }/*size*/);
	buttons.push_back(rightButton);
}

void TransformOption::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void TransformOption::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);

	std::shared_ptr<Button> leftButton = buttons[(int)ButtonTypes::LeftButton];
	if (UI::CheckMouseInBounds(mousePos, leftButton->rect))
	{
		//todo swap the opttion left
	}
	std::shared_ptr<Button> rightButton = buttons[(int)ButtonTypes::RightButton];
	if (UI::CheckMouseInBounds(mousePos, rightButton->rect))
	{
		//todo swap the opttion right
	}
}

void TransformOption::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void TransformOption::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}
