#pragma once

namespace Globals {  
    constexpr int screenWidth = 960;
    constexpr int screenHeight = 540;
    constexpr int halfScreenWidth = 480;
    constexpr int halfScreenHeight = 270;
    constexpr float referenceFPS{60.0f};

    constexpr float gravity{0.5f};
    constexpr float groundAcceleration{0.9f};
    constexpr float groundFriction{0.75f};
    constexpr float airAcceleration{0.6f};
    constexpr float airFriction{0.85f};

    constexpr float wallRiseFriction{1.02f};
    constexpr float wallFallFriction{0.93f};

    constexpr float movementPressDelay{5.0f / referenceFPS}; // 5 frames/sec
    constexpr float maxJumpHoldTime{10.0f / referenceFPS};
    constexpr float jumpHoldForce{0.9f};

    constexpr float floorHeight{40.0f};
    constexpr float floorY{screenHeight - floorHeight};

    struct Platform {
        Rectangle rect{0};
        bool onPlatform{false};
        bool blocking{false};
    };
    struct Bullet {
        Vector2 position;
        Vector2 direction;
    };
}