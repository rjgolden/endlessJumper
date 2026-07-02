#pragma once
#include "raylib.h"
#include "globals.h"

namespace Functions {
    void drawLight(Vector2 position, float radius, Color color);
    void drawSideArt(int screenW, int screenH);
    void toggleFullscreenWindow();
}