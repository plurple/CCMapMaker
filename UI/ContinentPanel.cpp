#include "ContinentPanel.h"
#include"UI.h"

ContinentPanel::ContinentPanel() :
	panel{ {592, 200} },
    closeButton{ {UI::windowSize.x - 38.0f, 8.0f}, {30, 30}, "X"},
    showPanel{false}
{
    panel.setPosition({ 1004,4 });
    panel.setFillColor({ 192, 192, 192 });
    panel.setOutlineThickness(2.0f);
    panel.setOutlineColor(sf::Color::Blue);
}

void ContinentPanel::Draw(sf::RenderWindow& window)
{
    if (showPanel)
    {
        window.draw(panel);
        closeButton.Draw(window);
        for (auto continent : continents)
        {
            continent->Draw(window);
        }
    }
}
