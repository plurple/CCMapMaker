#include "TransformPage.h"
#include "UI.h"
#include "../XML/Transform.h"
#include "../XML/Territory.h"

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

bool TransformPage::MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	if (UIPage::MapClick(ui, xmlData, maps, mousePos, boxIndex))
	{
		if (selectedEntry != -1)
		{
			auto entry = std::dynamic_pointer_cast<TransformEntry>(entries[selectedEntry]);
			if (entry->selectedCondition != -1)
			{
				auto condition = std::dynamic_pointer_cast<ConditionEntry>(entry->conditions[entry->selectedCondition]);
				condition->MapClick(ui, xmlData, maps, mousePos, boxIndex);
			}
		}
	}
	return true;
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
	amountBox->allowNegative = true;
	amountBox->RemoveNumber();
	boxes.push_back(amountBox);
	
	std::shared_ptr<TextBox> upperBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 460, entryTop + 54 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	upperBox->number = &data->upper;
	upperBox->allowNegative = true;
	upperBox->RemoveNumber();
	boxes.push_back(upperBox);
	
	std::shared_ptr<TextBox> lowerBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 460, entryTop + 96 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	lowerBox->number = &data->lower;
	lowerBox->allowNegative = true;
	lowerBox->RemoveNumber();
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

void TransformEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
	bool& select, bool mapClicked)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);

	std::shared_ptr<Button> percentage = buttons[(int)ButtonTypes::Percentage];
	if (mouseOnPage && percentage && UI::CheckMouseInBounds(mousePos, *percentage->rect))
	{
		percentage->Toggle();
	}
	std::shared_ptr<Button> addCondition = buttons[(int)ButtonTypes::AddCondition];
	if (mouseOnPage && addCondition && UI::CheckMouseInBounds(mousePos, *addCondition->rect))
	{
		AddCondition(xmlData);
	}
	std::shared_ptr<Button> removeCondition = buttons[(int)ButtonTypes::RemoveCondition];
	if (mouseOnPage && removeCondition && UI::CheckMouseInBounds(mousePos, *removeCondition->rect))
	{
		RemoveCondition(xmlData);
	}
	int oldCondition = selectedCondition;
	int index = 0;
	for (std::shared_ptr<UIEntry> entry : conditions)
	{
		bool sel = false;
		entry->MouseClick(xmlData, mousePos, mouseOnPage, sel, mapClicked);
		if (sel)
		{
			if (index == oldCondition)
				oldCondition = -1;
			selectedCondition = index;
		}
		index++;
	}
	if (!mapClicked && oldCondition == selectedCondition)
		selectedCondition = -1;
	if (!(oldCondition == -1 && selectedCondition == -1))
		SwapCondition(oldCondition, selectedCondition);

	PositionConditions();
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

	PositionConditions();
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
		PositionConditions();
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
	borderHeight += conditionsHeight ? conditionsHeight : 10.0f;
	std::dynamic_pointer_cast<sf::RectangleShape>(shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 530, borderHeight });
}

void TransformEntry::PositionConditions()
{
	conditionsHeight = 10.0f;
	for (auto condition : conditions)
	{
		auto borderBox = std::dynamic_pointer_cast<sf::RectangleShape>(condition->shapes[(int)UIEntry::ShapeTypes::Border]);
		condition->MoveEntry({ 0, (conditionPos.y + conditionsHeight) - borderBox->getPosition().y });
		conditionsHeight += borderBox->getSize().y + 6;
	}
	BorderBoxSize();
}

//-----------------------------------------------------------

void ConditionEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	selectedColor = sf::Color::Cyan;
	baseColor = sf::Color::Magenta;

	roundBoxPos = { 150, entryTop + 100 };
	armyBoxPos = { 180, entryTop + 100 };
	territoriesPos = { 190, entryTop + 100 };
	territoryIDPos = { 380, entryTop + 65 };	

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
	valueLabel->setPosition({ 20, entryTop + 96 });
	labels.push_back(valueLabel);

	std::shared_ptr<Button> addValue =
		std::make_shared<Button>(sf::Vector2f{ 305, entryTop + 58 }/*position*/,
			sf::Vector2f{ 30, 30 }/*size*/, "+");
	buttons.push_back(addValue);

	std::shared_ptr<Button> removeValue =
		std::make_shared<Button>(sf::Vector2f{ 340, entryTop + 58 }/*position*/,
			sf::Vector2f{ 30, 30 }/*size*/, "-");
	buttons.push_back(removeValue);

	std::shared_ptr<Transform> data = xmlData.transforms.at(xmlKey);
	std::shared_ptr<TextBox> valueBox = 
		std::make_shared<TextBox>(sf::Vector2f{ 190, entryTop + 100 }/*position*/, 
			sf::Vector2f{ 70, 30 }/*size*/);
	valueBox->number = &data->conditions[conditionNum].values.at(0);
	boxes.push_back(valueBox);

	std::shared_ptr<TransformOption> typeOptions =
		std::make_shared<TransformOption>(xmlKey);
	typeOptions->CreateEntry(xmlData, entryTop + 12, 20, 100, 135, 290, "Type:");
	typeOptions->optionType = TransformOptionType::ConditionType;
	typeOptions->selectedOption = 1;
	entries.push_back(typeOptions);

	std::shared_ptr<TransformOption> operatorOptions =
		std::make_shared<TransformOption>(xmlKey);
	operatorOptions->CreateEntry(xmlData, entryTop + 54, 20, 150, 185, 270, "Operator:");
	operatorOptions->optionType = TransformOptionType::Operator;
	operatorOptions->selectedOption = 7;
	entries.push_back(operatorOptions);

	std::shared_ptr<TransformOption> valueOptions = 
		std::make_shared< TransformOption>(xmlKey);
	valueOptions->CreateEntry(xmlData, entryTop + 54, 20, 110, 145, 315, "Value:");
	valueOptions->optionType = TransformOptionType::Who;
	valueOptions->selectedOption = 7;
	valueOptions->skipAll = true;
	entries.push_back(valueOptions);

	SwapConditionType(typeOptions->selectedOption, operatorOptions->selectedOption);
}

void ConditionEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	if(territoryID.uiIndex != -1)
		territoryID.nameLabel->Draw(window);
	ConditionType optionType = (ConditionType)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	if (optionType == ConditionType::Territory)
	{
		for (auto territory : territories)
		{
			territory->nameLabel->Draw(window);
		}
	}
}

void ConditionEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, 
	bool mouseOnPage, bool& select, bool mapClicked)
{
	int oldConditionType = std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	int oldOperatorType = std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Operator])->selectedOption;
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);
	int newConditionType = std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	int newOperatorType = std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Operator])->selectedOption;
	if (oldConditionType != newConditionType)
	{
		SwapConditionType(newConditionType, newOperatorType);
		ResetValues(xmlData, newConditionType);
	}
	std::shared_ptr<Button> addValue = buttons[(int)ButtonTypes::AddButton];
	std::shared_ptr<Button> removeValue = buttons[(int)ButtonTypes::RemoveButton];
	if (oldOperatorType != newOperatorType)
	{
		bool ins = (Operators)newOperatorType == Operators::In || (Operators)newOperatorType == Operators::NotIn;
		if (newConditionType != (int)ConditionType::Territory)
		{
			addValue->Hide(ins);
			removeValue->Hide(ins);
		}
		if (!ins)
		{
			ResetValues(xmlData, newConditionType);
		}
	}
	if (mouseOnPage && addValue && UI::CheckMouseInBounds(mousePos, *addValue->rect))
	{
		AddValue(xmlData);
		BorderBoxSize(newConditionType);
	}
	if (mouseOnPage && removeValue && UI::CheckMouseInBounds(mousePos, *removeValue->rect))
	{
		RemoveValue(xmlData);
		BorderBoxSize(newConditionType);
	}

	if (territoryID.uiIndex != -1)
		territoryID.nameLabel->active = mouseOnPage && UI::CheckMouseInBounds(mousePos, territoryID.nameLabel->box);
	ConditionType optionType = (ConditionType)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	if (optionType == ConditionType::Territory)
	{
		for (auto territory : territories)
		{
			territory->nameLabel->active = mouseOnPage && UI::CheckMouseInBounds(mousePos, territory->nameLabel->box);
		}
	}
}

bool ConditionEntry::MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	switch ((ConditionType)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption)
	{
	case ConditionType::Player:
	case ConditionType::ArmyCount:
		AddTerritoryID(xmlData, maps.mapBoxes[boxIndex]->border, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
		break;
	case ConditionType::Territory:
	{
		int i;
		bool removed = false;
		for (i = 0; i < territories.size(); i++)
		{
			auto territory = std::dynamic_pointer_cast<LinkedData>(territories[i]);
			if (territory->uiIndex == boxIndex)
			{
				if(std::dynamic_pointer_cast<LinkedData>(territories[i])->xmlKey == 0)
					RemoveTerritory(xmlData, i);
				else
					xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).values.erase(std::dynamic_pointer_cast<LinkedData>(territories[i])->xmlKey);				
				maps.mapBoxes[boxIndex]->border->setOutlineColor(sf::Color::White);				
				territories.erase(territories.begin() + i);
				removed = true;				
				break;
			}
		}
		if (removed)
		{
			for (int j = i; j < territories.size(); j++)
			{
				territories[j]->nameLabel->Move({ 0, -30 });
			}
		}
		else
		{
			Operators oper = (Operators)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Operator])->selectedOption;

			if (territories.size() && oper != Operators::In && oper != Operators::NotIn)
			{
				RemoveTerritory(xmlData, 0);
				territories.erase(territories.begin());
			}

			AddTerritory(xmlData, maps.mapBoxes[boxIndex]->border, boxIndex, ui.uiPages[(int)UIPageType::Territory]->entries[boxIndex]->xmlKey);
		}
		BorderBoxSize((int)ConditionType::Territory);
		break;
	}
	}
	return true;
}

void ConditionEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	UIEntry::Update(xmlData, window, timePassed, input, showCursor);

	if (territoryID.uiIndex != -1)
	{
		if (xmlData.territories.find(territoryID.xmlKey) != xmlData.territories.end())
		{
			territoryID.nameLabel->Update(window, timePassed, input, showCursor);
		}
		else
		{
			RemoveTerritoryID(xmlData);
		}
	}
	ConditionType optionType = (ConditionType)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	if (optionType == ConditionType::Territory)
	{
		for (int i = 0; i < territories.size(); i++)
		{
			if (xmlData.territories.find(territories[i]->xmlKey) != xmlData.territories.end())
			{
				territories[i]->nameLabel->Update(window, timePassed, input, showCursor);
			}
			else if(territories[i]->xmlKey != -1)
			{
				if(i == 0)
					RemoveTerritory(xmlData, i);
				else
				{
					xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).values.erase(std::dynamic_pointer_cast<LinkedData>(territories[i])->xmlKey);
				}
				territories.erase(territories.begin() + i);
			}
		}
	}
}

void ConditionEntry::MoveEntry(sf::Vector2f offset)
{
	armyBoxPos += offset;
	roundBoxPos += offset;
	territoriesPos += offset;
	territoryIDPos += offset;

	UIEntry::MoveEntry(offset);
	if(territoryID.uiIndex != -1)
		territoryID.nameLabel->Move(offset);
	ConditionType optionType = (ConditionType)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	if (optionType == ConditionType::Territory)
	{
		for (auto territory : territories)
		{
			territory->nameLabel->Move(offset);
		}
	}
}

void ConditionEntry::SwapConditionType(int conditionType, int operatorType)
{
	float round = conditionType == (int)ConditionType::Round;
	float player = conditionType == (int)ConditionType::Player;
	float army = conditionType == (int)ConditionType::ArmyCount;

	labels[(int)LabelTypes::IDLabel]->setScale({ (float)(player || army), (float)(player || army) });
	if(territoryID.uiIndex != -1)
		territoryID.nameLabel->Hide((float)(player || army));
	labels[(int)LabelTypes::ValueLabel]->setScale({ (float)!player, (float)!player });
	labels[(int)LabelTypes::ValueLabel]->setString(round ? "Round #:" : army ? "Stack Size:" : "Territories:");
	boxes[(int)BoxTypes::ValueBox]->Hide((army||round));
	sf::Vector2f offset = (army ? armyBoxPos: roundBoxPos) - boxes[(int)BoxTypes::ValueBox]->box.getPosition();
	for (auto box : boxes)
	{
		box->Move(offset);
	}
	std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Value])->Hide(player);
	std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Operator])->Hide(!player);

	bool ins = (Operators)operatorType == Operators::In || (Operators)operatorType == Operators::NotIn;
	if (ins)
	{
		buttons[(int)ButtonTypes::AddButton]->Hide((army || round));
		buttons[(int)ButtonTypes::RemoveButton]->Hide((army || round));
	}
	BorderBoxSize(conditionType);
}

void ConditionEntry::BorderBoxSize(int conditionType)
{
	float borderHeight = 86.0f;

	int numLines = 0;

	switch ((ConditionType)conditionType)
	{
	case ConditionType::ArmyCount:
	case ConditionType::Round:		
		numLines = (((boxes.size() + 1) / 4) + (int)((bool)((boxes.size() + 1) % 4)));
		break;
	case ConditionType::Territory:
		numLines = territories.size() ? territories.size() : 1;
		break;
	}
	borderHeight += numLines ? numLines * 40.0f : 5.0f;
	std::dynamic_pointer_cast<sf::RectangleShape>(shapes[(int)UIEntry::ShapeTypes::Border])->setSize({ 522, borderHeight });
}

void ConditionEntry::AddTerritoryID(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> border,
	int boxIndex, int otherXMLKey)
{
	if (territoryID.uiIndex == boxIndex)
	{
		RemoveTerritoryID(xmlData);
	}
	else
	{
		territoryID.uiIndex = boxIndex;
		territoryID.mapBox = std::make_shared<MapBox>();
		territoryID.nameLabel = std::make_shared<TextBox>(territoryIDPos, sf::Vector2f{ 150, 20 });
		territoryID.nameLabel->displayText->setCharacterSize(20);
		territoryID.nameLabel->baseColor = sf::Color::Magenta;
		territoryID.mapBox->border = border;
		territoryID.mapBox->border->setOutlineColor(sf::Color::Magenta);
		territoryID.xmlKey = otherXMLKey;
		territoryID.nameLabel->text = &xmlData.territories.at(otherXMLKey)->name;
		xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).index = otherXMLKey;
	}	
}

void ConditionEntry::RemoveTerritoryID(XMLData& xmlData)
{
	territoryID.uiIndex = -1;
	territoryID.xmlKey = -1;
	territoryID.mapBox->border->setOutlineColor(sf::Color::White);
	xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).index = -1;
	territoryID.mapBox->border = nullptr;
}

void ConditionEntry::AddTerritory(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> border,
	int boxIndex, int otherXMLKey)
{
	std::shared_ptr<LinkedData> territory = std::make_shared<LinkedData>();
	territory->uiIndex = boxIndex;
	territory->mapBox = std::make_shared<MapBox>();
	territory->nameLabel = std::make_shared<TextBox>(territoriesPos + sf::Vector2f{ 0, territories.size() * 30.0f },
		sf::Vector2f{ 150, 20 });
	territory->nameLabel->displayText->setCharacterSize(20);
	territory->nameLabel->baseColor = sf::Color::Magenta;
	territory->mapBox->border = border;
	territory->mapBox->border->setOutlineColor(sf::Color::Magenta);
	territory->xmlKey = otherXMLKey;
	territory->nameLabel->text = &xmlData.territories.at(otherXMLKey)->name;
	xmlData.transforms[xmlKey]->conditions[conditionNum].values.insert_or_assign(territories.size(), otherXMLKey); 
	territories.push_back(territory);
}

void ConditionEntry::RemoveTerritory(XMLData& xmlData, int index)
{
	if (territories.size())
	{
		territories[index]->uiIndex = -1;
		territories[index]->xmlKey = -1;
		territories[index]->mapBox->border->setOutlineColor(sf::Color::White);
		xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).values.insert_or_assign(index, -1);
		territories[index]->mapBox->border = nullptr;
	}
}

void ConditionEntry::AddValue(XMLData& xmlData)
{
	int num = boxes.size();
	std::shared_ptr<TextBox> box =
		std::make_shared<TextBox>(sf::Vector2f{ 190, 100 }/*position*/,
			sf::Vector2f{ 70, 30 }/*size*/);
	sf::Vector2f gridPos = sf::Vector2f{ (num % 4) * 80.0f, (num / 4) * 40.0f };
	ConditionType optionType = (ConditionType)std::dynamic_pointer_cast<TransformOption>(entries[(int)EntryTypes::Type])->selectedOption;
	sf::Vector2f startPos = optionType == ConditionType::ArmyCount ? armyBoxPos : roundBoxPos;
	box->box.setPosition(startPos + gridPos);
	box->displayText->setPosition(startPos + gridPos);
	xmlData.transforms[xmlKey]->conditions[conditionNum].values.insert_or_assign(num, -1);
	box->number = &xmlData.transforms[xmlKey]->conditions[conditionNum].values.at(num);
	boxes.push_back(box);	
}

void ConditionEntry::RemoveValue(XMLData& xmlData)
{
	if (boxes.size() > 1)
	{
		auto condition = xmlData.transforms.at(xmlKey)->conditions.at(conditionNum);
		auto num = boxes.size();
		condition.values.erase(num);
		boxes.pop_back();
	}
	else
	{
		xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).values.insert_or_assign(0, -1);
	}
}

void ConditionEntry::ResetValues(XMLData& xmlData, int conditionType)
{
	switch ((ConditionType)conditionType)
	{
	case ConditionType::ArmyCount:
	case ConditionType::Round:
		for (auto box : boxes)
		{
			RemoveValue(xmlData);
		}
		break;
	case ConditionType::Territory:
		for (int i = territories.size() - 1; i >= 0; i--)
		{
			if (std::dynamic_pointer_cast<LinkedData>(territories[i])->xmlKey == 0)
				RemoveTerritory(xmlData, i);
			else
				xmlData.transforms.at(xmlKey)->conditions.at(conditionNum).values.erase(std::dynamic_pointer_cast<LinkedData>(territories[i])->xmlKey);
			territories.erase(territories.begin() + i);
		}
		break;
	}
	BorderBoxSize(conditionType);
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

void TransformOption::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, 
	bool mouseOnPage, bool& select, bool mapClicked)
{
	UIEntry::MouseClick(xmlData, mousePos, mouseOnPage, select, mapClicked);

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
