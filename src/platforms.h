#pragma once
#include "raylib.h"
#include <array>
#include <algorithm> 
#include "globals.h"

class Platforms {
    public: 
        Platforms();
        void resetPlatforms();
        void drawPlatforms();
        void scrollPlatforms();
        void setCurrentColorIndex(int index);
        void setNextColorIndex(int index);
        int getCurrentColorIndex();
        int getNextColorIndex();

        float spacing{30.0f};
        float nextPlatformY{0.0f};
        int currentColorIndex{0};
        int nextColorIndex{1};
        std::array<Color, 5> platformColors = {GRAY, DARKBLUE, YELLOW, PINK, BROWN};
        std::array<Globals::Platform, 20> platformArr;
};
