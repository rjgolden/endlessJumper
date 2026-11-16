#include "floor.hpp"
#include "palette.hpp"

void Floor::drawBackground(){
    ClearBackground(BLACK);
    // First
    DrawRectangle(0, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);
    // Middle 
    DrawRectangle(165, -static_cast<int>(Globals::screenHeight)*10, 630, static_cast<int>(Globals::screenHeight)*13, LIGHTGREEN);
    // Last
    DrawRectangle(795, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);
    // Floor
    DrawRectangle(165, static_cast<int>(Globals::floorY), 630, static_cast<int>(Globals::floorHeight)*10, BLACK);
}


void Floor::resetGame(Player &player, GameCamera &gameCamera, float &score){
    platforms.resetPlatforms();
    player.position = {480.0f, 440.0f};
    gameCamera.camera.target = {(Globals::screenWidth/2), (Globals::screenHeight/2)};
    score = 0.0f;
    biggestY = 440.0f;
    deaths += 1;
}
