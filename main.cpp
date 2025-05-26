#include <SFML/Graphics.hpp>
#include <iostream>
#include "UI.h"
#include "TextBox.h"

int main()
{
    UI ui;
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode({ 1600, 1000 }), "SFML works!");

    TextBox textbox({1050, 155}, ui.font);
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (textbox.active)
            {
                if (const auto* TextEntered = event->getIf<sf::Event::TextEntered>())
                {
                    if (std::isprint(TextEntered->unicode))
                        textbox.text += TextEntered->unicode;
                }
                if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (KeyPressed->scancode == sf::Keyboard::Scancode::Backspace)
                    {
                        if (!textbox.text.empty())
                            textbox.text.pop_back();
                    }
                    if (KeyPressed->scancode == sf::Keyboard::Scancode::Enter)
                    {
                        textbox.active = false;
                    }
                }
            }
        }
        static sf::Time text_effect_time;
        static sf::Time mouse_effect_time;
        static bool show_cursor;

        sf::Time clockElapsed = clock.restart();
        text_effect_time += clockElapsed;
        mouse_effect_time += clockElapsed;

        if (text_effect_time >= sf::seconds(0.5f))
        {
            show_cursor = !show_cursor;
            text_effect_time = sf::Time::Zero;
        }
        if (!textbox.active) show_cursor = false;

        textbox.displayText->setString(textbox.text + (show_cursor ? '_' : ' '));

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        if (mouse_effect_time > sf::seconds(0.3f) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            mouse_effect_time = sf::Time::Zero;
            if(ui.CheckMouseInBounds(localPosition, ui.maps.mapCheckBox))
            {
                ui.maps.SwapMaps();
            }
            textbox.active = ui.CheckMouseInBounds(localPosition, textbox.box);
        }

        window.clear(); 
        ui.DrawUI(window);
        window.draw(textbox.box);
        window.draw(*textbox.displayText);
        window.display();
    }
}