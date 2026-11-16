#pragma once
#include <array>
#include "raylib.h"

struct Platform {
    Rectangle rect{0};
    bool onPlatform{false};
    bool falsePlatform{false};
    bool breakablePlatform{false};
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
        float platformHeight{10.0f};
        Color platformColor{};

    
};
