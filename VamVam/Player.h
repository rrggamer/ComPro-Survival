#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>
#include "Enemy.h"

class Player
{
	sf::Texture texture;
	float speed = 200.f;
	sf::Clock clock;
	std::vector<sf::RectangleShape> bullets;
	float bulletSpeed = 2.0f;
	sf::RectangleShape boundingRectangle;
	sf::Vector2i size;
public:
	sf::Sprite sprite;

public:
	void Initialize();
	void Load();
	void Update(Enemy&);
	void Draw(sf::RenderWindow&);
	void Shoot();
	void ReduceHp();
};

