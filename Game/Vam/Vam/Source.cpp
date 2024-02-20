#include <SFML/Graphics.hpp>

int main() 
{

	//----------------------------------------INITIALIZE--------------------------------//
	a

	sf::RenderWindow window(sf::VideoMode(800, 600), "Vam Clone");

	sf::CircleShape circle(50.0f);
	circle.setFillColor(sf::Color::Red);
	circle.setPosition(sf::Vector2f(100, 100));
	circle.setOutlineThickness(10);
	circle.setOutlineColor(sf::Color::Green);

	sf::Texture pngbird;
	pngbird.loadFromFile("bird.png");

	sf::Sprite bird;
	bird.setTexture(pngbird);
	bird.setPosition(100, 100);

	float speed = 0.5f;


	//----------------------------------------INITIALIZE--------------------------------//

	while (window.isOpen())
	{

		//----------------------------------------UPADTE--------------------------------//


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				bird.move(-speed, 0.0);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				bird.move(0.0, -speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				bird.move(0.0, speed);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				bird.move(speed, 0.0);

			window.draw(bird);
			window.display();
			window.clear(sf::Color::Black);
			window.draw(circle);


		}

		//----------------------------------------UPDATE--------------------------------//



		//----------------------------------------DRAW--------------------------------//
		
		

		//----------------------------------------DRAW--------------------------------//
	}

	return 0;

}