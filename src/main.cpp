#include <algorithm>
#include <cmath>
#include "raylib.h"
#include "palette.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "floor.hpp"
#include "gameCamera.hpp"
#include "enemies.hpp"
#include "particles.hpp"

int main() {

    InitWindow(Globals::screenWidth, Globals::screenHeight, "meatJumpShooter");
    SetTargetFPS(144);

    Player player;
    GameCamera gameCamera;
    Floor floor;
   
    float scale{1.0f};
    float offsetX{0.0f};
    float offsetY{0.0f};

    while (!WindowShouldClose()) {   
        
        // ======================================================= //
        // FIRST PASS - GAME LOGIC                                 //
        // ======================================================= //

        floor.update(player, gameCamera);
        player.update(floor, gameCamera);

        
        if(IsMouseButtonPressed(1)) {
            float x = GetMouseX();
            float y = GetMouseY();
            floor.particles.explodeNuggets({x,y}, WHITE, 0);
        }


        // ======================================================== //
        // SECOND PASS - CREATING LIGHT MAP                         //
        // ======================================================== //

        BeginTextureMode(floor.lightMap);
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

        BeginTextureMode(floor.target);
        // {
            // 2D mode - where game is drawn to virtual screen
            BeginMode2D(gameCamera.camera);

                ClearBackground(BLACK);
                floor.drawBackground(gameCamera);
                floor.platforms.drawPlatforms();
                floor.doors.drawDoors();
                floor.collectibles.drawCollectibles();
                floor.enemies.drawEnemies();
                floor.particles.drawParticles(floor);
                floor.drawCompleteZone();       
                player.drawPlayer();
                player.drawBullets();
            
            EndMode2D();
        
            // Draw light map
            BeginBlendMode(BLEND_MULTIPLIED);
            // {
                DrawTextureRec(floor.lightMap.texture, 
                              {0.0f, 0.0f, static_cast<float>(floor.lightMap.texture.width), 
                              -static_cast<float>(floor.lightMap.texture.height)}, 
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
                floor.target.texture,
                { 0.0f, 0.0f, static_cast<float>(floor.target.texture.width), -static_cast<float>(floor.target.texture.height) },
                { offsetX, offsetY, static_cast<float>(Globals::screenWidth) * scale, static_cast<float>(Globals::screenHeight) * scale },
                { 0.0f, 0.0f },
                0.0f,
                WHITE );

            // UI (temp)
            DrawText(TextFormat("Score: %d", static_cast<int>(floor.score)), 20*scale, 20*scale, 20*scale, YELLOW);
            DrawText(TextFormat("Deaths: %d", static_cast<int>(floor.deaths)), 20*scale, 40*scale, 20*scale, YELLOW);
            DrawText(TextFormat("Player Y: %d", static_cast<int>(player.position.y)), 20*scale, 60*scale, 20*scale, YELLOW);
            DrawText("Ascending", 20*scale, 80*scale, 20*scale, floor.ascending ? GREEN : RED);
            DrawText("F11: Fullscreen", 10*scale, 100*scale, 20*scale, YELLOW);
            DrawText("Debug Mode", 20*scale, 120*scale, 20*scale, player.debugMovement ? GREEN : RED);

        // }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}