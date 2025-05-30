#include "TransformPage.h"
#include "UI.h"

TransformPage::TransformPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize) :
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	addTransform({ 1051, 170 }, { 210, 30 }, "Add Transform"),
	testTransforms({ 1321, 170 }, { 230, 30 }, "Test Transforms")
{}

void TransformPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		addTransform.Draw(window);
		testTransforms.Draw(window);
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window);
		}
	}	
}

void TransformPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
{
    if (UI::CheckMouseInBounds(mousePos, addTransform.rect))
    {
		AddTransform();
    }
	if (UI::CheckMouseInBounds(mousePos, testTransforms.rect))
	{
		//TODO add a transform test function
	}

	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(mousePos);
	}
}

void TransformPage::Update(sf::RenderWindow& window, sf::Time timePassed,
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	}
}


void TransformPage::AddTransform()
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	TransformEntry pos{ pageTop + (boxSize + 6) * numEntries};
	entries.push_back(pos);
}

//-----------------------------------------------------------

TransformEntry::TransformEntry(float entryTop) :
	borderBox{ {580,288} },
	conditionsBox{ {572,116} },
	percentage({ 1540, entryTop + 96 }, { 35, 30 }, "%"),
	addCondition({ 1200, entryTop + 132 }, { 205, 30 }, "Add Condition"),
	amountBox({ 1140, entryTop + 96 }, { 70, 30 }, ""),
	upperBox({ 1320, entryTop + 96 }, { 50, 30 }, ""),
	lowerBox({ 1480, entryTop + 96 }, { 50, 30 }, ""),
	idBox({ 1370, entryTop +  176}, { 50, 30 }, ""),
	valueBox({ 1500, entryTop + 208 }, { 50, 30 }, ""),
	typeOptions(entryTop + 12, 1020, 1100, 1140, 1260, "Type:"),
	applyOptions(entryTop + 46, 1020, 1150, 1190, 1310, "Apply To:"),
	incOptions(entryTop + 12, 1320, 1380, 1420, 1540, "Inc:"),
	conditionTypeOptions(entryTop + 172, 1020, 1100, 1140, 1260, "Type:"),
	operatorOptions(entryTop + 208, 1020, 1150, 1190, 1310, "Operator:"),
	valueOptions(entryTop + 244, 1020, 1120, 1160, 1280, "Value:")
{
	borderBox.setPosition({ 1010, entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Blue);

	conditionsBox.setPosition({ 1014, entryTop + 168 });
	conditionsBox.setFillColor(sf::Color(192, 192, 192));
	conditionsBox.setOutlineThickness(2.0f);
	conditionsBox.setOutlineColor(sf::Color::Blue);

	amountLabel = new sf::Text(UI::font, "Amount:");
	amountLabel->setPosition({ 1020, entryTop + 92 });
	upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 1220, entryTop + 92 });
	lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 1380, entryTop + 92 }); 
	conditionsLabel = new sf::Text(UI::font, "Conditions:");
	conditionsLabel->setPosition({ 1020, entryTop + 128 });
	idLabel = new sf::Text(UI::font, "ID:");
	idLabel->setPosition({ 1310, entryTop + 172 });
	valueLabel = new sf::Text(UI::font, "Value:");
	valueLabel->setPosition({ 1400, entryTop + 208 });
}

void TransformEntry::Draw(sf::RenderWindow& window)
{
	window.draw(borderBox);
	window.draw(conditionsBox);
	amountBox.Draw(window);
	upperBox.Draw(window);
	lowerBox.Draw(window);
	idBox.Draw(window);
	valueBox.Draw(window);
	window.draw(*amountLabel);
	window.draw(*upperLabel);
	window.draw(*lowerLabel);
	window.draw(*conditionsLabel);
	window.draw(*idLabel);
	window.draw(*valueLabel);
	typeOptions.Draw(window);
	applyOptions.Draw(window);
	incOptions.Draw(window);
	conditionTypeOptions.Draw(window);
	operatorOptions.Draw(window);
	valueOptions.Draw(window);
	percentage.Draw(window);
	addCondition.Draw(window);
}

void TransformEntry::MouseClick(sf::Vector2i mousePos)
{
	if (UI::CheckMouseInBounds(mousePos, percentage.rect))
	{
		percentage.Toggle();
	}
	if (UI::CheckMouseInBounds(mousePos, addCondition.rect))
	{
		//todo add a conditions stuff
	}

	amountBox.active = UI::CheckMouseInBounds(mousePos, amountBox.box);
	upperBox.active = UI::CheckMouseInBounds(mousePos, upperBox.box);
	lowerBox.active = UI::CheckMouseInBounds(mousePos, lowerBox.box);
	idBox.active = UI::CheckMouseInBounds(mousePos, idBox.box);
	valueBox.active = UI::CheckMouseInBounds(mousePos, valueBox.box);

	typeOptions.MouseClick(mousePos);
	applyOptions.MouseClick(mousePos);
	incOptions.MouseClick(mousePos);
	conditionTypeOptions.MouseClick(mousePos);
	operatorOptions.MouseClick(mousePos);
	valueOptions.MouseClick(mousePos);
}

void TransformEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter,
	bool showCursor)
{
	//TODO make sure that you only care about numbers entered;
	typeOptions.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	applyOptions.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	incOptions.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	conditionTypeOptions.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	operatorOptions.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	valueOptions.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	amountBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	upperBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	lowerBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	idBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
	valueBox.Update(window, timePassed, keyPressed, backspace, enter, showCursor);
}

//-----------------------------------------------------------

TransformOptions::TransformOptions(float yCoord, float labelCoord, 
	float leftCoord, float optionCoord, float rightCoord, 
	std::string label) :
	leftArrow{ (15) },
	rightArrow{ (15) },
	leftButton({ leftCoord, yCoord+4 }, { 30, 30 }, " "),
	rightButton({ rightCoord, yCoord+4 }, { 30, 30 }, " ")
{
	leftArrow.setPointCount(3);
	leftArrow.setPosition({ leftCoord+4, yCoord+4 });
	leftArrow.setFillColor(sf::Color::White);
	leftArrow.setRotation(sf::degrees(90));
	leftArrow.setScale({ 1.0f, -1.0f });

	rightArrow.setPointCount(3);
	rightArrow.setPosition({ rightCoord+26, yCoord+4 });
	rightArrow.setFillColor(sf::Color::White);
	rightArrow.setRotation(sf::degrees(90));

	optionLabel = new sf::Text(UI::font, label);
	optionLabel->setPosition({ labelCoord, yCoord });
	selectedOption = new sf::Text(UI::font, "selected");
	selectedOption->setPosition({ optionCoord, yCoord });
}

void TransformOptions::Draw(sf::RenderWindow& window)
{
	leftButton.Draw(window);
	window.draw(leftArrow);
	rightButton.Draw(window);
	window.draw(rightArrow);
	window.draw(*optionLabel);
	window.draw(*selectedOption);
}

void TransformOptions::MouseClick(sf::Vector2i mousePos)
{
	if (UI::CheckMouseInBounds(mousePos, leftButton.rect))
	{
		//todo swap the opttion left
	}
	if (UI::CheckMouseInBounds(mousePos, rightButton.rect))
	{
		//todo swap the opttion right
	}
}

void TransformOptions::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter, bool showCursor)
{}
