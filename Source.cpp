#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
   
    //------------------------------- INITIALIZE --------------------------
   
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Vam Clone");


    //------------------------------- INITIALIZE --------------------------


    //------------------------------- LOAD --------------------------------
    //------------------------------- Enemy -------------------------------
    
    sf::Texture enemyTexture;
    sf::Sprite enemySpirte;

    if (enemyTexture.loadFromFile("Assets/Skelton/Textures/spritesheet.png")) 
    {
        std::cout << "Enemy images loaded!" << std:: endl;
        enemySpirte.setTexture(enemyTexture);
        enemySpirte.setPosition(sf::Vector2f(400,100));


        int XIndex = 0;
        int YIndex = 2;

        enemySpirte.setTextureRect(sf::IntRect(XIndex * 64, YIndex * 64, 64, 64));
        enemySpirte.setScale(sf::Vector2f(3,3));
          
    }
    else
    {
        std::cout << "Enemy images Failed to loaded!" << std::endl;
    }
    
    //------------------------------- Enemy -------------------------------

    //------------------------------- Player -------------------------------
    sf::Texture playerTexture;
    sf::Sprite playerSpirte;

   
    if (playerTexture.loadFromFile("Assets/Player/Textures/spritesheet.png"))
    {
        std::cout << "Player images loaded!" << std::endl;
        playerSpirte.setTexture(playerTexture);

        int XIndex = 0;
        int YIndex = 0;

        playerSpirte.setTextureRect(sf::IntRect(XIndex * 64, YIndex * 64, 64, 64));
        playerSpirte.setScale(sf::Vector2f(2,2)); 

    }
    else
    {
        std::cout << "Player images Failed to loaded!" << std::endl;
    }
    
    //------------------------------- Player -----------------------------
    //------------------------------- LOAD -------------------------------

    float playerSpeed = 200.f; 
    sf::Clock clock;

    //Game Loop
    while (window.isOpen()) {
        
        sf::Time deltaTime = clock.restart();
        float dtSeconds = deltaTime.asSeconds();


        //------------------------------- UPDATE -------------------------------

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

        
        playerSpirte.move(movement * playerSpeed * dtSeconds);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            sf::Vector2f position = playerSpirte.getPosition();
            playerSpirte.setPosition(position + sf::Vector2f(1, 0));
        }

        window.clear();
        window.draw(playerSpirte);
        window.draw(enemySpirte);
        window.display();
    
    
    }

        //------------------------------- UPDATE -------------------------------

    return 0;
}