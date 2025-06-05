#include "TerritoryPage.h"
#include "UI.h"
#include "../XML/Territory.h"

TerritoryPage::TerritoryPage(XMLData& xmlData, sf::Vector2f tabPos,
	sf::Vector2f tabSize, std::string tabLabel, sf::Vector2f buttonBoxSize):
	UIPage(tabPos, tabSize, tabLabel, buttonBoxSize),
	selectedView{ TerritoryView::Borders },
	linkCoordinates({ 1296, 260 }, { 240, 30 }, "Link Coordinates", true)
{
	std::shared_ptr<Button> borders = 
		std::make_shared<Button>(sf::Vector2f{ 1066, 170 }, 
			sf::Vector2f{ 115, 30 }, "Borders", true);
	territoryViews.push_back(borders);
	std::shared_ptr<Button> bombardments =
		std::make_shared<Button>(sf::Vector2f{ 1201, 170 }, 
			sf::Vector2f{ 215, 30 }, "Bombardments");
	territoryViews.push_back(bombardments);
	std::shared_ptr<Button> conditions =
		std::make_shared<Button>(sf::Vector2f{ 1066, 215 },
		sf::Vector2f{ 150, 30 }, "Conditions");
	territoryViews.push_back(conditions);
	std::shared_ptr<Button> extras =
		std::make_shared<Button>(sf::Vector2f{ 1436, 170 }, 
			sf::Vector2f{ 100, 30 }, "Extras");
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
			territoryViews[i]->Draw(window);
		}
	}
}

void TerritoryPage::MouseClick(XMLData& xmlData, sf::RenderWindow& window, 
	sf::Vector2i mousePos, Maps& maps)
{
	UIPage::MouseClick(xmlData, window, mousePos, maps);
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
		AddTerritory(xmlData, maps.AddMapBox({ 0, 0 }));
	}
	for (int i = 0; i < (int)TerritoryView::NumViews; i++)
	{
		if (UI::CheckMouseInBounds(mousePos, territoryViews[i]->rect))
		{
			territoryViews[(int)selectedView]->Toggle();
			selectedView = (TerritoryView)i;
			territoryViews[(int)selectedView]->Toggle();
			SwapView();
		}
	}	
}

bool TerritoryPage::MapClick(XMLData& xmlData, Maps& maps, sf::Vector2i mousePos)
{
	if (UIPage::MapClick(xmlData, maps, mousePos))
	{
		/*select this territory entry if none selected
		if one selected dependent on view type
		border- add the clicked territory to the borders
		bombardment- add to the bombardments
		condition- need to select a territory then add to condition
		extras- do nothing same if no condition territory selected*/
	}
	else
	{
		AddTerritory(xmlData, maps.AddMapBox(mousePos));
	}
	return true;
}

void TerritoryPage::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor, UIPageType pageType)
{
	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
}


void TerritoryPage::AddTerritory(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> mapBox)
{
	std::shared_ptr<TerritoryEntry> entry = 
		std::make_shared<TerritoryEntry>( selectedView, 
			xmlData.AddTerritory(), mapBox );
	UIPage::AddEntry(xmlData, entry);
}

void TerritoryPage::SwapView()
{
	/*TODO change the colour of the labels to make them visible or not
	also move the positions and size of the box and such*/
	for (std::shared_ptr<UIEntry> entry : entries)
	{
		std::dynamic_pointer_cast<TerritoryEntry>(entry)->SwapView(selectedView);
	}
}

//-----------------------------------------------------------

TerritoryEntry::~TerritoryEntry()
{
	mapBox->scale({ 0.0f, 0.0f });
}

void TerritoryEntry::CreateEntry(XMLData& xmlData, float entryTop)
{
	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{ 580,200 } );/*size*/
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor({ 150, 60, 255 });
	shapes.push_back(border);

	std::shared_ptr<sf::Text> nameLabel =
		std::make_shared<sf::Text>(UI::font, "Name:");
	nameLabel->setPosition({ 20, entryTop + 8 });
	labels.push_back(nameLabel);

	std::shared_ptr<sf::Text> coordinateLabel =
		std::make_shared<sf::Text>(UI::font, "Coordinates:");
	coordinateLabel->setPosition({ 20, entryTop + 46 });
	labels.push_back(coordinateLabel);

	std::shared_ptr<sf::Text> smallLabel =
		std::make_shared<sf::Text>(UI::font, "Small:");
	smallLabel->setPosition({ 200, entryTop + 46 });
	labels.push_back(smallLabel);

	std::shared_ptr<sf::Text> xSmallLabel =
		std::make_shared<sf::Text>(UI::font, "x:");
	xSmallLabel->setPosition({ 290, entryTop + 44 });
	labels.push_back(xSmallLabel);

	std::shared_ptr<sf::Text> ySmallLabel = 
		std::make_shared<sf::Text>(UI::font, "y:");
	ySmallLabel->setPosition({ 400, entryTop + 44 });
	labels.push_back(ySmallLabel);

	std::shared_ptr<sf::Text> largeLabel =
		std::make_shared<sf::Text>(UI::font, "Large:");
	largeLabel->setPosition({ 200, entryTop + 84 });
	labels.push_back(largeLabel);

	std::shared_ptr<sf::Text> xLargeLabel =
		std::make_shared<sf::Text>(UI::font, "X:");
	xLargeLabel->setPosition({ 290, entryTop + 84 });
	labels.push_back(xLargeLabel);

	std::shared_ptr<sf::Text> yLargeLabel =
		std::make_shared<sf::Text>(UI::font, "Y:");
	yLargeLabel->setPosition({ 400, entryTop + 84 });
	labels.push_back(yLargeLabel);

	std::shared_ptr<sf::Text> connectionLabel =
		std::make_shared<sf::Text>(UI::font, "Territories:");
	connectionLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(connectionLabel);

	std::shared_ptr<sf::Text> conditionLabel =
		std::make_shared<sf::Text>(UI::font, "Condition:");
	conditionLabel->setPosition({ 20, entryTop + 156 });
	labels.push_back(conditionLabel);

	std::shared_ptr<sf::Text> neutralLabel =
		std::make_shared<sf::Text>(UI::font, "Neutral:");
	neutralLabel->setPosition({ 200, entryTop + 120 });
	labels.push_back(neutralLabel);

	std::shared_ptr<sf::Text> bonusLabel =
		std::make_shared<sf::Text>(UI::font, "Bonus:");
	bonusLabel->setPosition({ 20, entryTop + 120 });
	labels.push_back(bonusLabel);

	std::shared_ptr<Territory> data = xmlData.territories.at(xmlKey);
	data->largePos = sf::Vector2i{ mapBox->getPosition() };

	std::shared_ptr<TextBox> nameBox =
		std::make_shared<TextBox>(sf::Vector2f{ 120, entryTop + 12 }/*position*/, 
			sf::Vector2f{ 450, 30 }/*size*/);
	nameBox->text = &data->name;
	boxes.push_back(nameBox);

	std::shared_ptr<TextBox> xSmallBox =
		std::make_shared<TextBox>(sf::Vector2f{ 330, entryTop + 50 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	xSmallBox->number = &data->smallPos.x;
	boxes.push_back(xSmallBox);

	std::shared_ptr<TextBox> ySmallBox =
		std::make_shared<TextBox>(sf::Vector2f{ 440, entryTop + 50 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	ySmallBox->number = &data->smallPos.y;
	boxes.push_back(ySmallBox);

	std::shared_ptr<TextBox> xLargeBox =
		std::make_shared<TextBox>(sf::Vector2f{ 330, entryTop + 88 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	xLargeBox->number = &data->largePos.x;
	boxes.push_back(xLargeBox);

	std::shared_ptr<TextBox> yLargeBox =
		std::make_shared<TextBox>(sf::Vector2f{ 440, entryTop + 88 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	yLargeBox->number = &data->largePos.y;
	boxes.push_back(yLargeBox);

	std::shared_ptr<TextBox> neutralBox =
		std::make_shared<TextBox>(sf::Vector2f{ 330, entryTop + 124 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	neutralBox->number = &data->neutral;
	boxes.push_back(neutralBox);

	std::shared_ptr<TextBox> bonusBox =
		std::make_shared<TextBox>(sf::Vector2f{ 120, entryTop + 124 }/*position*/,
			sf::Vector2f{ 50, 30 }/*size*/);
	bonusBox->number = &data->bonus;
	boxes.push_back(bonusBox);

	std::shared_ptr<Button> killer = 
		std::make_shared<Button>(sf::Vector2f{ 390, entryTop + 124 }/*position*/, 
			sf::Vector2f{ 100, 30 }/*size*/, "Killer");
	killer->xmlLink = &data->killer;
	buttons.push_back(killer);

	//todo link these 3 to the correct list and right name etc.
	std::shared_ptr<sf::Text> territory = 
		std::make_shared<sf::Text>(UI::font, "terr");
	territory->setPosition({ 170, entryTop + 120 });
	territories.push_back(territory);

	std::shared_ptr<sf::Text> condition =
		std::make_shared<sf::Text>(UI::font, "con");
	condition->setPosition({ 170, entryTop + 156 });
	conditions.push_back(condition);

	std::shared_ptr<sf::Text> bombardment = 
		std::make_shared< sf::Text>(UI::font, "bomb");
	bombardment->setPosition({ 250, entryTop + 120 });
	bombardments.push_back(bombardment);

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
	std::shared_ptr<sf::Shape> borderBox = shapes[(int)ShapeTypes::Border];
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

	std::shared_ptr<Button> killer = buttons[(int)ButtonTypes::Killer];
	if (killer && mouseOnPage && selectedView == TerritoryView::Extras && 
		UI::CheckMouseInBounds(mousePos, killer->rect))
	{
		killer->Toggle();
	}

	std::shared_ptr<TextBox> neutralBox = boxes[(int)BoxTypes::NeutralBox];
	if(neutralBox) neutralBox->active &= selectedView == TerritoryView::Extras;
	std::shared_ptr<TextBox> bonusBox = boxes[(int)BoxTypes::BonusBox];
	if(bonusBox) bonusBox->active &= selectedView == TerritoryView::Extras;
		
}

void TerritoryEntry::Update(sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	UIEntry::Update(window, timePassed, input, showCursor);
	MoveEntry({ 0, input.scroll });
	mapBox->setPosition({ (float)(*boxes[(int)BoxTypes::LargeXBox]->number)-7, (float)(*boxes[(int)BoxTypes::LargeYBox]->number)-34 });
}

void TerritoryEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);

	for (std::shared_ptr<sf::Text> territory : territories)
	{
		territory->move(offset);
	}
	for (std::shared_ptr<sf::Text> condition : conditions)
	{
		condition->move(offset);
	}
	for (std::shared_ptr<sf::Text> bomb : bombardments)
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