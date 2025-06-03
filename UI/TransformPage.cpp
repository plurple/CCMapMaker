#include "TransformPage.h"
#include "UI.h"

TransformPage::TransformPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
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

void TransformPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(xmlData, window, mousePos);
    if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
    {
		AddTransform(xmlData);
    }
	if (UI::CheckMouseInBounds(mousePos, testTransforms.rect))
	{
		//TODO add a transform test function
	}	
}

void TransformPage::Update(sf::RenderWindow& window, sf::Time timePassed,
    UserInput input, bool showCursor) 
{
	UIPage::Update(window, timePassed, input, showCursor);
}

void TransformPage::AddTransform(XMLData& xmlData)
{
	TransformEntry* entry = new TransformEntry{};
	UIPage::AddEntry(xmlData, entry);
}

//-----------------------------------------------------------

void TransformEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ { 580,288 } };/*size*/
	border->setPosition({ 10, entryTop });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Blue);
	shapes.push_back(border);

	sf::Text* amountLabel = new sf::Text(UI::font, "Amount:");
	amountLabel->setPosition({ 20, entryTop + 92 });
	labels.push_back(amountLabel);
	
	sf::Text* upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 220, entryTop + 92 });
	labels.push_back(upperLabel);
	
	sf::Text* lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 380, entryTop + 92 });
	labels.push_back(lowerLabel);
	
	sf::Text* conditionsLabel = new sf::Text(UI::font, "Conditions:");
	conditionsLabel->setPosition({ 20, entryTop + 128 });
	labels.push_back(conditionsLabel);

	TextBox* amountBox = new TextBox({ 140, entryTop + 96 }/*position*/, { 70, 30 }/*size*/, "");
	boxes.push_back(amountBox);
	
	TextBox* upperBox = new TextBox({ 320, entryTop + 96 }/*position*/, { 50, 30 }/*size*/, "");
	boxes.push_back(upperBox);
	
	TextBox* lowerBox = new TextBox({ 480, entryTop + 96 }/*position*/, { 50, 30 }/*size*/, "");
	boxes.push_back(lowerBox);

	Button* percentage = new Button({ 540, entryTop + 96 }/*position*/, { 35, 30 }/*size*/, "%");
	buttons.push_back(percentage);

	Button* addCondition = new Button({ 200, entryTop + 132 }/*position*/, { 205, 30 }/*size*/, "Add Condition");
	buttons.push_back(addCondition);

	TransformOption* typeOptions = new TransformOption();
	typeOptions->CreateEntry(xmlData, entryTop + 12, 20, 100, 140, 260, "Type:");
	entries.push_back(typeOptions);
	
	TransformOption* applyOptions = new TransformOption();
	applyOptions->CreateEntry(xmlData, entryTop + 46, 20, 150, 190, 310, "Apply To:");
	entries.push_back(applyOptions);
	
	TransformOption* incOptions = new TransformOption();
	incOptions->CreateEntry(xmlData, entryTop + 12, 320, 380, 420, 540, "Inc:");
	entries.push_back(incOptions);
	
	ConditionEntry* condition = new ConditionEntry();
	condition->CreateEntry(xmlData, entryTop);
	conditions.push_back(condition);
}

void TransformEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	for (UIEntry* entry : conditions)
	{
		entry->Draw(window);
	}
}

void TransformEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	sf::Shape* borderBox = shapes[(int)ShapeTypes::Border];
	if (mouseOnPage && borderBox)
	{
		if (UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()))
		{
			selected = true;
			borderBox->setOutlineThickness(4.0f);
			borderBox->setOutlineColor({ 60, 120, 240 });
			//TODO be able to pick a territory from the map
		}
		else
		{
			selected = false;
			borderBox->setOutlineThickness(2.0f);
			borderBox->setOutlineColor(sf::Color::Blue);
		}
	}
	Button* percentage = buttons[(int)ButtonTypes::Percentage];
	if (mouseOnPage && percentage && UI::CheckMouseInBounds(mousePos, percentage->rect))
	{
		percentage->Toggle();
	}
	Button* addCondition = buttons[(int)ButtonTypes::AddCondition];
	if (mouseOnPage && addCondition && UI::CheckMouseInBounds(mousePos, addCondition->rect))
	{
		//todo add a conditions stuff
	}
	for (UIEntry* entry : conditions)
	{
		entry->MouseClick(mousePos, mouseOnPage);
	}
}

void TransformEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });

	for (UIEntry* entry : conditions)
	{
		entry->Update(window, timePassed, input, showCursor);
	}
}

void TransformEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
	for (UIEntry* entry : conditions)
	{
		entry->MoveEntry(offset);
	}
}

//-----------------------------------------------------------

void ConditionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ { 572,116 } };/*size*/
	border->setPosition({ 14, entryTop + 168});
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Cyan);
	shapes.push_back(border);

	sf::Text* idLabel = new sf::Text(UI::font, "ID:");
	idLabel->setPosition({ 310, entryTop + 172 });
	labels.push_back(idLabel);

	sf::Text* valueLabel = new sf::Text(UI::font, "Value:");
	valueLabel->setPosition({ 400, entryTop + 208 });
	labels.push_back(valueLabel);

	TextBox* idBox = new TextBox({ 370, entryTop + 176 }/*position*/, { 50, 30 }/*size*/, "");
	boxes.push_back(idBox);

	TextBox* valueBox = new TextBox({ 500, entryTop + 208 }/*position*/, { 50, 30 }/*size*/, "");
	boxes.push_back(valueBox);

	TransformOption* typeOptions = new TransformOption();
	typeOptions->CreateEntry(xmlData, entryTop + 172, 20, 100, 140, 260, "Type:");
	entries.push_back(typeOptions);

	TransformOption* operatorOptions = new TransformOption();
	operatorOptions->CreateEntry(xmlData, entryTop + 208, 20, 150, 190, 310, "Operator:");
	entries.push_back(operatorOptions);

	TransformOption* valueOptions = new TransformOption();
	valueOptions->CreateEntry(xmlData, entryTop + 244, 20, 120, 160, 280, "Value:");
	entries.push_back(valueOptions);
}

void ConditionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void ConditionEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
}

void ConditionEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
}

void ConditionEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}

//-----------------------------------------------------------

void TransformOption::CreateEntry(XMLData& xmlData, float entryTop)
{

}

void TransformOption::CreateEntry(XMLData& xmlData, float yCoord, float labelCoord, float leftCoord,
	float optionCoord, float rightCoord, std::string label)
{
	sf::CircleShape* leftArrow = new sf::CircleShape{ (15) /*radius*/ };
	leftArrow->setPointCount(3);
	leftArrow->setPosition({ leftCoord + 4, yCoord + 4 });
	leftArrow->setFillColor(sf::Color::White);
	leftArrow->setRotation(sf::degrees(90));
	leftArrow->setScale({ 1.0f, -1.0f });
	shapes.push_back(leftArrow);

	sf::CircleShape* rightArrow = new sf::CircleShape{ (15)/*radius*/ };
	rightArrow->setPointCount(3);
	rightArrow->setPosition({ rightCoord + 26, yCoord + 4 });
	rightArrow->setFillColor(sf::Color::White);
	rightArrow->setRotation(sf::degrees(90));
	shapes.push_back(rightArrow);

	sf::Text* optionLabel = new sf::Text(UI::font, label);
	optionLabel->setPosition({ labelCoord, yCoord });
	labels.push_back(optionLabel);
	
	sf::Text* selectedOption = new sf::Text(UI::font, "selected");
	selectedOption->setPosition({ optionCoord, yCoord });
	labels.push_back(selectedOption);

	Button* leftButton  = new Button({ leftCoord, yCoord + 4 }/*position*/, { 30, 30 }/*size*/, " ");
	buttons.push_back(leftButton);

	Button* rightButton = new Button({ rightCoord, yCoord + 4 }/*position*/, { 30, 30 }/*size*/, " ");
	buttons.push_back(rightButton);
}

void TransformOption::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void TransformOption::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);

	Button* leftButton = buttons[(int)ButtonTypes::LeftButton];
	if (UI::CheckMouseInBounds(mousePos, leftButton->rect))
	{
		//todo swap the opttion left
	}
	Button* rightButton = buttons[(int)ButtonTypes::RightButton];
	if (UI::CheckMouseInBounds(mousePos, rightButton->rect))
	{
		//todo swap the opttion right
	}
}

void TransformOption::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
}

void TransformOption::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}
