#include "UIPage.h"
#include "UI.h"

UIPage::UIPage(sf::Vector2f tabPos, sf::Vector2f tabSize, 
	std::string tabLabel, sf::Vector2f buttonBoxSize, bool& continentPanel) :
	tabButton(tabPos, tabSize, tabLabel),
	addEntry(tabPos, tabSize, tabLabel),
	page{{592,UI::windowSize.y-158-8-buttonBoxSize.y}},
	buttonBox{ buttonBoxSize },
	mouseOnPage{false},
	showContinents(tabPos, { 240, 30 }, "Show Continents"),
	selectedEntry{ -1 },
	scrollBar(sf::View{ page.getGlobalBounds() }, { 555, 50 }/*position*/, { 30, page.getSize().y - 100 }/*size*/),
	contentSize{0.0f}
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

	showContinents.xmlLink = &continentPanel;
}

void UIPage::Draw(sf::RenderWindow& window, bool selected)
{
	tabButton.Draw(window);
	if (selected)
	{
		window.draw(buttonBox);
		addEntry.Draw(window);
		window.setView(scrollBar.scrollWindow);
		sf::View view = window.getView();
		sf::FloatRect viewRect(
			{ view.getCenter().x - view.getSize().x / 2.f,
			view.getCenter().y - view.getSize().y / 2.f },
			{ view.getSize().x,
			view.getSize().y }
		);
		for (std::shared_ptr<UIEntry> entry : entries)
		{
			bool onScreen = entry->shapes[0]->getGlobalBounds().findIntersection(viewRect) != std::nullopt;
			if(onScreen)
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
	int oldEntry = selectedEntry;
	int index = 0;
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		bool select = false;
		entry->MouseClick(xmlData, sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)), mouseOnPage, select, maps.clicked);
		if (select)
		{
			if (index == oldEntry)
				oldEntry = -1;
			selectedEntry = index;
		}

		index++;
	}
	if (!maps.clicked && oldEntry == selectedEntry) 
		selectedEntry = -1;
	if(!(oldEntry == -1 && selectedEntry == -1))
		SwapEntry(oldEntry, selectedEntry);
}

bool UIPage::MapClick(UI& ui, XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	int index = 0;
	for (std::shared_ptr<MapBox> box : maps.mapBoxes)
	{
		if (UI::CheckMouseInBounds(mousePos, *box->border))
		{
			boxIndex = index;
			return true;
		}
		index++;
	}

	return false;
}

bool UIPage::ContinentClick(UI& ui, XMLData& xmlData, ContinentPanel& panel, sf::Vector2i mousePos, int& continentIndex)
{
	int index = 0;
	for (auto continent : panel.continents)
	{
		if (UI::CheckMouseInBounds(mousePos, *continent->box.rect))
		{
			continentIndex = index;
			return true;
		}
		index++;
	}

	return false;
}

void UIPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor, UIPageType pageType)
{
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
		scrollBar.MoveBar({ 0, contentSize });

	float topBoxY = numEntries ? entries[0]->shapes[0]->getPosition().y : scrollBar.currentScroll.y;
	if (scrollBar.currentScroll.y != topBoxY)
		input.scroll = scrollBar.currentScroll.y - topBoxY;

	showContinents.Update();

	for (std::shared_ptr<UIEntry> entry : entries)
	{
		entry->Update(xmlData, window, timePassed, input, showCursor);
	}

	if (input.del && selectedEntry != -1)
	{
		if (entries[selectedEntry]->selected)
		{
			DeleteEntry(xmlData, pageType, selectedEntry);
		}
		PositionEntries();
		selectedEntry = -1;		
	}
	if (input.tab)
	{
		int oldEntry = selectedEntry;
		input.shift ? selectedEntry-- : selectedEntry++;

		if (selectedEntry < 0)
			selectedEntry = entries.size() - 1;
		else if (selectedEntry >= entries.size())
			selectedEntry = 0;
		if (entries.size() == 0)
			selectedEntry = -1;
		SwapEntry(oldEntry, selectedEntry);
	}		
}

void UIPage::DeleteEntry(XMLData& xmlData, UIPageType pageType, int entry)
{
	entries[entry]->Unselect(true);
	xmlData.RemoveData(pageType, entries[entry]->xmlKey);
	entries.erase(entries.begin() + entry);
}

void UIPage::AddEntry(XMLData& xmlData, std::shared_ptr<UIEntry> entry)
{
	int numEntries = entries.size();
	float topBoxY = numEntries ? entries[0]->shapes[0]->getPosition().y : 10.0f;
	entry->CreateEntry(xmlData, topBoxY + contentSize);
	contentSize += std::dynamic_pointer_cast<sf::RectangleShape>(entry->shapes[(int)UIEntry::ShapeTypes::Border])->getSize().y + 6.0f;
	if (selectedEntry != -1) entries[selectedEntry]->Unselect();
	selectedEntry = entries.size();
	entries.push_back(entry);
	scrollBar.BarSize({ 0, contentSize });
	scrollBar.MoveBar({ 0, 10 + contentSize });
	scrollBar.Scroll({ 0, scrollBar.maxScroll.y * -1 });
}

void UIPage::SwapEntry(int previous, int future)
{
	if (previous >= 0)
	{
		entries[previous]->Unselect();
	}
	if (future >= 0)
	{
		entries[future]->Select();
		auto entryPos = entries[future]->shapes[(int)UIEntry::ShapeTypes::Border]->getPosition();
		scrollBar.Scroll({ 0, -entryPos.y + 20 });
	}
}

void UIPage::PositionEntries()
{
	contentSize = 10.0f;
	float pageTop = entries.size() ? entries[0]->shapes[(int)UIEntry::ShapeTypes::Border]->getPosition().y : 0.0f;
	for (auto entry : entries)
	{
		auto borderBox = std::dynamic_pointer_cast<sf::RectangleShape>(entry->shapes[(int)UIEntry::ShapeTypes::Border]);
		entry->MoveEntry({ 0, (pageTop + contentSize) - borderBox->getPosition().y });
		contentSize += borderBox->getSize().y + 6;
	}
}

void UIPage::SelectPage()
{
	PositionEntries();
}

void UIPage::UnselectPage()
{
	selectedEntry = -1;
	for (auto entry : entries)
	{
		entry->Unselect(true);
	}
}

//----------------------------------------------------------------

void UIEntry::Draw(sf::RenderWindow& window)
{
	for (std::shared_ptr<Button> butt : buttons)
	{
		butt->Draw(window);
	}
	for (std::shared_ptr<sf::Text> text : labels)
	{
		window.draw(*text);
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

void UIEntry::MouseClick(XMLData& xmlData, sf::Vector2i mousePos, bool mouseOnPage, 
	bool& select, bool mapClicked)
{
	if (shapes.size())
	{
		std::shared_ptr<sf::Shape> borderBox = shapes[(int)ShapeTypes::Border];		
		select = mouseOnPage && borderBox && UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds());
	}

	for (std::shared_ptr<TextBox> box : boxes)
	{
		box->Toggle(UI::CheckMouseInBounds(mousePos, box->box));
	}
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		bool temp;
		entry->MouseClick(xmlData, mousePos, mouseOnPage, temp, mapClicked);
	}
}

void UIEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput& input, bool showCursor)
{
	for (auto button : buttons)
	{
		button->Update();
	}
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

void UIEntry::Unselect(bool white)
{
	selected = false;
	shapes[0]->setOutlineThickness(2.0f);
	shapes[0]->setOutlineColor(white ? sf::Color::White : baseColor);
}

void UIEntry::Toggle(bool toggle)
{
	toggle ? Select() : Unselect();
}