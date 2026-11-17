#include "functions.hpp"
#include "globals.hpp"

void Functions::drawLightCircle(Vector2 position, float radius, Color color, Color color2) {
    DrawCircleGradient(position, radius, color, color2);
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