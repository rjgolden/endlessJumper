#pragma once
#include "platforms.hpp"
#include "gameCamera.hpp"
#include "globals.hpp"
#include "collectibles.hpp"

class Player;
class GameCamera;

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
        int deaths{0};
        bool ascending{true};
        
        Platforms platforms;
        Collectibles collectibles;
        //Enemy enemies;
        //hazards hazards;

    private:

        int currentFloor{0};
        float endHeight{-500.0f};
        Rectangle completeZone{0, endHeight, Globals::screenWidth, 64.0f};
        Texture2D background;
        Texture2D wallTile;

};