#include <iostream>
#include <algorithm>
#include "raylib.h"
#include "functions.hpp"
#include "palette.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "floor.hpp"
#include "gameCamera.hpp"

int main()
{

    InitWindow(Globals::screenWidth, Globals::screenHeight, "meatJumpShooter");
    SetTargetFPS(144);

    Floor floor;
    Player player;
    GameCamera gameCamera;

    float score{0.0f};
    float scale{1.0f};
    float offsetX{0.0f};
    float offsetY{0.0f};

    RenderTexture2D lightMap = LoadRenderTexture(Globals::screenWidth, Globals::screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    RenderTexture2D target = LoadRenderTexture(Globals::screenWidth, Globals::screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    while (!WindowShouldClose())
    {   
        // ======================================================= //
        // FIRST PASS - GAME LOGIC                                 //
        // ======================================================= //
        
        float deltaTime = GetFrameTime();
        if(IsKeyPressed(KEY_F11)) Functions::toggleFullscreenWindow();
        if(IsKeyPressed(KEY_G)) gameCamera.shakeCamera();
        if(IsKeyPressed(KEY_H)) floor.resetGame(player, gameCamera, score);

        score +=  1.0f/10.0f;
        player.update(floor.platforms.platformArr);

        Vector2 playerScreenPos = GetWorldToScreen2D(player.position, gameCamera.camera);

        if(playerScreenPos.y > Globals::screenHeight && floor.ascending) {
            floor.resetGame(player, gameCamera, score);
        }

        if(player.position.y < floor.biggestY) {
            floor.biggestY = player.position.y;
        }

        if(player.position.y < floor.getEndHeight() + 64.0f) {
            floor.ascending = false;
        }

        float speed = fabs(player.velocity.y);
        float followSpeed = 1.55f + speed * 0.15f;
        followSpeed = std::clamp(followSpeed, 1.55f, 8.0f);
        float targetY = floor.ascending ? floor.biggestY : player.position.y;

        gameCamera.camera.target.y += (targetY - gameCamera.camera.target.y) * (followSpeed * deltaTime);
        
        // ======================================================== //
        // SECOND PASS - CREATING LIGHT MAP                         //
        // ======================================================== //

        BeginTextureMode(lightMap);
        // {
            ClearBackground(LIGHTGRAY); // screen tint (black for complete dark, etc.)
            BeginBlendMode(BLEND_ADDITIVE); 
            // {
                Functions::drawLightCircle({GetWorldToScreen2D(player.position, gameCamera.camera)}, 115.0f, Fade(YELLOW, 0.60f), BLANK);
            // }
            EndBlendMode();
        // }
        EndTextureMode();

        // ========================================================= //
        // THIRD PASS - CREATING TARGET TEXTURE (DRAWNING to SCREEN) //
        // ========================================================= //

        BeginTextureMode(target);
        // {
            // 2D mode - where game is drawn to virtual screen
            BeginMode2D(gameCamera.camera);

                floor.drawBackground();
                floor.platforms.drawPlatforms();   
                floor.drawCompleteZone();       
                player.drawPlayer();
                player.drawBullets();
            
            EndMode2D();
        
            // Light/pixel blending
            BeginBlendMode(BLEND_MULTIPLIED);
            // {
                DrawTextureRec(lightMap.texture, {0.0f,0.0f, static_cast<float>(lightMap.texture.width), -static_cast<float>(lightMap.texture.height)}, 
                              {0.0f, 0.0f}, WHITE);
            // }
            EndBlendMode();
        // }
        EndTextureMode();

        // ======================================================== //
        // FOURTH PASS - DRAW COMPLETED GAME TEXTURE                //
        // ======================================================== //

        BeginDrawing();
        // {
            ClearBackground(BLACK);

            scale = std::max(1.0f, std::min(static_cast<float>(GetRenderWidth())  / static_cast<float>(Globals::screenWidth),
                                            static_cast<float>(GetRenderHeight()) / static_cast<float>(Globals::screenHeight)));      
                                                
            offsetX = (static_cast<float>(GetRenderWidth()) - static_cast<float>(Globals::screenWidth) * scale) / 2.0f;
            offsetY = (static_cast<float>(GetRenderHeight()) - static_cast<float>(Globals::screenHeight) * scale) / 2.0f;
        
            // final draw to screen
            DrawTexturePro (
                target.texture,
                { 0.0f, 0.0f, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height) },
                { offsetX, offsetY, static_cast<float>(Globals::screenWidth) * scale, static_cast<float>(Globals::screenHeight) * scale },
                { 0.0f, 0.0f },
                0.0f,
                WHITE );

            // UI (temp)
            DrawText(TextFormat("Score: %d", static_cast<int>(score)), 20*scale, 20*scale, 20*scale, YELLOW);
            DrawText(TextFormat("Deaths: %d", static_cast<int>(floor.deaths)), 20*scale, 40*scale, 20*scale, YELLOW);
            DrawText(TextFormat("Player Y: %d", static_cast<int>(player.position.y)), 20*scale, 60*scale, 20*scale, YELLOW);
            DrawText("Ascending", 20*scale, 80*scale, 20*scale, floor.ascending ? GREEN : RED);
            DrawText("F11: Fullscreen", 10*scale, 100*scale, 20*scale, YELLOW);

        // }
        EndDrawing();
    }

    UnloadRenderTexture(lightMap);
    UnloadRenderTexture(target);

    CloseWindow();
    return 0;
}