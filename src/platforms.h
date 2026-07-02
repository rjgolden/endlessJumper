#pragma once
#include "raylib.h"
#include <array>
#include <algorithm> 

class Platforms {
    public: 
        Platforms();
        void resetPlatforms();
        
        struct Platform {
            Rectangle rect{0};
            int colorIndex{0};
        };
        float spacing{30.0f};
        std::array<Platform, 20> platformArr;
};
