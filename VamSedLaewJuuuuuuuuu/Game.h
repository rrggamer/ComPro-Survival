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
#include"RestartButton.h"


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
	std::map < std::string, sf::Texture* >texture;
	std::vector<Bullet*> bullets;

	//TEXT
	sf::Text uiText;
	sf::Text gameOverText;
	RestartButton* restartButton;


	//Game logic
	bool endGame;
	unsigned points;
	int hp;
	int hpMax;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	bool mouseHeld;
	int enemyHp;
	float speed;
	int target;
	bool isFirstMaxHPIncrease;
	bool isSecondMaxHPIncrease;
	bool isThirdMaxHPIncrease;
	bool isForthMaxHPIncrease;

	bool isFirstTextureChange;
	bool isSecondTextureChange;

	std::vector<int> enemiesHp;

	//GUI
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;
	std::map<std::string, sf::Texture*> enemyTextures;
	
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
	void initGUI();
	void initRestart();

public:
	//Constructors / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;
	void run();

	//Functions
	const bool canAttack();
	void restart();

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
	void updateLevel();

	//delete
	void deleteEnemy(size_t index);
	//render
	void renderEnemies();
	void renderPlayer();
	void renderWorld();
	void renderGUI();
	void renderText(sf::RenderTarget& target);
	void render();
};

