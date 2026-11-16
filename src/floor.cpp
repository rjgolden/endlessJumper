#include "floor.hpp"
#include "palette.hpp"

void Floor::drawBackground(){
    ClearBackground(BLACK);
    DrawRectangle(0, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);          // First  
    DrawRectangle(165, -static_cast<int>(Globals::screenHeight)*10, 630, static_cast<int>(Globals::screenHeight)*13, LIGHTGREEN); // Middle    
    DrawRectangle(795, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);        // Last  
    DrawRectangle(165, static_cast<int>(Globals::floorY), 630, static_cast<int>(Globals::floorHeight)*10, BLACK);                 // Floor
}

void Floor::resetGame(Player &player, GameCamera &gameCamera, float &score){
    platforms.resetPlatforms();
    player.position = {480.0f, 440.0f};
    gameCamera.camera.target = {(Globals::screenWidth/2), (Globals::screenHeight/2)};
    score = 0.0f;
    biggestY = 440.0f;
    deaths += 1;
    ascending = 1;
}

float Floor::getEndHeight(){
    return endHeight;
}

void Floor::drawCompleteZone(){
    DrawRectangleRec(completeZone, {50, 205, 50, 178});      
}