#include "Game.h"
#include "RestartButton.h"

//Private functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->hpMax = 10;
	this->hp = this->hpMax;
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 25.f;
	this->mouseHeld = false;

	this->attackCooldownMax = 10.f;
	this->attackCooldown = this->attackCooldownMax;
	int enemyHp = 0;

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

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(75);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("Game Over!");
	this->gameOverText.setPosition(this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);



}

void Game::initTexture()
{
	this->texture["BULLET"] = new sf::Texture();
	this->texture["BULLET"]->loadFromFile("Assets/Bullet/fireball.png");

}

void Game::initGUI()
{
	//init player GUI
	this->playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(sf::Vector2f(20.f, 20.f));

	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
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
	this->initGUI();
	this->initTexture();
	this->restartButton = new RestartButton(10.f, 10.f, 120.f, 50.f);


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
	for (auto* i : this->bullets)
	{
		delete i;
	}
	delete restartButton;
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
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
void Game::pollEvents() {
	while (window->pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left) {
				if (restartButton->checkClick(sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y))) {
					restart();
				}
			}
			break;
		default:
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

	this->enemy.setPosition(spawnX, spawnY);
	this->enemy.setFillColor(sf::Color::Green);

	//Random Enemy Type
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		this->enemyHp = 20;
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Blue);
		this->enemyHp = 40;
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		this->enemyHp = 60;
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Red);
		this->enemyHp = 80;
		break;
	default:
		this->enemy.setSize(sf::Vector2f(80.f, 80.f));
		this->enemy.setFillColor(sf::Color::Green);
		this->enemyHp = 100;
		break;
	}


	//Spawn the enemy
	this->enemies.push_back(this->enemy);
	this->enemiesHp.push_back(this->enemyHp);


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

		sf::Vector2f direction = sf::Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2) - this->enemies[i].getPosition();
		// Normalize direction
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		direction /= length;

		float speed = 1.0f; // Example speed, adjust as needed
		this->enemies[i].move(direction * speed);

		if (this->player.getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
		{
			// Handle player-enemy collision
			this->hp -= 1;
			std::cout << "Player Hit! Health: " << this->hp << std::endl;
			deleteEnemy(i);
		}

	}

	//Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
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
		<< "Health : " << this->hp << "\n";

	this->uiText.setString(ss.str());

	//Update player GUI
	float hpPercent = (static_cast<float> (this->hp) / this->hpMax);
	this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateBullet()
{
	unsigned counter = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();

		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			// Delete bullet if it goes out of the window
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}

		++counter;
	}

	// Check collision between bullets and enemies
	for (size_t i = 0; i < this->bullets.size(); ++i)
	{
		for (size_t j = 0; j < this->enemies.size(); ++j)
		{
			if (this->bullets[i]->getBounds().intersects(this->enemies[j].getGlobalBounds()))
			{
				this->points += 10.f;
				// Mark both bullet and enemy for deletion
				delete this->bullets[i];
				this->bullets.erase(this->bullets.begin() + i);
				deleteEnemy(j);
				break; // Break inner loop to avoid invalid access
			}
		}
	}
}

void Game::deleteEnemy(size_t index)
{
	// Remove enemy at given index
	this->enemies.erase(this->enemies.begin() + index);
}

//Functions
void Game::update()
{




	this->updateMousePosition();

	this->updateInput();

	this->updateAttack();

	this->updateEnemies();

	this->updateText();

	this->updateBullet();


}



void Game::restart() {

	this->endGame = false;
	this->points = 0;
	this->hpMax = 10;
	this->hp = this->hpMax;
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 25;
	this->mouseHeld = false;
	this->attackCooldownMax = 10.f;
	this->attackCooldown = this->attackCooldownMax;

	// Clear bullets vector
	for (auto* bullet : this->bullets) {
		delete bullet;
	}
	this->bullets.clear();

	// Clear enemies vector
	this->enemies.clear();
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

void Game::run()
{
	while (this->window->isOpen())
	{
		this->pollEvents();

		if (this->hp > 0)
			this->update();


		this->render();


	}
	if (restartButton->clicked() && this->hp <= 0) {
		restart();
	}
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

void Game::renderGUI()
{
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
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
	this->renderGUI();

	//Game Over
	if (this->hp <= 0)
	{
		this->window->draw(this->gameOverText);
		restartButton->draw(*window);
	}


	this->window->display();
}