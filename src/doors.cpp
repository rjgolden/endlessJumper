#include "doors.hpp"
#include "globals.hpp"

Doors::Doors() {
    resetDoors();
}

void Doors::resetDoors() {

    doorArr[0].hitbox = {
        Globals::leftEdge - 16.0f,  // x
        Globals::floorY - 64.0f,  // y
        17.0f,  // w
        64.0f                                 // h
    };

    doorArr[0].color = RED;

    doorArr[1].hitbox = {
        Globals::rightEdge - 16.0f - Globals::screenWidth,  // x
        Globals::floorY - 64.0f,  // y
        17.0f,  // w
        64.0f                                 // h
    };

    doorArr[1].color = RED;

}


void Doors::drawDoors(){
    for (Door& door : doorArr) {
        DrawRectangleLinesEx(door.hitbox, 1.0f, door.color);
    }
} 
