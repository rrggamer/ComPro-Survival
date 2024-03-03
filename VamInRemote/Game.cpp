#include "Game.h"

//Private functions
void Game::initVariables()
{
	this->window = nullptr;

	//Game logic
	this->endGame = false;
	this->points = 0;
	this->health = 30;
	this->enemySpawnTimerMax = 25.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 5;
	this->mouseHeld = false;

}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	 
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

//Constructors / Destructors
Game::Game()
{
	this->initVariables();
	this->initFonts();
	this->innitText();
	this->initWindow();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f,10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
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

		Spawns enemies and sets thier types and colors. Spawns them at random positions.
		-Sets a random type (diff). 
		-Sets a random postion.
		-Sets a random color.
		-Adds enemy to the vector.
	*/

	this->enemy.setPosition(
			static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
			0.f
		);
	//Randomize enemy Type
	int type = rand() % 5;
	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(10.f, 10.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
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

		this->enemies[i].move(0.f, 5.f);
		
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << this->health << "\n";

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
						//Gain Points
						if(this->enemies[i].getFillColor() == sf::Color::Magenta)
							this->points += 10.f;
						else if(this->enemies[i].getFillColor() == sf::Color::Blue)
							this->points += 7.f;
						else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
							this->points += 5.f;
						else if (this->enemies[i].getFillColor() == sf::Color::Red)
							this->points += 3.f;
						else if (this->enemies[i].getFillColor() == sf::Color::Green)
							this->points += 1.f;

						std::cout << "Points: " << this->points << "\n";
						//Delete the enemy
						deleted = true;
						this->enemies.erase(this->enemies.begin() + i);
					}

				}

			}
		
		}
		else
		{
			this->mouseHeld = false;
		}

}

void Game::updateText()
{
	std::stringstream ss;
	
	ss << "Point : " << this->points << "\n" 
		<< "Health : " << this->health << "\n";

	this->uiText.setString(ss.str());
}


void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	//Rendering all the enemies
	for (auto& e : this->enemies)
	{
		target.draw(e);
	}
}
const bool Game::getEndGame() const
{
	return this->endGame;
}
//Accessors


//Functions
void Game::update()
{	
	this->pollEvents();

	if(this->endGame == false)
	{
		this->updateMousePosition();

		this->updateText();

		this->updateEnemies();
	
	}

	if (this->health <= 0)
		this->endGame = true;
	
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
	this->renderEnemies(*this->window);
	this->renderText(*this->window);

	this->window->display();
}

