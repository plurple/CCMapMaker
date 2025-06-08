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

bool TerritoryPage::MapClick(XMLData& xmlData, Maps& maps, sf::Vector2i mousePos, int& boxIndex)
{
	if (UIPage::MapClick(xmlData, maps, mousePos, boxIndex))
	{
		if(selectedEntry == -1)		
		{
			SwapEntry(selectedEntry, boxIndex);
			selectedEntry = boxIndex;
		}
		else
		{
			if (selectedEntry == boxIndex)
			{
				SwapEntry(selectedEntry, -1);
				selectedEntry = -1;
			}
			else
			{
				switch (selectedView)
				{
				case TerritoryView::Extras:
				case TerritoryView::Borders:
				{
					auto border = std::dynamic_pointer_cast<TerritoryEntry>(entries[selectedEntry]);
					int i;
					bool removed = false;
					for (i = 0; i < border->territories.size(); i++)
					{
						if (border->territories[i]->index == boxIndex)
						{
							maps.mapBoxes[boxIndex]->setOutlineColor(sf::Color::White);
							border->territories.erase(border->territories.begin() + i);
							border->conditions.erase(border->conditions.begin() + i);
							xmlData.territories.at(border->xmlKey)->borders.erase(xmlData.territories.at(border->xmlKey)->borders.begin() + i);
							removed = true;
							break;
						}
					}
					if (removed)
					{
						//move stuff;
					}
					else
					{
						maps.mapBoxes[boxIndex]->setOutlineColor(sf::Color::Blue);
						border->AddBorder(xmlData, maps, boxIndex, entries[boxIndex]->xmlKey);
					}

					break;
				}
				case TerritoryView::Bombardments:
				{
					auto bomb = std::dynamic_pointer_cast<TerritoryEntry>(entries[selectedEntry]);
					int i;
					bool removed = false;
					for (i = 0; i < bomb->bombardments.size(); i++)
					{
						if (bomb->bombardments[i]->index == boxIndex)
						{
							maps.mapBoxes[boxIndex]->setOutlineColor(sf::Color::White);
							bomb->bombardments.erase(bomb->bombardments.begin() + i);
							xmlData.territories.at(bomb->xmlKey)->bombardments.erase(xmlData.territories.at(bomb->xmlKey)->bombardments.begin() + i);
							removed = true;
							break;
						}
					}
					if (removed)
					{
						//move stuff;
					}
					else
					{
						maps.mapBoxes[boxIndex]->setOutlineColor(sf::Color::Green);
						bomb->AddBombardment(xmlData, maps, boxIndex, entries[boxIndex]->xmlKey);
					}
					break;
				}
				case TerritoryView::Conditions:
				{
					auto cond = std::dynamic_pointer_cast<TerritoryEntry>(entries[selectedEntry]);
					cond->AddCondition(xmlData, maps, boxIndex, entries[boxIndex]->xmlKey);					
					break;
				}
				}
			}
		}
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
	if (*input.keyPressed.c_str() == 'x' && (selectedEntry == -1 || !entries[selectedEntry]->boxes[(int)TerritoryEntry::BoxTypes::NameBox]->active))
	{
		linkCoordinates.Toggle();
	}

	UIPage::Update(xmlData, window, timePassed, input, showCursor, pageType);
}


void TerritoryPage::AddTerritory(XMLData& xmlData, std::shared_ptr<sf::RectangleShape> mapBox)
{
	std::shared_ptr<TerritoryEntry> entry = 
		std::make_shared<TerritoryEntry>( selectedView, 
			xmlData.AddTerritory(), mapBox );
	entry->linkedCoords = &linkCoordinates.selected;
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
	baseColor = sf::Color{ 150, 60, 255 };
	selectedColor = sf::Color{ 120, 0, 255 };

	std::shared_ptr<sf::RectangleShape> border = 
		std::make_shared<sf::RectangleShape>( sf::Vector2f{ 530,200 } );/*size*/
	border->setPosition({ 10,entryTop });
	border->setFillColor(sf::Color::Transparent);
	border->setOutlineThickness(2.0f);
	border->setOutlineColor(baseColor);
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
	sf::Vector2f mapBoxPos = mapBox->getPosition();
	data->largePos = UI::isLarge ? sf::Vector2i{ mapBoxPos } : sf::Vector2i{ Maps::ConvertSmall(mapBoxPos.x, true), Maps::ConvertSmall(mapBoxPos.y, false) };
	data->smallPos = UI::isLarge ? sf::Vector2i{ Maps::ConvertSmall(mapBoxPos.x, true), Maps::ConvertSmall(mapBoxPos.y, false)} : sf::Vector2i{ mapBoxPos };

	std::shared_ptr<TextBox> nameBox =
		std::make_shared<TextBox>(sf::Vector2f{ 120, entryTop + 12 }/*position*/, 
			sf::Vector2f{ 400, 30 }/*size*/);
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

	territoriesPos = { 170, entryTop + 120 };
	conditionsPos = { 370, entryTop + 156 };
	bombardmentsPos = { 250, entryTop + 120 };

	SwapView(selectedView);
	Select();
}

void TerritoryEntry::Draw(sf::RenderWindow& window)
{
	UIEntry::Draw(window);
	if (selectedView == TerritoryView::Bombardments)
	{
		for (int i = 0; i < bombardments.size(); i++)
		{
			bombardments[i]->nameLabel->Draw(window);
		}
	}
	else if (selectedView != TerritoryView::Extras)
	{
		for (int i = 0; i < territories.size(); i++)
		{
			territories[i]->nameLabel->Draw(window);
			if (selectedView == TerritoryView::Conditions && i < conditions.size())
			{
				if(conditions[i]->index != -1)
					conditions[i]->nameLabel->Draw(window);
			}
		}
	}
}	

void TerritoryEntry::MouseClick(sf::Vector2i mousePos, bool mouseOnPage, bool& select)
{
	UIEntry::MouseClick(mousePos, mouseOnPage, select);

	std::shared_ptr<Button> killer = buttons[(int)ButtonTypes::Killer];
	if (killer && mouseOnPage && selectedView == TerritoryView::Extras && 
		UI::CheckMouseInBounds(mousePos, killer->rect))
	{
		killer->Toggle();
	}

	//the &= is so that they are only active on the extras view
	std::shared_ptr<TextBox> neutralBox = boxes[(int)BoxTypes::NeutralBox];
	if(neutralBox) neutralBox->active &= selectedView == TerritoryView::Extras;
	std::shared_ptr<TextBox> bonusBox = boxes[(int)BoxTypes::BonusBox];
	if(bonusBox) bonusBox->active &= selectedView == TerritoryView::Extras;	

	if (selectedView == TerritoryView::Bombardments)
	{
		for (int i = 0; i < bombardments.size(); i++)
		{
			bombardments[i]->nameLabel->active = mouseOnPage && UI::CheckMouseInBounds(mousePos, bombardments[i]->nameLabel->box);
		}
	}
	else if (selectedView != TerritoryView::Extras)
	{
		for (int i = 0; i < territories.size(); i++)
		{			
			//todo figure out how to not unselect the territory on condition selection
			territories[i]->nameLabel->active = mouseOnPage && UI::CheckMouseInBounds(mousePos, territories[i]->nameLabel->box);
			if (selectedView == TerritoryView::Conditions && i < conditions.size())
			{
				conditions[i]->nameLabel->active = mouseOnPage && UI::CheckMouseInBounds(mousePos, conditions[i]->nameLabel->box);
			}
		}
	}
}

void TerritoryEntry::Update(XMLData& xmlData, sf::RenderWindow& window, sf::Time timePassed,
	UserInput input, bool showCursor)
{
	int& smallxRef = *boxes[(int)BoxTypes::SmallXBox]->number;
	int& largexRef = *boxes[(int)BoxTypes::LargeXBox]->number;
	int& smallyRef = *boxes[(int)BoxTypes::SmallYBox]->number;
	int& largeyRef = *boxes[(int)BoxTypes::LargeYBox]->number;
	int smallx = smallxRef;
	int largex = largexRef;
	int smally = smallyRef;
	int largey = largeyRef;
	if (selected)
	{
		if (input.right)
			UI::isLarge ? largexRef++ : smallxRef++;
		if (input.left)
			UI::isLarge ? largexRef-- : smallxRef--;
		if (input.up)
			UI::isLarge ? largeyRef-- : smallyRef--;
		if (input.down)
			UI::isLarge ? largeyRef++ : smallyRef++;
	}

	UIEntry::Update(xmlData, window, timePassed, input, showCursor);

	if (selectedView == TerritoryView::Bombardments)
	{
		for (int i = 0; i < bombardments.size(); i++)
		{
			if (xmlData.territories.find(bombardments[i]->xmlKey) != xmlData.territories.end())
			{
				bombardments[i]->nameLabel->Update(window, timePassed, input, showCursor);
			}
			else
			{
				if (bombardments.size()) bombardments.erase(bombardments.begin() + i);
				i--;
			}

		}
	}
	else if (selectedView != TerritoryView::Extras)
	{
		for (int i = 0; i < territories.size(); i++)
		{
			if (xmlData.territories.find(territories[i]->xmlKey) != xmlData.territories.end())
			{
				territories[i]->nameLabel->Update(window, timePassed, input, showCursor);
				if (selectedView == TerritoryView::Conditions && i < conditions.size() && conditions[i]->xmlKey != -1)
				{
					if (xmlData.territories.find(conditions[i]->xmlKey) != xmlData.territories.end())
					{
						conditions[i]->nameLabel->Update(window, timePassed, input, showCursor);
					}
					else
					{
						RemoveCondition(xmlData, i);
					}
				}
			}
			else
			{
				if(territories.size()) territories.erase(territories.begin() + i);
				if(conditions.size()) conditions.erase(conditions.begin() + i);
				i--;
			}
		}
	}

	MoveEntry({ 0, input.scroll });
	if (*linkedCoords)
	{
		if (smallx != smallxRef)
		{
			smallx = smallxRef;
			largex = Maps::ConvertLarge(smallx, true);
			largexRef = largex;
		}
		else if (largex != largexRef)
		{
			largex = largexRef;
			smallx = Maps::ConvertSmall(largex, true);
			smallxRef = smallx;
		}
		else if (smally != smallyRef)
		{
			smally = smallyRef;
			largey = Maps::ConvertLarge(smally, false);
			largeyRef = largey;
		}
		else if (largey != largeyRef)
		{
			largey = largeyRef;
			smally = Maps::ConvertSmall(largey, false);
			smallyRef = smally;
		}
	}

	if (UI::isLarge)
		mapBox->setPosition({ (float)(largex) - 7, (float)(largey) - 34 });
	else
		mapBox->setPosition({ (float)(smallx) - 7, (float)(smally) - 34 });
}

void TerritoryEntry::MoveEntry(sf::Vector2f offset)
{
	UIEntry::MoveEntry(offset);

	territoriesPos += offset;
	conditionsPos += offset;
	bombardmentsPos += offset;

	for (std::shared_ptr<BorderEntry> territory : territories)
	{
		territory->nameLabel->Move(offset);
	}
	for (std::shared_ptr<BorderEntry> condition : conditions)
	{
		condition->nameLabel->Move(offset);
	}
	for (std::shared_ptr<BorderEntry> bomb : bombardments)
	{
		bomb->nameLabel->Move(offset);
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

void TerritoryEntry::Select()
{
	UIEntry::Select();
	mapBox->setOutlineColor(sf::Color::Red);
	if (selectedView == TerritoryView::Bombardments)
	{
		for (auto border : bombardments)
		{
			border->mapBox->setOutlineColor(sf::Color::Green);
		}
	}
	else if (selectedView != TerritoryView::Extras)
	{
		for (auto border : territories)
		{
			border->mapBox->setOutlineColor(sf::Color::Blue);
		}
	}
	//todo colour the map boxes properly for conditions.
}

void TerritoryEntry::Unselect()
{
	UIEntry::Unselect();
	mapBox->setOutlineColor(sf::Color::White);
	for (auto border : territories)
	{
		border->mapBox->setOutlineColor(sf::Color::White);
	}
	for (auto border : bombardments)
	{
		border->mapBox->setOutlineColor(sf::Color::White);
	}
}

void TerritoryEntry::AddBorder(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey)
{
	std::shared_ptr<BorderEntry> border = std::make_shared<BorderEntry>();
	border->nameLabel = std::make_shared<TextBox>(territoriesPos + sf::Vector2f{ 0, territories.size() * 25.0f }, 
		sf::Vector2f{ 200, 20 });
	border->nameLabel->displayText->setCharacterSize(20);
	border->index = boxIndex;
	border->mapBox = maps.mapBoxes[boxIndex];
	border->xmlKey = otherXMLKey;
	border->nameLabel->text = &xmlData.territories[otherXMLKey]->name;
	territories.push_back(border);	
	BorderData borderData;
	borderData.territory = otherXMLKey;
	xmlData.territories[xmlKey]->borders.push_back(borderData);
	std::shared_ptr<BorderEntry> condition = std::make_shared<BorderEntry>();
	condition->nameLabel = std::make_shared<TextBox>(conditionsPos + sf::Vector2f{ 0, conditions.size() * 25.0f },
		sf::Vector2f{ 200, 20 });
	condition->nameLabel->displayText->setCharacterSize(20);
	conditions.push_back(condition);
}

void TerritoryEntry::AddBombardment(XMLData& xmlData, Maps& maps, int boxIndex, int otherXMLKey)
{
	std::shared_ptr<BorderEntry> bomb = std::make_shared<BorderEntry>();
	bomb->nameLabel = std::make_shared<TextBox>(bombardmentsPos + sf::Vector2f{ 0, bombardments.size() * 25.0f },
		sf::Vector2f{ 200, 20 });
	bomb->nameLabel->displayText->setCharacterSize(20);
	bomb->index = boxIndex;
	bomb->mapBox = maps.mapBoxes[boxIndex];
	bomb->xmlKey = otherXMLKey;
	bomb->nameLabel->text = &xmlData.territories[otherXMLKey]->name;
	bombardments.push_back(bomb);
	xmlData.territories[xmlKey]->bombardments.push_back(otherXMLKey);
}

void TerritoryEntry::AddCondition(XMLData& xmlData, Maps& maps, int boxIndex, 
	int otherXMLKey)
{
	int borderIndex = 0;
	for (auto border : territories)
	{
		if (border->nameLabel->active)
			break;
		borderIndex++;
	}
	if (borderIndex < conditions.size())
	{
		std::shared_ptr<BorderEntry> condition = conditions[borderIndex];

		if (condition->index == boxIndex)
		{
			RemoveCondition(xmlData, borderIndex);
		}
		else
		{
			condition->index = boxIndex;
			if(condition->mapBox) condition->mapBox->setOutlineColor(sf::Color::White);
			condition->mapBox = maps.mapBoxes[boxIndex];
			condition->mapBox->setOutlineColor(sf::Color::Magenta);
			condition->xmlKey = otherXMLKey;
			condition->isContinent = false;
			condition->nameLabel->text = &xmlData.territories[otherXMLKey]->name;
			xmlData.territories[xmlKey]->borders[borderIndex].condition = otherXMLKey;
			xmlData.territories[xmlKey]->borders[borderIndex].conditionIsContintent = false;
		}
	}
}

void TerritoryEntry::RemoveCondition(XMLData& xmlData, int borderIndex)
{
	std::shared_ptr<BorderEntry> condition = conditions[borderIndex];
	condition->index = -1;
	condition->xmlKey = -1;
	condition->mapBox->setOutlineColor(sf::Color::White);
	xmlData.territories[xmlKey]->borders[borderIndex].condition = -1;
	condition->mapBox = nullptr;
}