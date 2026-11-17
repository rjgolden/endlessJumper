#pragma once
#include "raylib.h"
#include <array>
#include "globals.hpp"


struct Collectible {
    Vector2 position{};
    float radius{20.0f};
    int type{};
    bool collected{};
};


class Collectibles {

    public:

        Collectibles();
        ~Collectibles() = default;
        void drawCollectibles();
        std::array<Collectible, Globals::collectibleCount> collectibleArr = {};
        std::array<Vector2, Globals::collectibleCount> collectiblePositions = {
            Vector2{250, -1000}, 
            Vector2{250, -1250}, 
            Vector2{250, -1500},
            Vector2{250, -1750},
            Vector2{250, -2000},
        };


};