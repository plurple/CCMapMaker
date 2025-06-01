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
		window.setView(scrollBar.scrollWindow);
		for (int i = 0; i < entries.size(); i++)
		{
			entries[i].Draw(window);
		}
		window.draw(page);
		scrollBar.Draw(window);
		window.setView(window.getDefaultView());
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
	if (mouseOnPage)
	{
		scrollBar.MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)));
	}
	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)),
			mouseOnPage);
	}
	
}

void TransformPage::Update(sf::RenderWindow& window, sf::Time timePassed,
    std::string keyPressed, bool backspace, bool enter, bool showCursor,
	bool verticle, float scrolled)
{
	mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), page);

	if (!verticle || !mouseOnPage)
	{
		scrolled = 0.0f;
	}
	else
	{
		scrolled *= 7;
	}

	if (enter)
	{
		scrolled += 50;
	}
	if (backspace)
	{
		scrolled -= 50;
	}

	scrollBar.Scroll({ 0, scrolled });
	int numEntries = entries.size();
	if (numEntries)
		scrollBar.MoveBar({ 0, 10+(entries[0].borderBox.getSize().y + 6) * (numEntries) });

	float topBoxY = numEntries ? entries[0].borderBox.getPosition().y : scrollBar.currentScroll.y;
	if (scrollBar.currentScroll.y != topBoxY)
		scrolled = scrollBar.currentScroll.y - topBoxY;


	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].Update(window, timePassed, keyPressed, backspace, enter, showCursor, scrolled);
	}
}


void TransformPage::AddTransform()
{
	int numEntries = entries.size();
	float topBoxY = numEntries ? entries[0].borderBox.getPosition().y : 0.0f;
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	TransformEntry pos{ topBoxY + (boxSize + 6) * numEntries};
	entries.push_back(pos);
	scrollBar.BarSize({ 0, (entries[0].borderBox.getSize().y + 6) * (numEntries + 1) });
}

//-----------------------------------------------------------

TransformEntry::TransformEntry(float entryTop) :
	borderBox{ {580,288} /*size*/},
	conditionsBox{ {572,116} /*size*/ },
	percentage({ 540, entryTop + 96 }/*position*/, { 35, 30 }/*size*/, "%"),
	addCondition({ 200, entryTop + 132 }/*position*/, { 205, 30 }/*size*/, "Add Condition"),
	amountBox({ 140, entryTop + 96 }/*position*/, { 70, 30 }/*size*/, ""),
	upperBox({ 320, entryTop + 96 }/*position*/, { 50, 30 }/*size*/, ""),
	lowerBox({ 480, entryTop + 96 }/*position*/, { 50, 30 }/*size*/, ""),
	idBox({ 370, entryTop +  176}/*position*/, { 50, 30 }/*size*/, ""),
	valueBox({ 500, entryTop + 208 }/*position*/, { 50, 30 }/*size*/, ""),
	typeOptions(entryTop + 12, 20, 100, 140, 260, "Type:"),
	applyOptions(entryTop + 46, 20, 150, 190, 310, "Apply To:"),
	incOptions(entryTop + 12, 320, 380, 420, 540, "Inc:"),
	conditionTypeOptions(entryTop + 172, 20, 100, 140, 260, "Type:"),
	operatorOptions(entryTop + 208, 20, 150, 190, 310, "Operator:"),
	valueOptions(entryTop + 244, 20, 120, 160, 280, "Value:")
{
	borderBox.setPosition({ 10, entryTop });
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Blue);

	conditionsBox.setPosition({ 14, entryTop + 168 });
	conditionsBox.setFillColor(sf::Color(192, 192, 192));
	conditionsBox.setOutlineThickness(2.0f);
	conditionsBox.setOutlineColor(sf::Color::Blue);

	amountLabel = new sf::Text(UI::font, "Amount:");
	amountLabel->setPosition({ 20, entryTop + 92 });
	upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition({ 220, entryTop + 92 });
	lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition({ 380, entryTop + 92 });
	conditionsLabel = new sf::Text(UI::font, "Conditions:");
	conditionsLabel->setPosition({ 20, entryTop + 128 });
	idLabel = new sf::Text(UI::font, "ID:");
	idLabel->setPosition({ 310, entryTop + 172 });
	valueLabel = new sf::Text(UI::font, "Value:");
	valueLabel->setPosition({ 400, entryTop + 208 });
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

void TransformEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	if (mouseOnPage && UI::CheckMouseInBounds(mousePos, percentage.rect))
	{
		percentage.Toggle();
	}
	if (mouseOnPage && UI::CheckMouseInBounds(mousePos, addCondition.rect))
	{
		//todo add a conditions stuff
	}

	amountBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, amountBox.box);
	upperBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, upperBox.box);
	lowerBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, lowerBox.box);
	idBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, idBox.box);
	valueBox.active = mouseOnPage && UI::CheckMouseInBounds(mousePos, valueBox.box);

	if (mouseOnPage)
	{
		typeOptions.MouseClick(mousePos);
		applyOptions.MouseClick(mousePos);
		incOptions.MouseClick(mousePos);
		conditionTypeOptions.MouseClick(mousePos);
		operatorOptions.MouseClick(mousePos);
		valueOptions.MouseClick(mousePos);
	}
}

void TransformEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	std::string keyPressed, bool backspace, bool enter,
	bool showCursor, float scrolled)
{
	MoveEntry({ 0, scrolled });

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

void TransformEntry::MoveEntry(sf::Vector2f offset)
{
	borderBox.move(offset);
	typeOptions.MoveOption(offset);
	applyOptions.MoveOption(offset);
	incOptions.MoveOption(offset);
	amountLabel->move(offset);
	amountBox.MoveBox(offset);
	upperLabel->move(offset);
	upperBox.MoveBox(offset);
	lowerLabel->move(offset);
	lowerBox.MoveBox(offset);
	percentage.MoveButton(offset);
	conditionsLabel->move(offset);
	conditionsBox.move(offset);
	addCondition.MoveButton(offset);
	conditionTypeOptions.MoveOption(offset);
	idLabel->move(offset);
	idBox.MoveBox(offset); //this might want to be a territory picker sometimes
	operatorOptions.MoveOption(offset);
	valueOptions.MoveOption(offset);
	valueLabel->move(offset);
	valueBox.MoveBox(offset);
}

//-----------------------------------------------------------

TransformOptions::TransformOptions(float yCoord, float labelCoord,
	float leftCoord, float optionCoord, float rightCoord, 
	std::string label) :
	leftArrow{ (15) /*radius*/},
	rightArrow{ (15)/*radius*/ },
	leftButton({ leftCoord, yCoord+4 }/*position*/, {30, 30}/*size*/, " "),
	rightButton({ rightCoord, yCoord+4 }/*position*/, {30, 30}/*size*/, " ")
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

void TransformOptions::MoveOption(sf::Vector2f offset)
{
	optionLabel->move(offset);
	leftButton.MoveButton(offset);
	leftArrow.move(offset);
	selectedOption->move(offset);
	rightButton.MoveButton(offset);
	rightArrow.move(offset);
}
