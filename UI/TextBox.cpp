#include "TextBox.h"
#include "UI.h"

TextBox::TextBox(sf::Vector2f pos, sf::Vector2f boxSize, 
    std::string* defaultText, int* defaultNumber) :
	box{ boxSize },
	active{false},
	text{defaultText},
    number{defaultNumber},
    baseColor{sf::Color::White},
    activeColor{ sf::Color::White },
    allowNegative{false}
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
    UserInput& input, bool showCursor)
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
            if (*input.keyPressed.c_str() == '-' && allowNegative)
                *number = -*number;
        }
        if (input.enter)
            Unselect();
    }
    else
        box.setOutlineColor(baseColor);

    std::string newText = "";
    if (text)
        newText = *text;
    else if(number && (*number != -1 || allowNegative))
        newText = std::to_string(*number);

    newText += (active && showCursor ? '|' : ' ');

    if (newText != lastDisplayedText)
    {
        displayText->setString(newText);
        lastDisplayedText = newText;
    }
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
    return (int)key >= '0' && (int)key <= '9';
}

void TextBox::AddNumber(std::string key)
{
    std::string temp = *number == -1 && !allowNegative ? "" : std::to_string(*number);
    temp += key;
    *number = std::stoi(temp);
    if (*number > 9999)
        *number = 9999;
    if (allowNegative && *number < -9999)
        *number = -9999;
}

void TextBox::RemoveNumber()
{
    if ((*number == -1) || (*number >= 0 && *number <= 9))
    {
        if (allowNegative)
            *number = 0;
        else
            *number = -1;
    }
    else
    {
        std::string temp = std::to_string(*number);
        temp.pop_back();
        if (temp == "-")
            temp = "0";
        *number = std::stoi(temp);
    }
}

void TextBox::Select()
{
    active = true;
    box.setOutlineThickness(3.5f);
    box.setOutlineColor(activeColor);
}

void TextBox::Unselect()
{
    active = false;
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(baseColor);
}

void TextBox::Toggle(bool toggle)
{
    toggle ? Select() : Unselect();
}