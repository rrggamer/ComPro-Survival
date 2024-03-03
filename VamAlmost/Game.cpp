#include "Game.h"

//Private functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 10;
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 25;
	this->mouseHeld = false;

}

void Game::initWindow()
{
	this->videoMode.height = 1080;
	this->videoMode.width = 1920;
	 
	this->window = new sf::RenderWindow(this->videoMode, "ComPro Survival", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

//Constructors / Destructors
Game::Game()
{
	this->enemySpawnTimerMax = initialSpawnTimerMax;

	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initPlayer();
	this->spawnPlayer();
	this->initWorld();
}

Game::~Game()
{
	delete this->window;
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f,10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
	//this->enemy.setOutlineColor(sf::Color::Green);
	//this->enemy.setOutlineThickness(1.f);


}

const bool Game::running() const
{
	return this->window->isOpen();
}
//Functions
void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;

		}
	}
}
void Game::updateMousePosition()
{
	/*
	@ return void

	Update the mouse positions:
		- Mouse position relative to window (Vecetor2i)
	*/
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}
//Functions
void Game::spawnEnemy()
{
	/*
		@return void

		Spawns enemies and sets thier colors and positions.
		-Sets a random postion.
		-Sets a random color.
		-Adds enemy to the vector.
	*/

	float windowWidth = static_cast<float>(this->window->getSize().x);
	float windowHeight = static_cast<float>(this->window->getSize().y);

	float spawnX, spawnY;

	// Randomly select whether to spawn on the left/right or top/bottom side of the screen
	if (rand() % 2 == 0) {
		// Spawn on left/right side
		spawnX = (rand() % 2 == 0) ? -100.f : windowWidth + 100.f; // 100.f is the buffer distance outside the window
		spawnY = static_cast<float>(rand() % static_cast<int>(windowHeight));
	}
	else {
		// Spawn on top/bottom side
		spawnX = static_cast<float>(rand() % static_cast<int>(windowWidth));
		spawnY = (rand() % 2 == 0) ? -100.f : windowHeight + 100.f; // 100.f is the buffer distance outside the window
	}
	
	this->enemy.setPosition(spawnX,spawnY);
	this->enemy.setFillColor(sf::Color::Green);
	
	//Random Enemy Type
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	}


	//Spawn the enemy
	this->enemies.push_back(this->enemy);

	
}
void Game::updateEnemies()
{
	/*
		@return void

		Update the enemy spawn timer and spawns eneimes
		when the total amount of enemies is smaller than the maximum.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen. //TODO
		
	*/
	float elapsedTime = clock.getElapsedTime().asSeconds();
	this->enemySpawnTimerMax = initialSpawnTimerMax / (1 + elapsedTime / 10);

	//Updating the timer for enemy spawing
	if (this->enemies.size() < this->maxEnemies) 
	{
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) 
		{
			//Spawn the enemy  and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//Moving and updating enemies
	for (int i = 0; i < this->enemies.size(); i++) 
	{
		bool deleted = false;
		
		sf::Vector2f direction = sf::Vector2f(this->window->getSize().x/2, this->window->getSize().y/2) - this->enemies[i].getPosition();
		// Normalize direction
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction /= length;

		float speed = 1.0f; // Example speed, adjust as needed
		this->enemies[i].move(direction * speed);
		
		if (this->player.getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
		{

			this->health -= 1;
			std::cout << "Player Hit! Health: " << this->health << std::endl;
			//Delete the enemy
			this->enemies.erase(this->enemies.begin() + i);

		}

	}

	//Check if clicked upon
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if(this->mouseHeld == false)
			{
				this->mouseHeld = true;
				bool deleted = false;
				for (size_t i = 0; i < this->enemies.size() && deleted == false;i++)
				{
					if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
					{
						//Delete the enemy
						deleted = true;
						this->enemies.erase(this->enemies.begin() + i);

						//Gain Points
						this->points += 10.f;
						std::cout << "Points: " << this->points << "\n";
					}
				}

			}
		
		}
		else
		{
			this->mouseHeld = false;
		}

		clock.restart();
}

void Game::initPlayer()
{
	// Load the player texture from a PNG file
	if (!this->playerTexture.loadFromFile("Assets/player/player.png")) {
		// Handle loading error
		std::cerr << "Failed to load player texture!" << std::endl;
	}
	player.setTexture(&playerTexture);


	// Get the center of the window
	sf::Vector2f centerWindow(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f);

	// Set the player's position relative to the center of the window
	this->player.setPosition(centerWindow);

	//Set CenterOrigin
	sf::Vector2u playerTextureSize = playerTexture.getSize();
	player.setOrigin(playerTextureSize.x / 2.f, playerTextureSize.y / 2.f);

	this->player.setSize(sf::Vector2f(100.f, 100.f));
	this->player.setScale(sf::Vector2f(1.f, 1.f));
	this->player.setFillColor(sf::Color::White);
}


void Game::renderEnemies()
{
	//Rendering all the enemies
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}
const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::initWorld()
{
	if (!this->worldBackgroundTex.loadFromFile("Assets/BG/BG1.png"))
	{
		std::cerr << "Failed to load background texture!" << std::endl;
	}
	this->worldBackground.setTexture(this->worldBackgroundTex);
}

void Game::spawnPlayer()
{
	// Get the center of the window
	sf::Vector2f centerWindow(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f);

	// Set the player's position relative to the center of the window
	this->player.setPosition(centerWindow);

}
void Game::renderPlayer()
{
	//Rendering all the enemies

		this->window->draw(this->player);
}
void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}
//Accessors


//Functions
void Game::update()
{	
	this->pollEvents();

	if(this->endGame == false)
	{
		this->updateMousePosition();

		this->updateEnemies();
	
	}

	if (this->health <= 0)
		this->endGame = false;
	
}	


void Game::render()
{
	/*

		
		- clear old frame
		- render objects
		- display frame in window

		Render the game objects.
	*/

	this->window->clear();

	//Draw Game objects
	this->renderWorld();
	this->renderEnemies();
	this->renderPlayer();

	this->window->display();
}

