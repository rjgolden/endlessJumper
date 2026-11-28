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
    shopBackground = LoadTexture("src/resources/shopBackground.png");
    wallTile = LoadTexture("src/resources/wallTile.png");
}

Floor::~Floor(){
    UnloadRenderTexture(lightMap);
    UnloadRenderTexture(target);
    UnloadTexture(background);
    UnloadTexture(shopBackground);
    UnloadTexture(wallTile);
}

void Floor::drawBackground(GameCamera& gameCamera){

    ClearBackground(BLACK);
    DrawTextureEx(shopBackground, {-Globals::screenWidth, 0}, 0.0f, 1.0f, WHITE);
    //DrawRectangle(Globals::leftEdge, -static_cast<int>(Globals::screenHeight)*10, Globals::middleWidth, static_cast<int>(Globals::screenHeight)*13, LIGHTGREEN); // Middle    
    float parallax = gameCamera.camera.target.y * 0.1f;
    float backgroundPlacement = Globals::ceilingHeight;
    for(int i{0}; i<12; i++){
        DrawTextureEx(background, {0, backgroundPlacement - parallax}, 0.0f, 1.0f, WHITE);
        backgroundPlacement += Globals::screenHeight;
    }
    DrawRectangle(0, -static_cast<int>(Globals::screenHeight)*10, Globals::edgeWidth, static_cast<int>(Globals::screenHeight)*13, TAN);          // First  
    DrawRectangle(Globals::rightEdge, -static_cast<int>(Globals::screenHeight)*10, Globals::edgeWidth, static_cast<int>(Globals::screenHeight)*13, TAN);        // Last  
    DrawRectangle(Globals::leftEdge, static_cast<int>(Globals::floorY), Globals::middleWidth, static_cast<int>(Globals::floorHeight)*10, BLACK);                 // Floor

    /*
    float backgroundPlacement = Globals::ceilingHeight;
    for(int i{0}; i<12; i++){
        DrawTexture(background, 0, backgroundPlacement, WHITE);
        backgroundPlacement += Globals::screenHeight;
    }
    
    float tilePlacement = Globals::ceilingHeight;
    for(int i{0}; i<15*13; i++){
        DrawTexture(wallTile, 0, tilePlacement, WHITE); // left side
        DrawTexture(wallTile, Globals::rightEdge, tilePlacement, WHITE); // right side
        tilePlacement += wallTile.height;
    }

     DrawRectangle(Globals::leftEdge, static_cast<int>(Globals::floorY), 640, static_cast<int>(Globals::floorHeight)*10, BLACK); // Floor   
     DrawRectangle(0, Globals::ceilingHeight - Globals::floorHeight, static_cast<int>(Globals::screenWidth), Globals::floorHeight, RED); // End goal
     */
}

void Floor::resetGame(Player& player, GameCamera& gameCamera){

    platforms.resetPlatforms();
    collectibles.resetCollectibles();
    enemies.resetEnemies();
    particles.resetParticles();
    player.position = {480.0f, 440.0f};
    gameCamera.camera.target = {(Globals::screenWidth/2), (Globals::screenHeight/2)};
    score = 0.0f;
    biggestY = 440.0f;
    deaths += 1;
    ascending = true;

}

void Floor::update(Player& player, GameCamera& gameCamera){

    // Controls
    float deltaTime = GetFrameTime();
    if(IsKeyPressed(KEY_F11)) Functions::toggleFullscreenWindow();
    if(IsKeyPressed(KEY_G)) gameCamera.shakeCamera();
    if(IsKeyPressed(KEY_R)) resetGame(player, gameCamera);
    if(IsKeyPressed(KEY_M)) {
        if(player.debugMovement) player.debugMovement = false;
        else player.debugMovement = true;
    }

    // Match camera speed to player velocity
    float speed = fabs(player.velocity.y);
    float followSpeed = 1.55f + speed * 0.15f;
    followSpeed = std::clamp(followSpeed, 1.55f, 8.0f);

    // Get biggest current y for camera to follow
    float targetY = ascending ? biggestY : player.position.y;
    if(player.position.y < biggestY) biggestY = player.position.y;

    // Check if player reached top (or current boundary)
    if(player.position.y < getEndHeight() + 64.0f) ascending = false;
    
    // Check for death boundary (bottom of screen)
    Vector2 playerScreenPos = GetWorldToScreen2D(player.position, gameCamera.camera);
    if(playerScreenPos.y > Globals::screenHeight && ascending) {
            resetGame(player, gameCamera);
            score -= 5.0f;
    }

    enemies.update(player.position);
    
    // Follow target or player 
    if(player.debugMovement){
        gameCamera.camera.target.x += (player.position.x - gameCamera.camera.target.x) * deltaTime;
        gameCamera.camera.target.y += (player.position.y - gameCamera.camera.target.y) * deltaTime;
    }
    else if(!player.inDoorArea)  gameCamera.camera.target.y += (targetY - gameCamera.camera.target.y) * (followSpeed * deltaTime);
}

float Floor::getEndHeight(){
    return endHeight;
}

void Floor::drawCompleteZone(){
    DrawRectangleRec(completeZone, {50, 205, 50, 178});      
}