#pragma once
#include <array>
#include <algorithm> 
#include "raylib.h"
#include "globals.hpp"

class Platforms {
    public: 
        Platforms();
        void resetPlatforms();
        void drawPlatforms();
        void scrollPlatforms();
        Rectangle generatePlatform(const Rectangle& previous);

        float spacing{60.0f};
        float scrollSpeed{120.0f};
        float platformHeight{10.0f};
        int currentColorIndex{0};
        int nextColorIndex{1};
        Color platformColor{};
        std::array<Globals::Platform, 20> platformArr;
};
