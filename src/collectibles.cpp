#include "collectibles.hpp"

Collectibles::Collectibles(){
    resetCollectibles();
}

void Collectibles::resetCollectibles(){
    float currentY = -5400;
    for(int i{}; i<Globals::collectibleCount; i++){
        float x{static_cast<float>(GetRandomValue(170, 600))};
        float y{currentY - static_cast<float>(GetRandomValue(80, 100))};
        collectiblePositions[i] = {x, y};
        collectibleArr[i].position = collectiblePositions[i];
        currentY += 540.0f;
    }
}

void Collectibles::drawCollectibles() {
    for (const auto& collectible : collectibleArr){
        if(!collectible.collected){
            DrawCircleLines(collectible.position.x, collectible.position.y, collectible.radius, collectible.collected ? RED : GREEN);
        }
    }
}
