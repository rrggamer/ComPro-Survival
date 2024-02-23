#include "Enemy.h"

void Enemy::Initialize()
{
    boundingRectangle.setFillColor(sf::Color::Transparent);
    boundingRectangle.setOutlineColor(sf::Color::Blue);
    boundingRectangle.setOutlineThickness(1);
    size = sf::Vector2i(64, 64);
}

void Enemy::Load()
{
    if (texture.loadFromFile("Assets/Skelton/Textures/spritesheet.png"))
    {
        std::cout << "Enemy images loaded!" << std::endl;
        sprite.setTexture(texture);
        sprite.setPosition(sf::Vector2f(400, 100));


        int XIndex = 0;
        int YIndex = 2;

        sprite.setTextureRect(sf::IntRect(XIndex * 64, YIndex * 64, 64, 64));
        sprite.setScale(sf::Vector2f(2, 2));
        boundingRectangle.setSize(sf::Vector2f(size.x * sprite.getScale().x, size.y * sprite.getScale().y));

    }
    else
    {
        std::cout << "Enemy images Failed to loaded!" << std::endl;
    }

}

void Enemy::Update()
{
    boundingRectangle.setPosition(sprite.getPosition());
}

void Enemy::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(boundingRectangle);
}
