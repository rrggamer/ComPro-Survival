#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>
#include<map>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as the game engine.
	Wrapper class.
*/
#include"Bullet.h"


class Game
{
private:
	//Variables
	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//Clock
	sf::Clock clock;
	float initialSpawnTimerMax = 10.f;

	//World
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	//Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	//Cooldown
	float attackCooldown;
	float attackCooldownMax;

	//Resources
	sf::Font font;
	std::map < std::string ,sf::Texture * >texture;
	std::vector<Bullet*> bullets;

	//TEXT
	sf::Text uiText;

	//Game logic
	bool endGame;
	unsigned points;
	int health;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;

	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	//Player
	sf::Texture playerTexture;
	sf::RectangleShape player;

	//Private functions
	void initVariables();
	void initWindow();
	void initEnemies();
	void initPlayer();
	void initWorld();
	void initFonts();
	void innitText();
	void initTexture();

public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	const bool canAttack();

		//spawn
	void spawnEnemy();
	void spawnPlayer();

		//update
	void pollEvents();
	void updateInput();
	void updateAttack();
	void updateMousePosition();
	void updateEnemies();
	void updateText();
	void updateBullet();
	void update();

		//render
	void renderEnemies();
	void renderPlayer();
	void renderWorld();
	void renderText(sf::RenderTarget& target);
	void render();
};

