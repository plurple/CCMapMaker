#include "TextBox.h"
#include "UI.h"

TextBox::TextBox(sf::Vector2f pos, sf::Vector2f boxSize, 
    std::shared_ptr<std::string> defaultText, 
    std::shared_ptr<int> defaultNumber) :
	box{ boxSize },
	active{false},
	text{defaultText},
    number{defaultNumber}
{
	box.setPosition(pos);
	box.setFillColor(sf::Color::Black);
	box.setOutlineThickness(2.0f);
	box.setOutlineColor(sf::Color::White);

    std::string startText = text ? *text : (number ? std::to_string(*number) : "");
	displayText = std::make_shared<sf::Text>(UI::font, startText);
	displayText->setPosition({ pos.x, pos.y-5});
}

void TextBox::Update(sf::RenderWindow& window, sf::Time timePassed, 
    UserInput input, bool showCursor)
{
    if (active)
    {
        if (text)
        {
            *text += input.keyPressed;
            if (input.backSpace && !text->empty())
                text->pop_back();
        }
        else if (number)
        {
            if (IsNumber(*input.keyPressed.c_str()))
                AddNumber(input.keyPressed);
            if (input.backSpace)
                RemoveNumber();
        }
        if (input.enter)
            active = false;       
    }

    std::string currentText = "";
    if (text)
        currentText = *text;
    else if(number && *number != -1)
        currentText = std::to_string(*number);

    displayText->setString(currentText + (active && showCursor ? '|' : ' '));
}

void TextBox::Draw(sf::RenderWindow& window)
{
    window.draw(box);
    window.draw(*displayText);
}

void TextBox::Move(sf::Vector2f offset)
{
    box.move(offset);
    displayText->move(offset);
}

void TextBox::Hide(bool show)
{
    box.setScale({ (float)show,(float)show });
    displayText->setScale({ (float)show, (float)show });
}

bool TextBox::IsNumber(char key)
{
    return (int)key >= 48 && (int)key <= 57;
}

void TextBox::AddNumber(std::string key)
{
    std::string temp = *number == -1 ? "" : std::to_string(*number);
    temp += key;
    *number = std::stoi(temp);
    if (*number > 9999)
        *number = 9999;
}

void TextBox::RemoveNumber()
{
    if ((*number == -1) || (*number >= 0 && *number <= 9))
    {
        *number = -1;
    }
    else
    {
        std::string temp = std::to_string(*number);
        temp.pop_back();
        *number = std::stoi(temp);
    }
}