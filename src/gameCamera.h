#pragma once
#include <cmath>
#include <algorithm>
#include <raylib.h>
#include "globals.h"

class GameCamera {

    public:
        GameCamera();
        Camera2D camera;
        void shakeCamera();

    private:
        float shakeTimer{0.0f};
        float shakeIntensity{16.0f};
        Vector2 middle ={(Globals::screenWidth/2), (Globals::screenHeight/2)};

};