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
		AddTransform(window);
    }
	if (UI::CheckMouseInBounds(mousePos, testTransforms.rect))
	{
		//TODO add a transform test function
	}

	for (int i = 0; i < entries.size(); i++)
	{
		entries[i].MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)));
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


void TransformPage::AddTransform(sf::RenderWindow& window)
{
	int numEntries = entries.size();
	float boxSize = numEntries ? entries[numEntries - 1].borderBox.getSize().y : 0.0f;
	TransformEntry pos{window, scrollBar.scrollWindow, 100+(boxSize + 6) * numEntries};
	entries.push_back(pos);
}

//-----------------------------------------------------------

TransformEntry::TransformEntry(sf::RenderWindow& window, sf::View& view, float entryTop) :
	borderBox{ {580,288} /*size*/},
	conditionsBox{ {572,116} /*size*/ },
	percentage(sf::Vector2f(window.mapCoordsToPixel({ 1540, entryTop + 96 }, view))/*position*/, { 35, 30 }/*size*/, "%"),
	addCondition(sf::Vector2f(window.mapCoordsToPixel({ 1200, entryTop + 132 }, view))/*position*/, { 205, 30 }/*size*/, "Add Condition"),
	amountBox(sf::Vector2f(window.mapCoordsToPixel({ 1140, entryTop + 96 }, view))/*position*/, { 70, 30 }/*size*/, ""),
	upperBox(sf::Vector2f(window.mapCoordsToPixel({ 1320, entryTop + 96 }, view))/*position*/, { 50, 30 }/*size*/, ""),
	lowerBox(sf::Vector2f(window.mapCoordsToPixel({ 1480, entryTop + 96 }, view))/*position*/, { 50, 30 }/*size*/, ""),
	idBox(sf::Vector2f(window.mapCoordsToPixel({ 1370, entryTop +  176}, view))/*position*/, { 50, 30 }/*size*/, ""),
	valueBox(sf::Vector2f(window.mapCoordsToPixel({ 1500, entryTop + 208 }, view))/*position*/, { 50, 30 }/*size*/, ""),
	typeOptions(window, view, entryTop + 12, 1020, 1100, 1140, 1260, "Type:"),
	applyOptions(window, view, entryTop + 46, 1020, 1150, 1190, 1310, "Apply To:"),
	incOptions(window, view, entryTop + 12, 1320, 1380, 1420, 1540, "Inc:"),
	conditionTypeOptions(window, view, entryTop + 172, 1020, 1100, 1140, 1260, "Type:"),
	operatorOptions(window, view, entryTop + 208, 1020, 1150, 1190, 1310, "Operator:"),
	valueOptions(window, view, entryTop + 244, 1020, 1120, 1160, 1280, "Value:")
{
	borderBox.setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1010, entryTop }, view)));
	borderBox.setFillColor(sf::Color(192, 192, 192));
	borderBox.setOutlineThickness(2.0f);
	borderBox.setOutlineColor(sf::Color::Blue);

	conditionsBox.setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1014, entryTop + 168 }, view)));
	conditionsBox.setFillColor(sf::Color(192, 192, 192));
	conditionsBox.setOutlineThickness(2.0f);
	conditionsBox.setOutlineColor(sf::Color::Blue);

	amountLabel = new sf::Text(UI::font, "Amount:");
	amountLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 92 }, view)));
	upperLabel = new sf::Text(UI::font, "Upper:");
	upperLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1220, entryTop + 92 }, view)));
	lowerLabel = new sf::Text(UI::font, "Lower:");
	lowerLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1380, entryTop + 92 }, view)));
	conditionsLabel = new sf::Text(UI::font, "Conditions:");
	conditionsLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1020, entryTop + 128 }, view)));
	idLabel = new sf::Text(UI::font, "ID:");
	idLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1310, entryTop + 172 }, view)));
	valueLabel = new sf::Text(UI::font, "Value:");
	valueLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ 1400, entryTop + 208 }, view)));
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

TransformOptions::TransformOptions(sf::RenderWindow& window, sf::View& view, 
	float yCoord, float labelCoord,
	float leftCoord, float optionCoord, float rightCoord, 
	std::string label) :
	leftArrow{ (15) /*radius*/},
	rightArrow{ (15)/*radius*/ },
	leftButton(sf::Vector2f(window.mapCoordsToPixel({ leftCoord, yCoord+4 }, view))/*position*/, {30, 30}/*size*/, " "),
	rightButton(sf::Vector2f(window.mapCoordsToPixel({ rightCoord, yCoord+4 }, view))/*position*/, {30, 30}/*size*/, " ")
{
	leftArrow.setPointCount(3);
	leftArrow.setPosition(sf::Vector2f(window.mapCoordsToPixel({ leftCoord+4, yCoord+4 }, view)));
	leftArrow.setFillColor(sf::Color::White);
	leftArrow.setRotation(sf::degrees(90));
	leftArrow.setScale({ 1.0f, -1.0f });

	rightArrow.setPointCount(3);
	rightArrow.setPosition(sf::Vector2f(window.mapCoordsToPixel({ rightCoord+26, yCoord+4 }, view)));
	rightArrow.setFillColor(sf::Color::White);
	rightArrow.setRotation(sf::degrees(90));

	optionLabel = new sf::Text(UI::font, label);
	optionLabel->setPosition(sf::Vector2f(window.mapCoordsToPixel({ labelCoord, yCoord }, view)));
	selectedOption = new sf::Text(UI::font, "selected");
	selectedOption->setPosition(sf::Vector2f(window.mapCoordsToPixel({ optionCoord, yCoord }, view)));
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
