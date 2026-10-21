#include "platforms.hpp"

Platforms::Platforms() {
    resetPlatforms();
}

void Platforms::resetPlatforms() {
    platformArr[0].rect = {
        static_cast<float>(GetRandomValue(170, 720)),
        static_cast<float>(GetRandomValue(200, 250)),
        static_cast<float>(GetRandomValue(50, 90)),
        platformHeight
    };
    for (size_t i{1}; i < platformArr.size(); i++) {
        platformArr[i].rect = generatePlatform(platformArr[i-1].rect);
        platformArr[i].onPlatform = false;
        platformArr[i].blocking = false;
        //if(GetRandomValue(0, 1) == 0) platformArr[i].blocking = true;
    }
}

Rectangle Platforms::generatePlatform(const Rectangle& previous) {
    float x{static_cast<float>(GetRandomValue(170, 720))};
    float y = previous.y - static_cast<float>(GetRandomValue(80, 100));
    float width{static_cast<float>(GetRandomValue(50, 90))};
    
    // Keep horizontal distance from the previous platform
    if (std::abs(x - previous.x) < spacing) {
        x = previous.x + (x >= previous.x ? spacing : -spacing);
    }

    x = std::clamp(x, 170.0f, 740.0f);

    return {x, y, width, platformHeight};
}

void Platforms::scrollPlatforms() {
    float deltaTime{GetFrameTime()};

    for(auto& platform : platformArr){
        platform.rect.y += scrollSpeed * deltaTime;
    }

    for (auto& platform : platformArr) {

        if (platform.rect.y > Globals::screenHeight) {

            size_t highestIndex{0};

            for (size_t i{1}; i < platformArr.size(); i++) {
                if (platformArr[i].rect.y < platformArr[highestIndex].rect.y) {
                    highestIndex = i;
                }
            }

            platform.rect = generatePlatform(platformArr[highestIndex].rect);
            platform.onPlatform = false;
        }
    }
}

void Platforms::drawPlatforms(){
    for (auto& platform : platformArr) {
        if(platform.blocking) platformColor = WHITE;
        else platformColor = platform.onPlatform ? RED : GRAY;
        DrawRectangleLinesEx(platform.rect, 2.0f, platformColor);
    }
}
