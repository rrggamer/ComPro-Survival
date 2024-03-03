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

	this->attackCooldownMax = 10.f;
	this->attackCooldown = this->attackCooldownMax;

}

void Game::initWindow()
{
	this->videoMode.height = 1080;
	this->videoMode.width = 1920;
	 
	this->window = new sf::RenderWindow(this->videoMode, "ComPro Survival", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
} 

void Game::initFonts()
{
	if (this->font.loadFromFile("Fonts/VCR_OSD_MONO_1.001.ttf")) {

		std::cout << "ERROR::GAME::INITFONTS::Failed to load font!\n";
	}
}

void Game::innitText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(25);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("NONE");


}

void Game::initTexture()
{
	this->texture["BULLET"] = new sf::Texture();
	this->texture["BULLET"]->loadFromFile("Assets/Bullet/fireball.png");

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
	this->initFonts();
	this->innitText();
	this->initTexture();
}

Game::~Game()
{
	delete this->window;
	//Delete texture
	for (auto& i : this->texture) 
	{
		delete i.second;
	}
	//Deleted bullets
	for(auto *i : this->bullets)
	{
		delete i;
	}
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
void Game::updateInput()
{
	if (this->canAttack())
	{
		// Calculate distances to enemies and store them in a vector of pairs (distance, index)
		std::vector<std::pair<float, size_t>> distances;
		for (size_t i = 0; i < this->enemies.size(); ++i)
		{
			sf::Vector2f enemyPos = this->enemies[i].getPosition();
			sf::Vector2f playerPos = this->player.getPosition();
			float distance = std::sqrt(std::pow(enemyPos.x - playerPos.x, 2) + std::pow(enemyPos.y - playerPos.y, 2));
			distances.push_back(std::make_pair(distance, i));
		}

		// Sort the distances vector by distance
		std::sort(distances.begin(), distances.end());

		// Fire bullets towards the two nearest enemies
		for (size_t i = 0; i < std::min(distances.size(), size_t(2)); ++i)
		{
			sf::Vector2f direction = this->enemies[distances[i].second].getPosition() - this->player.getPosition();
			float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
			direction /= length;
			this->bullets.push_back(new Bullet(this->texture["BULLET"], this->player.getPosition().x, this->player.getPosition().y, direction.x, direction.y, 5.f));
		}
		this->bullets.push_back(new Bullet(this->texture["BULLET"], this->player.getPosition().x, this->player.getPosition().y, -1.f, 0.f, 5.f)); // Bullet moving left
		this->bullets.push_back(new Bullet(this->texture["BULLET"], this->player.getPosition().x, this->player.getPosition().y, 1.f, 0.f, 5.f)); // Bullet moving right

	}
}

void Game::updateAttack()
{
	// Update attack cooldown
	if (this->attackCooldown < this->attackCooldownMax)
		this->attackCooldown += 0.1f;
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

void Game::updateText()
{
	std::stringstream ss;

	ss << "Point : " << this->points << "\n"
		<< "Health : " << this->health << "\n";

	this->uiText.setString(ss.str());
}

void Game::updateBullet()
{
	unsigned counter = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();
	
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{	
			//Delete bullet
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}

		++counter;
	}

}

//Functions
void Game::update()
{
	this->pollEvents();

	if (this->endGame == false)
	{
		this->updateMousePosition();

		this->updateInput();

		this->updateAttack();

		this->updateText();

		this->updateEnemies();

		this->updateBullet();

	}

	if (this->health <= 0)
		this->endGame = false;

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

const bool Game::canAttack()
{
	if (this->attackCooldown >= attackCooldownMax)
	{
		this->attackCooldown = 0.f; // Reset cooldown
		return true; // Allow firing
	}
	else
		return false; // Don't allow firing yet
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

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

//Accessors

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
	for (auto* bullet : this->bullets)
	{
		bullet->render(this->window);
	}
	this->renderPlayer();
	this->renderText(*this->window);

	this->window->display();
}

