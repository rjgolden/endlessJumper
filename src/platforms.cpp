#include "platforms.h"

Platforms::Platforms(){
    for (size_t i{0}; i < platformArr.size(); i++) {
        platformArr[i].rect = {
            static_cast<float>(GetRandomValue(170, 740)), 
            400.0f - (static_cast<float>(i)*spacing*3) - spacing,               
            static_cast<float>(GetRandomValue(20, 70)),                          
            10.0f                              
        };
        platformArr[i].colorIndex = 0;
    }
}

void Platforms::resetPlatforms(){
    for (size_t i{0}; i < platformArr.size(); i++) {
        platformArr[i].rect = {
            static_cast<float>(GetRandomValue(170, 740)), 
            400.0f - (static_cast<float>(i)*30.0f*3) - 30.0f,               
            static_cast<float>(GetRandomValue(20, 70)),                          
            10.0f                              
        };
        platformArr[i].colorIndex = 0;
    }
}

void Platforms::drawPlatforms(){
    for (auto& platform : platformArr) {
        DrawRectangleLinesEx(platform.rect, 2.0f, platformColors[platform.colorIndex]);
    }
}

void Platforms::scrollPlatforms(){
    float deltaTime = GetFrameTime();
     // platforms 
    for (auto& platform : platformArr) {
        platform.rect.y += 120.0f * deltaTime;
                    
        // if platform is below the camera view
        if (platform.rect.y > Globals::screenHeight) {
            platform.rect.x = static_cast<float>(GetRandomValue(170, 740));
            platform.rect.y = nextPlatformY;
            platform.rect.width =  static_cast<float>(GetRandomValue(20, 70)); 
        }    
    }
}

void Platforms::setCurrentColorIndex(int index){
    currentColorIndex = index;
}
void Platforms::setNextColorIndex(int index){
    nextColorIndex = index;
}

int Platforms::getCurrentColorIndex(){
    return currentColorIndex;
}
int Platforms::getNextColorIndex(){
    return nextColorIndex;
}
