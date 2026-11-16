#pragma once
#include <array>
#include <algorithm> 
#include "raylib.h"
#include "globals.hpp"

struct Platform {
    Rectangle rect{0};
    bool onPlatform{false};
    bool blocking{false};
};

class Platforms {

    public: 
    
        Platforms();
        void resetPlatforms();
        void drawPlatforms();
        Rectangle generatePlatform(const Rectangle& previous);

        std::array<Platform, 90> platformArr;

    private:

        float spacing{60.0f};
        float scrollSpeed{120.0f};
        float platformHeight{10.0f};
        int currentColorIndex{0};
        int nextColorIndex{1};
        Color platformColor{};

    
};
