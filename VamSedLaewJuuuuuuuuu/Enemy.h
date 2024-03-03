#pragma once
#include "Game.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
class Enemy {
private:
    sf::RectangleShape enemy;
    int type;
    int hp;
    int hpmax;
    int damage;
    int points;

    void initType();
    void initVariables();


public:
    
    Enemy(float pos_x, float pos_y);
    virtual ~Enemy();

    //Functions
    void update();
    void render(sf::RenderTarget* target);

    //ACC
    const sf::FloatRect getBounds() const;

    const int& getPoints() const;


};



