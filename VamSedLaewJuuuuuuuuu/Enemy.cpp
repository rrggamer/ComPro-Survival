#include "Enemy.h"

#include "Game.h"
 



void Enemy::initType()
{
    this->enemy.setSize(sf::Vector2f(80.f, 80.f));
    this->enemy.setFillColor(sf::Color::Red);

}

void Enemy::initVariables()
{
    this->type = 0;
    this->hp = 0;
    this->hpmax = 10;
    this->damage = 1;
    this->points = 5;
}

Enemy::Enemy(float pos_x, float pos_y)
{
    this->initType();
    this->initVariables();
    this->enemy.setPosition(pos_x, pos_y);

}

Enemy::~Enemy()
{
    
}
void Enemy::update()
{
    

        sf::Vector2f direction = sf::Vector2f(1920 / 2, 1080/ 2) - this->enemy.getPosition();
        // Normalize direction
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length;

        float speed = 1.0f;
        this->enemy.move(direction * speed);

        //Remove
      
}

void Enemy::render(sf::RenderTarget* target)
{
    target->draw(this->enemy);
}

const sf::FloatRect Enemy::getBounds() const
{
    return this->enemy.getGlobalBounds();
}

const int& Enemy::getPoints() const
{
    return this->points;
}

