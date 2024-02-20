#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Vam Clone");


    sf::RectangleShape birdbox(sf::Vector2f(50, 50));
    birdbox.setPosition(100, 100);

    sf::Texture pngbird;
    pngbird.loadFromFile("bird.png");

    if (!pngbird.loadFromFile("bird.png")) {
        
        return EXIT_FAILURE;
    }

    birdbox.setTexture(&pngbird);


    float playerSpeed = 200.f; 

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dtSeconds = deltaTime.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += 1.f;

        
        if (movement != sf::Vector2f(0.f, 0.f)) {
            float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length;
        }

        
        birdbox.move(movement * playerSpeed * dtSeconds);

        window.clear();
        window.draw(birdbox);
        window.display();
    }

    return 0;
}