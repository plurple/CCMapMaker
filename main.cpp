#include <SFML/Graphics.hpp>
#include <iostream>
#include "UI/UI.h"
#include "UI/TextBox.h"

int main()
{
    UI ui;
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode({ 1600, 1000 }), "CC Map Maker");

    while (window.isOpen())
    {
        bool backspace = false, enter = false;
        std::string keyPressed;
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* TextEntered = event->getIf<sf::Event::TextEntered>())
            {
                if (std::isprint(TextEntered->unicode))
                    keyPressed += TextEntered->unicode;
            }
            if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Backspace)
                {
                    backspace = true;
                }
                if (KeyPressed->scancode == sf::Keyboard::Scancode::Enter)
                {
                    enter = true;
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
            ui.MouseClick(mousePos);
        }

        ui.Update(window, clockElapsed, keyPressed, backspace, enter);

        window.clear(); 
        ui.Draw(window);
        window.display();
    }
}