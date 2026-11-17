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
        void resetCollectibles();
        std::array<Collectible, Globals::collectibleCount> collectibleArr = {};
        std::array<Vector2, Globals::collectibleCount> collectiblePositions = {};

};