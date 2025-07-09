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

    bool focused = true;


    while (window.isOpen())
    {
        UserInput input;
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::FocusLost>())
            {
                focused = false;
            }
            if (event->is<sf::Event::FocusGained>())
            {
                focused = true;
            }
            
            if (focused)
            {
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
                    input.backSpace = KeyPressed->scancode == sf::Keyboard::Scancode::Backspace;
                    input.enter = KeyPressed->scancode == sf::Keyboard::Scancode::Enter;
                    input.del = KeyPressed->scancode == sf::Keyboard::Scancode::Delete;
                    input.right = KeyPressed->scancode == sf::Keyboard::Scancode::Right;
                    input.left = KeyPressed->scancode == sf::Keyboard::Scancode::Left;
                    input.up = KeyPressed->scancode == sf::Keyboard::Scancode::Up;
                    input.down = KeyPressed->scancode == sf::Keyboard::Scancode::Down;
                    input.shift = KeyPressed->shift;
                    input.ctrl = KeyPressed->control;
                    input.alt = KeyPressed->alt;
                    input.tab = KeyPressed->scancode == sf::Keyboard::Scancode::Tab;
                }
            }
        }

        if (focused)
        {
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

            ui.Update(xmlData, window, clockElapsed, input);

            window.clear();
            ui.Draw(window);
            window.display();
        }
    }
}