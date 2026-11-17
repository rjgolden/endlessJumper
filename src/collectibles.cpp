#include "collectibles.hpp"

Collectibles::Collectibles(){
    for(int i{}; i<Globals::collectibleCount; i++){
        collectibleArr[i].position = collectiblePositions[i];
    }
}

void Collectibles::drawCollectibles() {
    for (const auto& collectible : collectibleArr){
        if(!collectible.collected){
            DrawCircleLines(collectible.position.x, collectible.position.y, collectible.radius, collectible.collected ? RED : GREEN);
        }
    }
}
