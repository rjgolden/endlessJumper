#include "floor.hpp"
#include "palette.hpp"
#include "player.hpp"
#include <cmath>
#include <algorithm>

Floor::Floor(){
    lightMap = LoadRenderTexture(Globals::screenWidth, Globals::screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    target = LoadRenderTexture(Globals::screenWidth, Globals::screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 
    background = LoadTexture("src/resources/background.png");
    wallTile = LoadTexture("src/resources/wallTile.png");
}

Floor::~Floor(){
    UnloadRenderTexture(lightMap);
    UnloadRenderTexture(target);
    UnloadTexture(background);
    UnloadTexture(wallTile);
}

void Floor::drawBackground(){
    ClearBackground(BLACK);
    /*
    DrawRectangle(0, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);          // First  
    DrawRectangle(165, -static_cast<int>(Globals::screenHeight)*10, 630, static_cast<int>(Globals::screenHeight)*13, LIGHTGREEN); // Middle    
    DrawRectangle(795, -static_cast<int>(Globals::screenHeight)*10, 165, static_cast<int>(Globals::screenHeight)*13, TAN);        // Last  
    DrawRectangle(165, static_cast<int>(Globals::floorY), 630, static_cast<int>(Globals::floorHeight)*10, BLACK);                 // Floor
    */
    
    DrawRectangle(0, -5400 - Globals::floorHeight, static_cast<int>(Globals::screenWidth), Globals::floorHeight, RED);         
    float backgroundPlacement = Globals::ceilingHeight;
    for(int i{0}; i<12; i++){
        DrawTexture(background, 0, backgroundPlacement, WHITE);
        backgroundPlacement += 540.0f;
    }
    float tilePlacement = Globals::ceilingHeight;
    for(int i{0}; i<15*13; i++){
        DrawTexture(wallTile, 0, tilePlacement, WHITE);
        DrawTexture(wallTile, 795, tilePlacement, WHITE);
        tilePlacement += 36.0f;
    }
    DrawRectangle(165, static_cast<int>(Globals::floorY), 630, static_cast<int>(Globals::floorHeight)*10, BLACK); // Floor
}

void Floor::resetGame(Player &player, GameCamera &gameCamera){

    platforms.resetPlatforms();
    player.position = {480.0f, 440.0f};
    gameCamera.camera.target = {(Globals::screenWidth/2), (Globals::screenHeight/2)};
    score = 0.0f;
    biggestY = 440.0f;
    deaths += 1;
    ascending = 1;

}

void Floor::update(Player& player, GameCamera& gameCamera){

    float deltaTime = GetFrameTime();
    if(IsKeyPressed(KEY_F11)) Functions::toggleFullscreenWindow();
    if(IsKeyPressed(KEY_G)) gameCamera.shakeCamera();
    if(IsKeyPressed(KEY_R)) resetGame(player, gameCamera);
    
    if(IsKeyPressed(KEY_M)) {
        if(player.debugMovement) player.debugMovement = false;
        else player.debugMovement = true;
    }

    float speed = fabs(player.velocity.y);
    float followSpeed = 1.55f + speed * 0.15f;
    followSpeed = std::clamp(followSpeed, 1.55f, 8.0f);
    float targetY = ascending ? biggestY : player.position.y;

    Vector2 playerScreenPos = GetWorldToScreen2D(player.position, gameCamera.camera);
    if(playerScreenPos.y > Globals::screenHeight && ascending) {
            resetGame(player, gameCamera);
            score -= 5.0f;
    }

    if(player.position.y < biggestY) {
        biggestY = player.position.y;
    }

    if(player.position.y < getEndHeight() + 64.0f) {
        ascending = false;
    }

    gameCamera.camera.target.y += (targetY - gameCamera.camera.target.y) * (followSpeed * deltaTime);
}

float Floor::getEndHeight(){
    return endHeight;
}

void Floor::drawCompleteZone(){
    DrawRectangleRec(completeZone, {50, 205, 50, 178});      
}