#include "platforms.h"

Platforms::Platforms(){
    for (size_t i{0}; i < platformArr.size(); i++) {
        platformArr[i].rect = {
            static_cast<float>(GetRandomValue(10, 170)), 
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
            static_cast<float>(GetRandomValue(10, 170)), 
            400.0f - (static_cast<float>(i)*30.0f*3) - 30.0f,               
            static_cast<float>(GetRandomValue(20, 70)),                          
            10.0f                              
        };
        platformArr[i].colorIndex = 0;
    }
}