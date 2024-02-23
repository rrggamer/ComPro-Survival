#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>

class Math
{
public:
	static sf::Vector2f NormalizeVector(sf::Vector2f );
	static bool CheckRectCollision(sf::FloatRect rect1,sf::FloatRect rect2);
};

