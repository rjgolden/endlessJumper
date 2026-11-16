#pragma once
#include "platforms.hpp"
#include "player.hpp"
#include "gameCamera.hpp"

class Floor {

    public:

        Floor() = default ;
        ~Floor() = default ;

        void drawBackground();
        void resetGame(Player &player, GameCamera &gameCamera, float &score);

        float startingY{440.0f};
        float biggestY{440.0f};
        float endY{-5400.0f};
        int deaths{0};
        
        Platforms platforms;
        //Enemy enemies;
        //Collectibles collectibles;
        //hazards hazards;

    private:

        int currentFloor{0};

};