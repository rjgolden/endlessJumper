#include "functions.h"

void Functions::drawLight(Vector2 position, float radius, Color color) {
    DrawCircleGradient(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        radius,
        color,
        BLANK );
}

void Functions::drawSideArt(int screenW, int screenH) {
    static Texture2D sideArt = LoadTexture("src/resources/sideArt.png");
    DrawTexturePro( sideArt, 
                    {0.0f, 0.0f, static_cast<float>(screenW), static_cast<float>(screenH)}, 
                    {0.0f, 0.0f, static_cast<float>(sideArt.width), static_cast<float>(sideArt.width)}, 
                    {0.0f, 0.0f}, 
                    0.0f, 
                    WHITE);
    /*for (int y = 0; y < screenH; y += 40)s
    {
        DrawRectangle(0, y, screenW, 20, RED);
        DrawRectangle(0, y+20, screenW, 20, WHITE);
    }*/
}

void Functions::toggleFullscreenWindow(){
    if (!IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else {
        ToggleFullscreen();
        SetWindowSize(Global::startScreenWidth, Global::startScreenHeight);
    } 
}