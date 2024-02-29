#include "RestartButton.h"

RestartButton::RestartButton(float x, float y, float width, float height) : isClicked(false) {
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(sf::Color::Green); // Customize button color as needed

    if (!font.loadFromFile("Fonts/VCR_OSD_MONO_1.001.ttf")) {
        // Handle font loading error
    }

    buttonText.setFont(font);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
}

void RestartButton::setText(const std::string& text) {
    buttonText.setString(text);
    // Center the text within the button
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
    buttonText.setPosition(button.getPosition().x + button.getSize().x / 2.0f,
        button.getPosition().y + button.getSize().y / 2.0f);
}

void RestartButton::draw(sf::RenderTarget& target) {
    target.draw(button);
    target.draw(buttonText);
}

bool RestartButton::checkClick(const sf::Vector2f& mousePos) {
    if (button.getGlobalBounds().contains(mousePos)) {
        isClicked = true;
        return true;
    }
    return false;
}

bool RestartButton::clicked() const {
    return isClicked;
}