#pragma once

#include<iostream>
#include<vector>
#include<ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as the game engine.
	Wrapper class.
*/



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
public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();
	void spawnPlayer();

	void pollEvents();
	void updateMousePosition();
	void updateEnemies();
	void update();

	void renderEnemies();
	void render();

	void renderPlayer();
	void renderWorld();
};

