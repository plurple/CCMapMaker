#include "TerritoryPage.h"
#include "UI.h"

TerritoryPage::TerritoryPage(sf::Vector2f tabPos, 
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize):
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	selectedView{ TerritoryView::Borders },
	linkCoordinates({ 1296, 260 }, { 240, 30 }, "Link Coordinates", true)
{
	Button borders({ 1066, 170 }, { 115, 30 }, "Borders", true);
	territoryViews.push_back(borders);
	Button bombardments({ 1201, 170 }, { 215, 30 }, "Bombardments");
	territoryViews.push_back(bombardments);
	Button conditions({ 1066, 215 }, { 150, 30 }, "Conditions");
	territoryViews.push_back(conditions);
	Button extras({ 1436, 170 }, { 100, 30 }, "Extras");
	territoryViews.push_back(extras);

	addEntry.SetPosition({ 1066, 260 });
	addEntry.rect.setSize({ 190, 30 });
	addEntry.label->setString("Add Territory");

	showContinents.SetPosition({ 1296, 215 });
}

void TerritoryPage::Draw(sf::RenderWindow& window, bool selected)
{
	UIPage::Draw(window, selected);
	if (selected)
	{
		if(selectedView == TerritoryView::Conditions)
			showContinents.Draw(window);
		linkCoordinates.Draw(window);
		for (int i = 0; i < (int)TerritoryView::NumViews; i++)
		{
			territoryViews[i].Draw(window);
		}
	}
}

void TerritoryPage::MouseClick(sf::RenderWindow& window, sf::Vector2i mousePos)
{
	UIPage::MouseClick(window, mousePos);
	if (UI::CheckMouseInBounds(mousePos, linkCoordinates.rect))
	{
		linkCoordinates.Toggle();
	}
	if (selectedView == TerritoryView::Conditions)
	{
		if (UI::CheckMouseInBounds(mousePos, showContinents.rect))
		{
			showContinents.Toggle();
			//TODO open continents page
			//TODO unselect show continent on tab change
		}
	}
	else
	{
		showContinents.Unselect();
		//TODO close continents page
	}
		
	if (UI::CheckMouseInBounds(mousePos, addEntry.rect))
	{
		AddTerritory();
	}
	for (int i = 0; i < (int)TerritoryView::NumViews; i++)
	{
		if (UI::CheckMouseInBounds(mousePos, territoryViews[i].rect))
		{
			territoryViews[(int)selectedView].Toggle();
			selectedView = (TerritoryView)i;
			territoryViews[(int)selectedView].Toggle();
			SwapView();
		}
	}	
}

void TerritoryPage::Update(sf::RenderWindow& window, sf::Time timePassed, 
	UserInput input, bool showCursor)
{
	UIPage::Update(window, timePassed, input, showCursor);
}


void TerritoryPage::AddTerritory()
{
	TerritoryEntry* entry = new TerritoryEntry{ selectedView };
	UIPage::AddEntry(entry);
}

void TerritoryPage::SwapView()
{
	/*TODO change the colour of the labels to make them visible or not
	also move the positions and size of the box and such*/
	for (UIEntry* entry : entries)
	{
		dynamic_cast<TerritoryEntry*>(entry)->SwapView(selectedView);
	}
}

//-----------------------------------------------------------

void TerritoryEntry::CreateEntry(float entryTop)
{
	sf::RectangleShape* border = new sf::RectangleShape{ { 580,200 } };/*size*/
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color(192, 192, 192, 0));
	border->setOutlineThickness(2.0f);
	border->setOutlineColor({ 150, 60, 255 });
	shapes.push_back(border);

	sf::Text* nameLabel = new sf::Text(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);

	sf::Text* coordinateLabel = new sf::Text(UI::font, "Coordinates:");
	coordinateLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(coordinateLabel);

	sf::Text* smallLabel = new sf::Text(UI::font, "Small:");
	smallLabel->setPosition({ 200, entryTop + 46 });
	labels.push_back(smallLabel);

	sf::Text* xSmallLabel = new sf::Text(UI::font, "x:");
	xSmallLabel->setPosition({ 290, entryTop + 44 });
	labels.push_back(xSmallLabel);

	sf::Text* ySmallLabel = new sf::Text(UI::font, "y:");
	ySmallLabel->setPosition({ 400, entryTop + 44 });
	labels.push_back(ySmallLabel);

	sf::Text* largeLabel = new sf::Text(UI::font, "Large:");
	largeLabel->setPosition({ 200, entryTop + 84 });
	labels.push_back(largeLabel);

	sf::Text* xLargeLabel = new sf::Text(UI::font, "X:");
	xLargeLabel->setPosition({ 290, entryTop + 84 });
	labels.push_back(xLargeLabel);

	sf::Text* yLargeLabel = new sf::Text(UI::font, "Y:");
	yLargeLabel->setPosition({ 400, entryTop + 84 });
	labels.push_back(yLargeLabel);

	sf::Text* connectionLabel = new sf::Text(UI::font, "Territories:");
	connectionLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(connectionLabel);

	sf::Text* conditionLabel = new sf::Text(UI::font, "Condition:");
	conditionLabel->setPosition({ 20, entryTop + 156 });
	labels.push_back(conditionLabel);

	sf::Text* neutralLabel = new sf::Text(UI::font, "Neutral:");
	neutralLabel->setPosition({ 200, entryTop + 120 });
	labels.push_back(neutralLabel);

	sf::Text* bonusLabel = new sf::Text(UI::font, "Bonus:");
	bonusLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(bonusLabel);

	sf::Text* territory = new sf::Text(UI::font, "terr");
	territory->setPosition({ 170, entryTop + 120 });
	territories.push_back(territory);

	sf::Text* condition = new sf::Text(UI::font, "con");
	condition->setPosition({ 170, entryTop + 156 });
	conditions.push_back(condition);

	sf::Text* bombardment = new sf::Text(UI::font, "bomb");
	bombardment->setPosition({ 250, entryTop + 120 });
	bombardments.push_back(bombardment);

	Button* killer = new Button({ 390, entryTop + 124 }/*position*/, { 100, 30 }/*size*/, "Killer");
	buttons.push_back(killer);

	TextBox* nameBox = new TextBox({ 120, entryTop + 12 }/*position*/, { 450, 30 }/*size*/, "");
	boxes.push_back(nameBox);

	TextBox* xSmallBox = new TextBox({ 330, entryTop + 50 }/*position*/, { 50, 30 }/*size*/, "0");
	boxes.push_back(xSmallBox);

	TextBox* ySmallBox = new TextBox({ 440, entryTop + 50 }/*position*/, { 50, 30 }/*size*/, "0");
	boxes.push_back(ySmallBox);

	TextBox* xLargeBox = new TextBox({ 330, entryTop + 88 }/*position*/, { 50, 30 }/*size*/, "0");
	boxes.push_back(xLargeBox);

	TextBox* yLargeBox = new TextBox({ 440, entryTop + 88 }/*position*/, { 50, 30 }/*size*/, "0");
	boxes.push_back(yLargeBox);

	TextBox* neutralBox = new TextBox({ 330, entryTop + 124 }/*position*/, { 50, 30 }/*size*/, "");
	boxes.push_back(neutralBox);

	TextBox* bonusBox = new TextBox({ 120, entryTop + 124 }/*position*/, { 50, 30 }/*size*/, "");
	boxes.push_back(bonusBox);

	SwapView(selectedView);
}

void TerritoryEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	if (selectedView == TerritoryView::Bombardments)
	{
		for (int i = 0; i < bombardments.size(); i++)
		{
			window.draw(*bombardments[i]);
		}
	}
	else if (selectedView != TerritoryView::Extras)
	{
		for (int i = 0; i < territories.size(); i++)
		{
			window.draw(*territories[i]);
			if (selectedView == TerritoryView::Conditions)
			{
				window.draw(*conditions[i]);
			}
		}
	}
}	

void TerritoryEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage)
{
	UIEntry::MouseClick(mousePos, mouseOnPage);
	sf::Shape* borderBox = shapes[(int)ShapeTypes::Border];
	if (mouseOnPage && borderBox)
	{
		if (UI::CheckMouseInBounds(mousePos, borderBox->getGlobalBounds()))
		{
			selected = true;
			borderBox->setOutlineThickness(4.0f);
			borderBox->setOutlineColor({ 120, 0, 255 });
			//TODO be able to pick a territory from the map
		}
		else
		{
			selected = false;
			borderBox->setOutlineThickness(2.0f);
			borderBox->setOutlineColor({ 150, 60, 255 });
		}
	}

	Button* killer = buttons[(int)ButtonTypes::Killer];
	if (killer && mouseOnPage && selectedView == TerritoryView::Extras && 
		UI::CheckMouseInBounds(mousePos, killer->rect))
	{
		killer->Toggle();
	}

	TextBox* neutralBox = boxes[(int)BoxTypes::NeutralBox];
	if(neutralBox) neutralBox->active &= selectedView == TerritoryView::Extras;
	TextBox* bonusBox = boxes[(int)BoxTypes::BonusBox];
	if(bonusBox) bonusBox->active &= selectedView == TerritoryView::Extras;
		
}

void TerritoryEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
}

void TerritoryEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);

	for (sf::Text* territory : territories)
	{
		territory->move(offset);
	}
	for (sf::Text* condition : conditions)
	{
		condition->move(offset);
	}
	for (sf::Text* bomb : bombardments)
	{
		bomb->move(offset);
	}
}

void TerritoryEntry::SwapView(TerritoryView view)
{
	selectedView = view;
	float bombardment = selectedView == TerritoryView::Bombardments;
	float extra = selectedView == TerritoryView::Extras;
	float condition = selectedView == TerritoryView::Conditions;

	labels[(int)LabelTypes::ConnectionLabel]->setString(bombardment ? "Bombardment:" : "Territory:");
	labels[(int)LabelTypes::ConditionLabel]->setScale({ condition,condition });
	labels[(int)LabelTypes::BonusLabel]->setScale({ extra,extra });
	labels[(int)LabelTypes::NeutralLabel]->setScale({ extra,extra });
	labels[(int)LabelTypes::ConnectionLabel]->setScale({ (float)!extra,(float)!extra});
	buttons[(int)ButtonTypes::Killer]->Hide(extra);
	boxes[(int)BoxTypes::BonusBox]->Hide(extra);
	boxes[(int)BoxTypes::NeutralBox]->Hide(extra);
}