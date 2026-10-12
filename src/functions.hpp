#pragma once
#include "raylib.h"
#include "globals.hpp"
#include "palette.hpp"

namespace Functions {
    void drawLightCircle(Vector2 position, float radius, Color color, Color color2);
    void drawBackground();
    void toggleFullscreenWindow();
}