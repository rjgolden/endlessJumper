#include "gameCamera.h"

GameCamera::GameCamera(){

    camera = {0};
    camera.target = middle;
    camera.offset = middle;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
}

void GameCamera::shakeCamera(){
    shakeTimer = 0.5f;        // Lasts 0.5 seconds
    float deltaTime = GetFrameTime();
    deltaTime = std::min(deltaTime, 0.05f);

    // camera shake
    Vector2 cameraShakeOffset = { 0.0f, 0.0f };
    if (shakeTimer > 0.0f) {
        shakeTimer -= deltaTime;
        float currentIntensity = shakeIntensity * (shakeTimer / 0.5f);
        cameraShakeOffset.x = (GetRandomValue(-100, 100) / 100.0f) * currentIntensity;
        cameraShakeOffset.y = (GetRandomValue(-100, 100) / 100.0f) * currentIntensity;
        camera.target = (Vector2){ middle.x + cameraShakeOffset.x, middle.y + cameraShakeOffset.y };
    }
}