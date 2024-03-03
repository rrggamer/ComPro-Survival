#ifndef RESTARTBUTTON_H
#define RESTARTBUTTON_H

#include <SFML/Graphics.hpp>

class RestartButton
{
private:
    sf::RectangleShape button;
    sf::Text buttonText;
    sf::Font font;
    bool isClicked;

public:
    RestartButton(float x, float y, float width, float height);

    // Set the text to display on the button
    void setText(const std::string& text);

    // Draw the button to a render target
    void draw(sf::RenderTarget& target);

    // Check if the button is clicked
    bool checkClick(const sf::Vector2f& mousePos);

    // Check if the button is currently clicked
    bool clicked() const;
};
#endif 
