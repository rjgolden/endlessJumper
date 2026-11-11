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
    DrawRectangle(0, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);
    // Middle 
    DrawRectangle(165, -static_cast<int>(Globals::screenHeight)*10, 630, static_cast<int>(Globals::screenHeight)*13, LIGHTGREEN);
    // Last
    DrawRectangle(795, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);
    // Floor
    DrawRectangle(165, static_cast<int>(Globals::floorY), 630, static_cast<int>(Globals::floorHeight)*10, BLACK);
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