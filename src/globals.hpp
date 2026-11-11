#pragma once

namespace Globals {  
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;
    constexpr int halfScreenWidth = 480;
    constexpr int halfScreenHeight = 270;
    constexpr float referenceFPS{60.0f};
    constexpr float floorHeight{40.0f};
    constexpr float floorY{screenHeight - floorHeight};
}

struct Platform {
    Rectangle rect{0};
    bool onPlatform{false};
    bool blocking{false};
};
struct Bullet {
    Vector2 position;
    Vector2 direction;
    float rotation{0.0f};
};