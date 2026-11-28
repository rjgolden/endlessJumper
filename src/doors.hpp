#pragma once 
#include "raylib.h"
#include <array>

struct Door {
    Rectangle hitbox;
    Color color{};
};

class Doors {
    
    public: 
    
        Doors();
        void resetDoors();
        void drawDoors();
    
        std::array<Door, 2> doorArr;

};