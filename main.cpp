#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::Clock clock;

    sf::RenderWindow window(sf::VideoMode({ 1500, 1000 }), "SFML works!");
    sf::RectangleShape shape({ 50,50 });
    shape.setPosition({ 1200,100 });
    shape.setFillColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::White);
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
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        if (clock.getElapsedTime().asSeconds() > 0.3f && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) &&
            localPosition.x > shape.getPosition().x &&
            localPosition.x < shape.getPosition().x + shape.getSize().x &&
            localPosition.y > shape.getPosition().y &&
            localPosition.y < shape.getPosition().y + shape.getSize().y)
        {
            largeSprite = !largeSprite;
            clock.restart();
        }

        window.clear(); 
        window.draw(largeSprite ? spriteL : spriteS);
        window.draw(shape);
        window.display();
    }
}