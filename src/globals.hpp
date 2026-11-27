#pragma once

namespace Globals {  
    constexpr int screenWidth{960};
    constexpr int screenHeight{540};
    constexpr int halfScreenWidth{480};
    constexpr int halfScreenHeight{270};
    constexpr int platformCount{84};
    constexpr int collectibleCount{20};
    constexpr int enemyCount{5};
    constexpr float referenceFPS{60.0f};
    constexpr float floorHeight{40.0f};
    constexpr float leftEdge{160.0f};
    constexpr float rightEdge{800.0f};
    constexpr float edgeWidth{leftEdge};
    constexpr float middleWidth{640.0f};  
    constexpr float floorY{screenHeight - floorHeight};
    constexpr float ceilingHeight{-5400.0f};
    
    struct Projectile {
        Vector2 position;
        Vector2 direction;
        float rotation{0.0f};
        float radius{10.0f};
        float shotSpeed{300.0f};
        float knockbackStrength{300.0f};
        float damage{10.0f};
    };
};
