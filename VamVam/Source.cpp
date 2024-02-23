#include <SFML/Graphics.hpp>
#include <iostream>
#include<vector>
#include<math.h>
#include "Player.h"
#include "Enemy.h"


int main() {

    //------------------------------- INITIALIZE --------------------------

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Vam Clone");


    //------------------------------- INITIALIZE --------------------------
   
    Player myPlayer;
    Enemy skeleton;
    myPlayer.Initialize();
    skeleton.Initialize();
    //------------------------------- LOAD --------------------------------
    myPlayer.Load();
    skeleton.Load();

    


    
    //------------------------------- LOAD -------------------------------
       
    

    

    //Game Loop
    while (window.isOpen()) {

        


        //------------------------------- UPDATE -------------------------------

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        skeleton.Update();
        myPlayer.Update(skeleton);
        
    
//------------------------------- UPDATE -------------------------------

        window.clear();
        
        
        
        myPlayer.Draw(window);
        skeleton.Draw(window);
        window.display();


    }

    

    return 0;
}