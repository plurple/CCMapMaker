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
	addEntry.rect->setSize({ 210, 30 });
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
	PositionEntries();
    if (!showContinents.selected && UI::CheckMouseInBounds(mousePos, *addEntry.rect))
    {
		AddTransform(xmlData);
    }
	if (!showContinents.selected && UI::CheckMouseInBounds(mousePos, *testTransforms.rect))
	{
		//TODO add a transform test function
	}	
}

void TransformPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
    UserInput& input, bool showCursor, UIPageType pageType) 
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
	PositionEntries();
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

	conditionPos = { 0, entryTop + 170 };

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{ 580,170 } );/*size*/
	border->setPosition({ 10, entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(sf::Color::Blue);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> amountLabel = 
		std::make_shared<sf::Text>(UI::font, "Amount:");
	amountLabel->setPosition({ 340, entryTop + 8 });
	labels.push_back(amountLabel);
	
	std::shared_ptr<sf::Text> upperLabel = 
		std::make_shared<sf::Text>(UI::font, "Upper:");
	upperLabel->setPosition({ 360, entryTop + 50 });
	labels.push_back(upperLabel);
	
	std::shared_ptr<sf::Text> lowerLabel = 
		std::make_shared<sf::Text>(UI::font, "Lower:");
	lowerLabel->setPosition({ 360, entryTop + 92 });
	labels.push_back(lowerLabel);
	
	std::shared_ptr<sf::Text> conditionsLabel = 
		std::make_shared<sf::Text>(UI::font, "Conditions:");
	conditionsLabel->setPosition({ 20, entryTop + 134 });
	labels.push_back(conditionsLabel);

	std::shared_ptr<Transform> data = xmlData.transforms.at(xmlKey);
	std::shared_ptr<TextBox> amountBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 460, entryTop + 12 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	amountBox->number = &data->amount;
	boxes.push_back(amountBox);
	
	std::shared_ptr<TextBox> upperBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 460, entryTop + 54 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	upperBox->number = &data->upper;
	boxes.push_back(upperBox);
	
	std::shared_ptr<TextBox> lowerBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 460, entryTop + 96 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	lowerBox->number = &data->lower;
	boxes.push_back(lowerBox);

	std::shared_ptr<Button> percentage = 
		std::make_shared<Button>(sf::Vector2f{ 290, entryTop + 12 }/*position*/,
			sf::Vector2f{ 35, 30 }/*size*/, "%");
	percentage->xmlLink = &data->percentage;
	buttons.push_back(percentage);

	std::shared_ptr<Button> addCondition = 
		std::make_shared<Button>(sf::Vector2f{ 180, entryTop + 138 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/, "Add");
	buttons.push_back(addCondition);

	std::shared_ptr<Button> removeCondition =
		std::make_shared<Button>(sf::Vector2f{ 270, entryTop + 138 }/*position*/,
			sf::Vector2f{ 120, 30 }/*size*/, "Remove");
	buttons.push_back(removeCondition);

	std::shared_ptr<TransformOption> typeOptions = 
		std::make_shared<TransformOption>(xmlKey);
	typeOptions->CreateEntry(xmlData, entryTop + 8, 20, 100, 135, 230, "Type:");
	typeOptions->optionType = TransformOptionType::When;
	typeOptions->selectedOption = 1;
	entries.push_back(typeOptions);
	
	std::shared_ptr<TransformOption> applyOptions = 
		std::make_shared<TransformOption>(xmlKey);
	applyOptions->CreateEntry(xmlData, entryTop + 50, 20, 110, 145, 315, "Apply:");
	applyOptions->optionType = TransformOptionType::Who;
	applyOptions->selectedOption = 3;
	entries.push_back(applyOptions);
	
	std::shared_ptr<TransformOption> incOptions = 
		std::make_shared<TransformOption>(xmlKey);
	incOptions->CreateEntry(xmlData, entryTop + 92, 20, 75, 110, 290, "Inc:");
	incOptions->optionType = TransformOptionType::INC;
	incOptions->selectedOption = 2;
	entries.push_back(incOptions);

	Select();
	BorderBoxSize();
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
	if (mouseOnPage && percentage && UI::CheckMouseInBounds(mousePos, *percentage->rect))
	{
		percentage->Toggle();
	}
	std::shared_ptr<Button> addCondition = buttons[(int)ButtonTypes::AddCondition];
	if (mouseOnPage && addCondition && UI::CheckMouseInBounds(mousePos, *addCondition->rect))
	{
		//todo add a conditions stuff
		AddCondition(xmlData);
	}
	std::shared_ptr<Button> removeCondition = buttons[(int)ButtonTypes::RemoveCondition];
	if (mouseOnPage && removeCondition && UI::CheckMouseInBounds(mousePos, *removeCondition->rect))
	{
		//todo add a conditions stuff
		RemoveCondition(xmlData);
	}
	int oldCondition = selectedCondition;
	int index = 0;
	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		bool sel = false;
		entry->MouseClick(xmlData, mousePos, mouseOnPage, sel);
		if (sel)
		{
			if (index == oldCondition)
				oldCondition = -1;
			selectedCondition = index;
		}
		index++;
	}
	if (oldCondition == selectedCondition)
		selectedCondition = -1;
	if (!(oldCondition == -1 && selectedCondition == -1))
		SwapCondition(oldCondition, selectedCondition);
}

void TransformEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	if (input.del && selectedCondition != -1)
	{
		if (conditions[selectedCondition]->selected)
		{
			RemoveCondition(xmlData);
			input.del = false;
		}
	}
	if (input.tab && selectedCondition != -1)
	{
		int oldCondition = selectedCondition;
		input.shift ? selectedCondition-- : selectedCondition++;

		if (selectedCondition < 0)
			selectedCondition = conditions.size() - 1;
		else if (selectedCondition >= conditions.size())
			selectedCondition = 0;
		if (conditions.size() == 0)
			selectedCondition = -1;
		SwapCondition(oldCondition, selectedCondition);
		input.tab = false;
	}
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

	conditionPos += offset;

	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		entry->MoveEntry(offset);
	}
}

void TransformEntry::Select()
{
	UIEntry::Select();
}

void TransformEntry::Unselect(bool white)
{
	UIEntry::Unselect();
}

void TransformEntry::AddCondition(XMLData& xmlData)
{
	std::shared_ptr<ConditionEntry> condition =
		std::make_shared<ConditionEntry>(xmlKey, conditions.size());
	ConditionData data;
	xmlData.transforms[xmlKey]->conditions.insert({ conditions.size(), data });
	float offset = conditions.size() ? 125.0f : 0.0f;
	condition->CreateEntry(xmlData, conditionPos.y + conditions.size() * offset);
	conditions.push_back(condition);

	BorderBoxSize();
}

void TransformEntry::RemoveCondition(XMLData& xmlData)
{
	if (conditions.size())
	{
		if (selectedCondition == -1)
			selectedCondition = conditions.size() - 1;

		conditions[selectedCondition]->Unselect(true);
		auto transform = xmlData.transforms.at(xmlKey);
		transform->conditions.erase(selectedCondition);
		conditions.erase(conditions.begin() + selectedCondition);
		for (int i = selectedCondition; i < conditions.size(); i++)
		{
			conditions[i]->MoveEntry({ 0.0f, -125.0f });
		}
		BorderBoxSize();
	}
	selectedCondition = -1;
}

void TransformEntry::SwapCondition(int previous, int future)
{
	if (previous >= 0)
	{
		conditions[previous]->Unselect();
	}
	if (future >= 0)
	{
		conditions[future]->Select();
	}
}

void TransformEntry::BorderBoxSize()
{
	float borderHeight = 170.0f;

	int numConditions = conditions.size();
	borderHeight += numConditions ? numConditions * 125.0f : 10.0f;
	
	std::dynamic_pointer_cast<sf::RectangleShape>(shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 530, borderHeight });
}

//-----------------------------------------------------------

void ConditionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	selectedColor = sf::Color::Cyan;
	baseColor = sf::Color::Magenta;

	armyBoxPos = { 150, entryTop + 88 };
	roundBoxPos = { 180, entryTop + 88 };
	territoriesPos = { 190, entryTop + 88 };

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>(sf::Vector2f{ 522,116 } );/*size*/
	border->setPosition({ 14, entryTop + 8});
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(baseColor);
	shapes.push_back(border);

	std::shared_ptr<sf::Text> idLabel = 
		std::make_shared<sf::Text>(UI::font, "Territory:");
	idLabel->setPosition({ 380, entryTop + 12 });
	labels.push_back(idLabel);

	std::shared_ptr<sf::Text> valueLabel = 
		std::make_shared<sf::Text>(UI::font, "Value:");
	valueLabel->setPosition({ 20, entryTop + 84 });
	labels.push_back(valueLabel);

	std::shared_ptr<Button> addValue =
		std::make_shared<Button>(sf::Vector2f{ 305, entryTop + 52 }/*position*/,
			sf::Vector2f{ 30, 30 }/*size*/, "+");
	buttons.push_back(addValue);

	std::shared_ptr<Button> removeValue =
		std::make_shared<Button>(sf::Vector2f{ 340, entryTop + 52 }/*position*/,
			sf::Vector2f{ 30, 30 }/*size*/, "-");
	buttons.push_back(removeValue);

	std::shared_ptr<Transform> data = xmlData.transforms.at(xmlKey);
	std::shared_ptr<TextBox> valueBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 190, entryTop + 88 }/*position*/, 
			sf::Vector2f{ 70, 30 }/*size*/);
	valueBox->number = &data->conditions[conditionNum].values[0];
	boxes.push_back(valueBox);

	std::shared_ptr<TransformOption> typeOptions =
		std::make_shared<TransformOption>(xmlKey);
	typeOptions->CreateEntry(xmlData, entryTop + 12, 20, 100, 135, 290, "Type:");
	typeOptions->optionType = TransformOptionType::ConditionType;
	typeOptions->selectedOption = 1;
	entries.push_back(typeOptions);

	std::shared_ptr<TransformOption> operatorOptions =
		std::make_shared<TransformOption>(xmlKey);
	operatorOptions->CreateEntry(xmlData, entryTop + 48, 20, 150, 185, 270, "Operator:");
	operatorOptions->optionType = TransformOptionType::Operator;
	operatorOptions->selectedOption = 7;
	entries.push_back(operatorOptions);

	std::shared_ptr<TransformOption> valueOptions = 
		std::make_shared< TransformOption>(xmlKey);
	valueOptions->CreateEntry(xmlData, entryTop + 48, 20, 110, 145, 315, "Value:");
	valueOptions->optionType = TransformOptionType::Who;
	valueOptions->selectedOption = 7;
	valueOptions->skipAll = true;
	entries.push_back(valueOptions);

	SwapConditionType(typeOptions->selectedOption);
}

void ConditionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
}

void ConditionEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	int oldConditionType = std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select);
	int newConditionType = std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	if (oldConditionType != newConditionType)
	{
		SwapConditionType(newConditionType);
	}
}

void ConditionEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void ConditionEntry::MoveEntry(sf::Vector2f offset)
{
	armyBoxPos += offset;
	roundBoxPos += offset;
	territoriesPos += offset;

	UIEntry::MoveEntry(offset);
}

void ConditionEntry::SwapConditionType(int conditionType)
{
	float round = conditionType == (int)ConditionType::Round;
	float player = conditionType == (int)ConditionType::Player;
	float army = conditionType == (int)ConditionType::ArmyCount;

	labels[(int)LabelTypes::IDLabel]->setScale({ (float)(player || army), (float)(player || army) });
	labels[(int)LabelTypes::ValueLabel]->setScale({ (float)!player, (float)!player });
	labels[(int)LabelTypes::ValueLabel]->setString(round ? "Round #:" : army ? "Stack Size:" : "Territories:");
	boxes[(int)BoxTypes::ValueBox]->Hide((army||round));
	sf::Vector2f offset = (army ? roundBoxPos: armyBoxPos) - boxes[(int)BoxTypes::ValueBox]->box.getPosition();
	boxes[(int)BoxTypes::ValueBox]->Move(offset);
	std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Value])->Hide(player);
	std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Operator])->Hide(!player);
	buttons[(int)ButtonTypes::AddButton]->Hide((army || round));
	buttons[(int)ButtonTypes::RemoveButton]->Hide((army || round));
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
	if (UI::CheckMouseInBounds(mousePos, *leftButton->rect))
	{
		xmlData.ChangeTransformOption(optionType, selectedOption, false, skipAll);
	}
	std::shared_ptr<Button> rightButton = buttons[(int)ButtonTypes::RightButton];
	if (UI::CheckMouseInBounds(mousePos, *rightButton->rect))
	{
		xmlData.ChangeTransformOption(optionType, selectedOption, true, skipAll);
	}
}

void TransformOption::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	labels[(int)LabelTypes::SelectedOption]->setString(xmlData.GetTransformOptionString(optionType, selectedOption));
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);
}

void TransformOption::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);
}

void TransformOption::Hide(bool show)
{
	for (int i = 0; i< shapes.size(); i++)
	{
		shapes[i]->setScale({ (float)show, (float)show * (i ? 1.0f : -1.0f) });
	}
	for (auto label : labels)
	{
		label->setScale({ (float)show, (float)show });
	}
	for (auto button : buttons)
	{
		button->Hide(show);
	}
}
