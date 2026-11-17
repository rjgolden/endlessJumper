#pragma once
#include "platforms.hpp"
#include "gameCamera.hpp"
#include "globals.hpp"
#include "collectibles.hpp"
#include "functions.hpp"

class Player;
class GameCamera;

class Floor {

    public:

        Floor();
        ~Floor();

        void drawBackground();
        void resetGame(Player &player, GameCamera &gameCamera);
        void update(Player& player, GameCamera& gameCamera);
        void drawCompleteZone();
        float getEndHeight();

        float score{0};
        float biggestY{440.0f};
        int deaths{0};
        bool ascending{true};

        Platforms platforms;
        Collectibles collectibles;
        //Enemy enemies;
        //hazards hazards;

        RenderTexture2D lightMap;
        RenderTexture2D target;

    private:

        int currentFloor{0};
        float endHeight{-500.0f};
        float startingY{440.0f};
       
        Rectangle completeZone{0, endHeight, Globals::screenWidth, 64.0f};
        Texture2D background;
        Texture2D wallTile;

};