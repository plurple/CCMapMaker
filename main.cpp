#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode({ 1500, 1000 }), "SFML works!");
    sf::RectangleShape sizeBox({ 50,50 });
    sizeBox.setPosition({ 1200,100 });
    sizeBox.setFillColor(sf::Color::Black);
    sizeBox.setOutlineThickness(2.0f);
    sizeBox.setOutlineColor(sf::Color::White);

    sf::RectangleShape textBox({ 400,32 });
    textBox.setPosition({ 1000,155 });
    textBox.setFillColor(sf::Color::Black);
    textBox.setOutlineThickness(2.0f);
    textBox.setOutlineColor(sf::Color::White);

    std::string input_text;
    sf::Font font;
    font.openFromFile("C:/Windows/Fonts/Arial.ttf");
    sf::Text text(font, " ");
    text.setPosition({ 1000, 150 });

    sf::Texture large;
    if (!large.loadFromFile("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsLarge4.17a.png"))
    {

    }
    sf::Sprite spriteL(large);
    sf::Texture small;
    if (!small.loadFromFile("C:/Users/Matthew/Documents/Conquer club/map xml/TemplarsSmall4.17.png"))
    {

    }
    sf::Sprite spriteS(small);

    bool largeSprite = true;
    float delay = 0.0f;
    bool textActive = false;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (textActive)
            {
                if (const auto* TextEntered = event->getIf<sf::Event::TextEntered>())
                {
                    if (std::isprint(TextEntered->unicode))
                        input_text += TextEntered->unicode;
                }
                if (const auto* KeyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (KeyPressed->scancode == sf::Keyboard::Scancode::Backspace)
                    {
                        if (!input_text.empty())
                            input_text.pop_back();
                    }
                    if (KeyPressed->scancode == sf::Keyboard::Scancode::Enter)
                    {
                        textActive = false;
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
        if (!textActive) show_cursor = false;

        text.setString(input_text + (show_cursor ? '_' : ' '));

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        if (mouse_effect_time > sf::seconds(0.3f) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
            localPosition.x > sizeBox.getPosition().x &&
            localPosition.x < sizeBox.getPosition().x + sizeBox.getSize().x &&
            localPosition.y > sizeBox.getPosition().y &&
            localPosition.y < sizeBox.getPosition().y + sizeBox.getSize().y)
        {
            largeSprite = !largeSprite;
            mouse_effect_time = sf::Time::Zero;
        }
        if (mouse_effect_time > sf::seconds(0.3f) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
            localPosition.x > textBox.getPosition().x &&
            localPosition.x < textBox.getPosition().x + textBox.getSize().x &&
            localPosition.y > textBox.getPosition().y &&
            localPosition.y < textBox.getPosition().y + textBox.getSize().y)
        {
            textActive = true;
            mouse_effect_time = sf::Time::Zero;
        }

        window.clear(); 
        window.draw(largeSprite ? spriteL : spriteS);
        window.draw(sizeBox);
        window.draw(textBox);
        window.draw(text);
        window.display();
    }
}