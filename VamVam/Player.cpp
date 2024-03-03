#include "Player.h"
#include "Enemy.h"
#include "Math.h"


void Player::Initialize()
{
    boundingRectangle.setFillColor(sf::Color::Transparent);
    boundingRectangle.setOutlineColor(sf::Color::Red);
    boundingRectangle.setOutlineThickness(1);
    size = sf::Vector2i(64, 64);
    
}

void Player::Load()
{
    if (texture.loadFromFile("Assets/Player/Textures/spritesheet.png"))
    {
        std::cout << "Player images loaded!" << std::endl;
        sprite.setTexture(texture);

        int XIndex = 0;
        int YIndex = 0;

        sprite.setTextureRect(sf::IntRect(XIndex * size.x, YIndex * size.y, size.x, size.y));
        sprite.setPosition(sf::Vector2f(960, 540));
        sprite.setScale(sf::Vector2f(2, 2));
        boundingRectangle.setSize(sf::Vector2f(size.x * sprite.getScale().x, size.y * sprite.getScale().y));

    }
    else
    {
        std::cout << "Player images Failed to loaded!" << std::endl;
    }
}

void Player::Update(Enemy& skeleton)
{
    sf::Time deltaTime = clock.restart();
    float dtSeconds = deltaTime.asSeconds();

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


    sprite.move(movement * speed * dtSeconds);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
    {
        sf::Vector2f position = sprite.getPosition();
        sprite.setPosition(position + sf::Vector2f(1, 0));
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        bullets.push_back(sf::RectangleShape(sf::Vector2f(30, 15)));
        int i = bullets.size() - 1;
        bullets[i].setPosition(sprite.getPosition());


    }
    for (size_t i = 0; i < bullets.size(); i++) {
        sf::Vector2f direction = skeleton.sprite.getPosition() - bullets[i].getPosition();
        direction = Math::NormalizeVector(direction);
        bullets[i].setPosition(bullets[i].getPosition() + direction * bulletSpeed);

    }
    boundingRectangle.setPosition(sprite.getPosition());

    if (Math::CheckRectCollision(sprite.getGlobalBounds(), skeleton.sprite.getGlobalBounds()) == true) {
        std::cout << "GG KUB\n";
    }
}

void Player::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(boundingRectangle);
    for (size_t i = 0; i < bullets.size(); i++) {
        window.draw(bullets[i]);
    }
}
void Player::Shoot()
{
}

void Player::ReduceHp()
{
}

