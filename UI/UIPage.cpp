#include "UIPage.h"
#include "UI.h"

UIPage::UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
	std::string tabLabel, sf::Vector2f buttonBoxSize) :
	tabButton(tabPos, tabSize, tabLabel),
	addEntry(tabPos, tabSize, tabLabel),
	page{{592,UI::windowSize.y-158-8-buttonBoxSize.y}},
	buttonBox{ buttonBoxSize },
	mouseOnPage{false},
	showContinents(tabPos, { 240, 30 }, "Show Continents"),
	selectedEntry{ -1 },
	scrollBar(sf::View{ page.getGlobalBounds() }, { 555, 50 }/*position*/, { 30, page.getSize().y - 100 }/*size*/)
{
	page.setPosition({ 0, 0 });
	page.setFillColor(sf::Color::Transparent);
	page.setOutlineThickness(4.0f);
	page.setOutlineColor(sf::Color::White);

	scrollBar.scrollWindow = sf::View{ page.getGlobalBounds() };
	float heightRatio = (page.getSize().y + 8.0f) / UI::windowSize.y;
	float widthRatio = (page.getSize().x + 8.0f) / UI::windowSize.x;
	scrollBar.scrollWindow.setViewport(sf::FloatRect({ 1.0f - widthRatio, 1.0f-heightRatio }, { widthRatio, heightRatio }));
	scrollBar.minScroll.y = 10.0f;

	buttonBox.setPosition({ 1004, 154 });
	buttonBox.setFillColor(sf::Color::Transparent);
	buttonBox.setOutlineThickness(4.0f);
	buttonBox.setOutlineColor(sf::Color::Cyan);
}

void UIPage::Draw(sf::RenderWindow& window, bool selected)
{
	tabButton.Draw(window);
	if (selected)
	{
		window.draw(buttonBox);
		addEntry.Draw(window);
		window.setView(scrollBar.scrollWindow);
		for (std::shared_ptr<UIEntry> entry : entries)
		{
			entry->Draw(window);
		}
		window.draw(page);
		scrollBar.Draw(window);
		window.setView(window.getDefaultView());
	}
}

void UIPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	if (mouseOnPage)
	{
		scrollBar.MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)));
	}
	selectedEntry = -1;
	int index = 0;
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		bool select = false;
		entry->MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)), mouseOnPage, select);
		if (select)
			selectedEntry = index;
		index++;
	}
}

bool UIPage::MapClick(XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	int index = 0;
	for (std::shared_ptr<sf::RectangleShape> box : maps.mapBoxes)
	{
		if (UI::CheckMouseInBounds(mousePos, *box))
		{
			boxIndex = index;
			return true;
		}
		index++;
	}

	return false;
}

void UIPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor, UIPageType pageType)
{
	if (input.del)
	{
		if (entries[selectedEntry]->selected)
		{
			//todo remove entry and all that entails.
			xmlData.RemoveData(pageType, entries[selectedEntry]->xmlKey);
			entries.erase(entries.begin() + selectedEntry);
		}
		if (selectedEntry < entries.size())
		{
			for (int i = selectedEntry; i < entries.size(); i++)
			{
				entries[i]->MoveEntry({ 0,  -entries[0]->shapes[0]->getGlobalBounds().size.y - 6 });
			}
		}
		selectedEntry = -1;		
	}
	if (input.tab)
	{
		if (selectedEntry >= 0)
		{
			entries[selectedEntry]->Unselect();
		}
		input.shift ? selectedEntry-- : selectedEntry++;
		if (selectedEntry < 0)
			selectedEntry = entries.size() - 1;
		else if (selectedEntry >= entries.size())
			selectedEntry = 0;
		if(selectedEntry >= 0)
		{
			entries[selectedEntry]->Select();
			auto entryPos = entries[selectedEntry]->boxes[(int)UIEntry::ShapeTypes::Border]->box.getPosition();
			scrollBar.Scroll({ 0, -entryPos.y + 20 });
		}
	}

	mouseOnPage = UI::CheckMouseInBounds(sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window), scrollBar.scrollWindow)), page);

	if (!input.verticle || !mouseOnPage)
	{
		input.scroll = 0.0f;
	}
	else
	{
		input.scroll *= 7;
	}
	scrollBar.Scroll({ 0, input.scroll });
	int numEntries = entries.size();
	if (numEntries)
		scrollBar.MoveBar({ 0, 10 + (entries[0]->shapes[0]->getGlobalBounds().size.y + 6) * (numEntries) });

	float topBoxY = numEntries ? entries[0]->shapes[0]->getPosition().y : scrollBar.currentScroll.y;
	if (scrollBar.currentScroll.y != topBoxY)
		input.scroll = scrollBar.currentScroll.y - topBoxY;

	for (std::shared_ptr<UIEntry> entry : entries)
	{
		entry->Update(xmlData, window, timePassed, input, showCursor);
	}
}

void UIPage::AddEntry(XMLData& xmlData, std::shared_ptr<UIEntry> entry)
{
	int numEntries = entries.size();
	float topBoxY = numEntries ? entries[0]->shapes[0]->getPosition().y : 0.0f;
	float boxSize = numEntries ? entries[0]->shapes[0]->getGlobalBounds().size.y : 0.0f;
	entry->CreateEntry(xmlData, topBoxY + (boxSize + 6) * numEntries);
	if (selectedEntry != -1) entries[selectedEntry]->Unselect();
	selectedEntry = entries.size();
	entries.push_back(entry);
	scrollBar.BarSize({ 0, (boxSize + 6) * (numEntries + 1) });
	scrollBar.MoveBar({ 0, 10 + (boxSize + 6) * (numEntries + 1) });
	scrollBar.Scroll({ 0, scrollBar.maxScroll.y * -1 });
}

//----------------------------------------------------------------

void UIEntry::Draw(sf::RenderWindow& window)
{
	for (std::shared_ptr<sf::Text> text : labels)
	{
		window.draw(*text);
	}
	for (std::shared_ptr<Button> butt : buttons)
	{
		butt->Draw(window);
	}
	for (std::shared_ptr<TextBox> box : boxes)
	{
		box->Draw(window);
	}
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		entry->Draw(window);
	}
	for (std::shared_ptr<sf::Shape> shape : shapes)
	{
		window.draw(*shape);
	}
}

void UIEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	if (shapes.size())
	{
		std::shared_ptr<sf::Shape> borderBox = shapes[(int)ShapeTypes::Border];
		Toggle(mouseOnPage && borderBox && UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()));
		select = selected;
	}

	for (std::shared_ptr<TextBox> box : boxes)
	{
		box->Toggle(UI::CheckMouseInBounds(mousePos, box->box));
	}
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		entry->MouseClick(mousePos, mouseOnPage, select);
	}
}

void UIEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	for (std::shared_ptr<TextBox> box : boxes)
	{
		box->Update(window, timePassed, input, showCursor);
	}
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		entry->Update(xmlData, window, timePassed, input, showCursor);
	}
}

void UIEntry::MoveEntry(sf::Vector2f offset)
{
	for (std::shared_ptr<sf::Shape> shape : shapes)
	{
		shape->move(offset);
	}
	for (std::shared_ptr<sf::Text> text : labels)
	{
		text->move(offset);
	}
	for (std::shared_ptr<Button> butt : buttons)
	{
		butt->Move(offset);
	}
	for (std::shared_ptr<TextBox> box : boxes)
	{
		box->Move(offset);
	}
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		entry->MoveEntry(offset);
	}
}

void UIEntry::Select()
{
	selected = true;
	shapes[0]->setOutlineThickness(4.0f);
	shapes[0]->setOutlineColor(selectedColor);
}

void UIEntry::Unselect()
{
	selected = false;
	shapes[0]->setOutlineThickness(2.0f);
	shapes[0]->setOutlineColor(baseColor);
}

void UIEntry::Toggle(bool toggle)
{
	toggle ? Select() : Unselect();
}