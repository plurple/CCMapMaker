#include <SFML/Graphics.hpp>
#include <iostream>
#include "UI/UI.h"
#include "UI/TextBox.h"

int main()
{
    UI ui;
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode({ 1600, 1000 }), "CC Map Maker");

    TextBox textbox({1050, 155}, ui.font);
    
    while (window.isOpen())
    {
        static sf::Time mouse_effect_time;

        sf::Time clockElapsed = clock.restart();
        mouse_effect_time += clockElapsed;

        textbox.Update(window, clockElapsed);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (mouse_effect_time > sf::seconds(0.3f) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            mouse_effect_time = sf::Time::Zero;
            ui.MouseClick(mousePos);
            textbox.active = ui.CheckMouseInBounds(mousePos, textbox.box);
        }

        window.clear(); 
        ui.Draw(window);
        textbox.Draw(window);
        window.display();
    }
}