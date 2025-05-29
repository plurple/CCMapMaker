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
	}
}

void TransformPage::MouseClick(sf::Vector2i mousePos)
{
    if (UI::CheckMouseInBounds(mousePos, addTransform.rect))
    {
        //TODO add a transform
    }
	if (UI::CheckMouseInBounds(mousePos, testTransforms.rect))
	{
		//TODO add a transform test function
	}
}

void TransformPage::Update(sf::RenderWindow& window, sf::Time timePassed,
    std::string keyPressed, bool backspace, bool enter, bool showCursor)
{

}