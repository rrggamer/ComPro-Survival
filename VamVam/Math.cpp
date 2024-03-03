#include "Math.h"

sf::Vector2f Math::NormalizeVector(sf::Vector2f vector)
{
    float m = std::sqrt(pow(vector.x, 2) + pow(vector.y, 2));

    sf::Vector2f normalizeVector;

    normalizeVector.x = vector.x / m;
    normalizeVector.y = vector.y / m;
    return normalizeVector;
}

bool Math::CheckRectCollision(sf::FloatRect rect1, sf::FloatRect rect2)
{
    if (rect1.left + rect1.width > rect2.left && rect1.left < rect2.left + rect2.width &&
        rect2.top + rect2.height > rect1.top && rect2.top < rect1.top + rect1.height) {
        return true;
    }else
    return false;
}
