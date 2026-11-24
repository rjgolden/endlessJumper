#pragma once
#include <array>
#include "raylib.h"
#include "globals.hpp"

struct Platform {
    Rectangle rect{};
    bool onPlatform{};
    bool goldNuggetsOnPlatform{};
    bool falsePlatform{};
    bool breakablePlatform{};
};

class Platforms {

    public: 
    
        Platforms();
        void resetPlatforms();
        void drawPlatforms();
        
        Rectangle generatePlatform(const Rectangle& previous);
        std::array<Platform, Globals::platformCount> platformArr;

    private:

        float spacing{40.0f};
        float platformHeight{10.0f};
        Color platformColor{};

};
