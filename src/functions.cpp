#include "functions.hpp"

void Functions::drawLightCircle(Vector2 position, float radius, Color color, Color color2) {
    DrawCircleGradient(
        position,
        radius,
        color,
        color2 );
}

void Functions::drawBackground(){
    ClearBackground(BLACK);
    // First
    DrawRectangle(-400, -static_cast<int>(Globals::screenHeight)*10, 570, static_cast<int>(Globals::screenHeight)*13, DARKGRAY2);
    // Middle 
    DrawRectangle(170, -static_cast<int>(Globals::screenHeight)*10, Globals::screenWidth, static_cast<int>(Globals::screenHeight)*13, LIGHTBLUE2);
    // Last
    DrawRectangle(800, -static_cast<int>(Globals::screenHeight)*10, Globals::screenWidth*2, static_cast<int>(Globals::screenHeight)*13, DARKGRAY2);
    // Floor
    DrawRectangle(160, static_cast<int>(Globals::floorY), static_cast<int>(Globals::floorHeight)*16, static_cast<int>(Globals::floorHeight)*6, BLACK);
}

void Functions::toggleFullscreenWindow(){
    if (!IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else {
        ToggleFullscreen();
        SetWindowSize(Globals::screenWidth, Globals::screenHeight);
    } 
}