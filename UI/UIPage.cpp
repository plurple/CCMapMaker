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
	scrollBar(sf::View{ page.getGlobalBounds() }, { 555, 50 }/*position*/, { 30, page.getSize().y - 100 }/*size*/)
{
	page.setPosition({ 0, 0 });
	page.setFillColor(sf::Color(192, 192, 192, 0));
	page.setOutlineThickness(4.0f);
	page.setOutlineColor(sf::Color::White);

	scrollBar.scrollWindow = sf::View{ page.getGlobalBounds() };
	float bob = (page.getSize().y + 8.0f) / UI::windowSize.y;
	float bob2 = (page.getSize().x + 8.0f) / UI::windowSize.x;
	scrollBar.scrollWindow.setViewport(sf::FloatRect({ 1.0f - bob2, 1.0f-bob }, { bob2, bob }));
	scrollBar.minScroll.y = 10.0f;

	buttonBox.setPosition({ 1004, 154 });
	buttonBox.setFillColor(sf::Color(192, 192, 192, 0));
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
		for (UIEntry* entry : entries)
		{
			entry->Draw(window);
		}
		window.draw(page);
		scrollBar.Draw(window);
		window.setView(window.getDefaultView());
	}
}

void UIPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, sf::Vector2i mousePos)
{
	if (mouseOnPage)
	{
		scrollBar.MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)));
	}
	for (UIEntry* entry : entries)
	{
		entry->MouseClick(sf::Vector2i(window.mapPixelToCoords(mousePos, scrollBar.scrollWindow)), mouseOnPage);
	}
}

void UIPage::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	if (input.del)
	{
		for (UIEntry* entry : entries)
		{
			if (entry->selected)
			{
				//todo remove entry and all that entails.
			}
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

	for (UIEntry* entry : entries)
	{
		entry->Update(window, timePassed, input, showCursor);
	}
}

void UIPage::AddEntry(XMLData& xmlData, UIEntry* entry, int insertedKey)
{
	int numEntries = entries.size();
	float topBoxY = numEntries ? entries[0]->shapes[0]->getPosition().y : 0.0f;
	float boxSize = numEntries ? entries[0]->shapes[0]->getGlobalBounds().size.y : 0.0f;
	entry->CreateEntry(xmlData, topBoxY + (boxSize + 6) * numEntries, insertedKey);
	entries.push_back(entry);
	scrollBar.BarSize({ 0, (boxSize + 6) * (numEntries + 1) });
	scrollBar.MoveBar({ 0, 10 + (boxSize + 6) * (numEntries + 1) });
	scrollBar.Scroll({ 0, scrollBar.maxScroll.y * -1 });
}

//----------------------------------------------------------------

void UIEntry::Draw(sf::RenderWindow& window)
{
	for (sf::Text* text : labels)
	{
		window.draw(*text);
	}
	for (Button* butt : buttons)
	{
		butt->Draw(window);
	}
	for (TextBox* box : boxes)
	{
		box->Draw(window);
	}
	for (UIEntry* entry : entries)
	{
		entry->Draw(window);
	}
	for (sf::Shape* shape : shapes)
	{
		window.draw(*shape);
	}
}

void UIEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	for (TextBox* box : boxes)
	{
		box->active = UI::CheckMouseInBounds(mousePos, box->box);
	}
	for (UIEntry* entry : entries)
	{
		entry->MouseClick(mousePos, mouseOnPage);
	}
}

void UIEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	for (TextBox* box : boxes)
	{
		box->Update(window, timePassed, input, showCursor);
	}
	for (UIEntry* entry : entries)
	{
		entry->Update(window, timePassed, input, showCursor);
	}
}

void UIEntry::MoveEntry(sf::Vector2f offset)
{
	for (sf::Shape* shape : shapes)
	{
		shape->move(offset);
	}
	for (sf::Text* text : labels)
	{
		text->move(offset);
	}
	for (Button* butt : buttons)
	{
		butt->Move(offset);
	}
	for (TextBox* box : boxes)
	{
		box->Move(offset);
	}
	for (UIEntry* entry : entries)
	{
		entry->MoveEntry(offset);
	}
}