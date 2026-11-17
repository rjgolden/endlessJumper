#pragma once
#include "platforms.hpp"
#include "player.hpp"
#include "gameCamera.hpp"
#include "globals.hpp"

class Floor {

    public:

        Floor();
        ~Floor() = default;

        void drawBackground();
        void resetGame(Player &player, GameCamera &gameCamera, float &score);
        void drawCompleteZone();
        float getEndHeight();

        float startingY{440.0f};
        float biggestY{440.0f};
        float endHeight{-500.0f};
        int deaths{0};
        bool ascending{true};

        Texture2D background;
        Texture2D wallTile;
        
        Platforms platforms;
        //Enemy enemies;
        //Collectibles collectibles;
        //hazards hazards;

    private:

        int currentFloor{0};
        Rectangle completeZone{0, endHeight, Globals::screenWidth, 64.0f};

};