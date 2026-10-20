#include <iostream>
#include <algorithm>
#include "raylib.h"
#include "functions.hpp"
#include "palette.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "platforms.hpp"
#include "gameCamera.hpp"

int main()
{

    InitWindow(Globals::screenWidth, Globals::screenHeight, "meatJumpShooter");
    SetTargetFPS(144);

    { 
        Player player;
        Platforms platforms;
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
        
            if(IsKeyPressed(KEY_F11)) Functions::toggleFullscreenWindow();
            if(IsKeyPressed(KEY_G)) gameCamera.shakeCamera();
            score +=  1.0f / 10.0f;
            player.update(platforms.platformArr);
            platforms.scrollPlatforms();

            // ======================================================== //
            // SECOND PASS - CREATING LIGHT MAP                         //
            // ======================================================== //

            BeginTextureMode(lightMap);
            // {
                ClearBackground(LIGHTGRAY); // screen tint (black for complete dark, etc.)
                BeginBlendMode(BLEND_ADDITIVE); 
                // {
                    Functions::drawLightCircle({player.position.x, player.position.y}, 115.0f, Fade(YELLOW, 0.60f), BLANK);
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

                    Functions::drawBackground();
                    platforms.drawPlatforms();          
                    player.drawPlayer();
                    player.drawBullets();
                
                EndMode2D();
            
                // Light/pixel blending
                BeginBlendMode(BLEND_MULTIPLIED);
                // {
                    DrawTextureRec(lightMap.texture, 
                                   Rectangle{0.0f,0.0f, static_cast<float>(lightMap.texture.width), -static_cast<float>(lightMap.texture.height)}, 
                                   Vector2{0.0f, 0.0f},
                                   WHITE);
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
                DrawText(TextFormat("Score: %d", static_cast<int>(score)),
                         20 * scale, 20, 20 * scale, YELLOW);
                DrawText("F11: Fullscreen", 10, 80, 20 * scale, YELLOW);

            // }
            EndDrawing();
        }

        UnloadRenderTexture(lightMap);
        UnloadRenderTexture(target);
    }

    CloseWindow();
    return 0;
}