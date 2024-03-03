#include <iostream>
#include "Game.h"




int main()
{
	//Init srand
	srand(static_cast<unsigned>(time(NULL)));


	//Init Game engine
	Game game;

	//Game loop
	while (game.running() && !game.getEndGame())
	{
		
		game.run();
	}


	//End of application
	return 0;
}