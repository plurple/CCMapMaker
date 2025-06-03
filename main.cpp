#include <SFML/Graphics.hpp>
#include <iostream>
#include "UI/UI.h"
#include "UI/TextBox.h"
#include "UserInput.h"
#include "XML/XMLData.h"

int main()
{
    XMLData xmlData;
    UI ui(xmlData);
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(UI::windowSize), "CC Map Maker");

    while (window.isOpen())
    {
        UserInput input;
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
            {
                input.verticle = mouseWheelScrolled->wheel == sf::Mouse::Wheel::Vertical;
                input.scroll = mouseWheelScrolled->delta;
            }
            if (const auto* TextEntered = event->getIf<sf::Event::TextEntered>())
            {
                if (std::isprint(TextEntered->unicode))
                    input.keyPressed += TextEntered->unicode;
            }
            if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Backspace)
                {
                    input.backSpace = true;
                }
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Enter)
                {
                    input.enter = true;
                }
            }
        }

        static sf::Time mouse_effect_time;

        sf::Time clockElapsed = clock.restart();
        mouse_effect_time += clockElapsed;

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (mouse_effect_time > sf::seconds(0.3f) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            //TODO add a hover effect to buttons :D
            mouse_effect_time = sf::Time::Zero;
            ui.MouseClick(xmlData, window, mousePos);
        }

        ui.Update(window, clockElapsed, input);

        window.clear(); 
        ui.Draw(window);
        window.display();
    }
}