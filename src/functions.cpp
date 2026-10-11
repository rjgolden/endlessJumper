#include "functions.h"

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
    DrawRectangle(0, 0, 170, Globals::screenHeight, DARKGRAY2);
    // Middle 
    DrawRectangle(160, 0, Globals::screenWidth, Globals::screenHeight, LIGHTBLUE2);
    // Last
    DrawRectangle(800, 0, Globals::screenWidth, Globals::screenHeight, DARKGRAY2);
    // Floor
    DrawRectangle(-100, static_cast<int>(Globals::floorY), Globals::screenWidth * 2, static_cast<int>(Globals::floorHeight) * 2, BLACK);
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