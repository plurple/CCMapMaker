#include <SFML/Graphics.hpp>
#include <iostream>
#include "UI/UI.h"
#include "UI/TextBox.h"
#include "UserInput.h"
#include "XML/XMLData.h"
#include <sstream>
#include <iomanip>

int main()
{
    XMLData xmlData;
    UI ui(xmlData);
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode(UI::windowSize), "CC Map Maker");

    bool focused = true;

    sf::Time dt;
    float fps = 0.0f;

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
                if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    input.mouseLeft = mouseButton->button == sf::Mouse::Button::Left;
                    input.mouseRight = mouseButton->button == sf::Mouse::Button::Right;
                }
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
            if (input.mouseLeft)
            {
                //TODO add a hover effect to buttons :D
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                ui.MouseClick(xmlData, window, mousePos);
            }

            ui.Update(xmlData, window, dt, input);
            
            //std::cout << "FPS: " << fps << "\n";
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << fps;
            sf::Text fpsCounter = sf::Text(ui.font, ss.str(), 15);
            fpsCounter.setPosition({ UI::windowSize.x *0.5f+100.0f, 10.0f });

            window.clear();
            ui.Draw(window);
            window.draw(fpsCounter);
            window.display();

            dt = clock.restart();
            fps = 1.f / dt.asSeconds();
        }
    }
}