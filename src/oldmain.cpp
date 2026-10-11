#include <raylib.h>
#include <algorithm> 
#include <array>
#include <iostream>
#include "gameCamera.h"
#include "globals.h"
#include "animation.h"
#include "player.h"
#include "functions.h"
#include "platforms.h"

int main(){
    
    // initial setup
    InitWindow(Global::startScreenWidth, Global::startScreenHeight, "endlessJumper");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    // screen
    float scale{1.0f};
    float offsetX{0.0f};
    float offsetY{0.0f};
    Color screenTint = LIGHTGRAY;

    // backgrounds
    std::array<Texture2D, 5> backgrounds = {
        LoadTexture("src/resources/background.png"),
        LoadTexture("src/resources/background2.png"),
        LoadTexture("src/resources/background3.png"),
        LoadTexture("src/resources/background4.png"),
        LoadTexture("src/resources/background5.png"),
    }; 
    Texture2D* currentBackground = &backgrounds[0];
    Texture2D* nextBackground = &backgrounds[1];

    float bgY{0.0f};
    float bgY2{-480.0f};
    float scoreHeight{0};
    float scrollSpeed{120.0f};
    float scoreSpeed{2.0f}; // 2 per second
    bool changeBackground{false};
    bool isScrolling{false};
    int backgroundIndex{2}; // init 2 for next background
    int nextLevel{20};

    // player
    bool inAir{true};
    float jumpVelocity{0.0f};
    float gravity{1500.0f};
    bool onPlatform{false};
    bool gameOver{false};

    Player classPlayer;

    // camera
    GameCamera gameCamera;
    gameCamera.camera.zoom = 1.0f;
    gameCamera.camera.offset = {static_cast<float>(Global::halfScreenWidth), 
                                static_cast<float>(Global::halfScreenHeight)};

    // platforms
    Platforms platforms;
    float nextPlatformY = platforms.platformArr.back().rect.y - 30.0f;

    // light map
    RenderTexture2D lightMap = LoadRenderTexture(Global::screenWidth, Global::screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    RenderTexture2D target = LoadRenderTexture(Global::screenWidth, Global::screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    // ------------------------ //
    while (!WindowShouldClose()){

        // first pass - game logic
        if(IsKeyPressed(KEY_F11)) Functions::toggleFullscreenWindow();

        if(IsKeyPressed(KEY_R) && gameOver) {
            platforms.resetPlatforms();
            scoreHeight = 0;
            isScrolling = false;
            screenTint = LIGHTGRAY;
            currentBackground = &backgrounds[0];
            nextBackground = &backgrounds[1];
            backgroundIndex = 1;
            platforms.setCurrentColorIndex(0);
            platforms.setNextColorIndex(1);
            bgY = 0.0f;
            bgY2 = -480.0f;
            nextLevel = 20;
            gameOver = false;
        }

        // mouse for world
        /*Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {
            (mouse.x - offsetX) / scale,
            (mouse.y - offsetY) / scale
        };*/

        if(!gameOver){

            float dt = GetFrameTime();
           
            // 2x speed and score
            if(classPlayer.getPositionY() < 96) {
                scrollSpeed = 240.0f; 
                scoreSpeed = 4.0f;
            }
            else {
                scrollSpeed = 120.0f; 
                scoreSpeed = 2.0f;
            }
            
            // jump
            if(IsKeyPressed(KEY_SPACE) && !inAir){
                inAir = true;
                jumpVelocity -= 600.0f;
                isScrolling = true;
            }

            float previousPlayerY = classPlayer.getPositionY();
            if(inAir || onPlatform) jumpVelocity += gravity * dt;
            classPlayer.setPositionY(classPlayer.getPositionY() + jumpVelocity * dt);

            if(!isScrolling){
                if(classPlayer.getPositionY() >= Global::screenHeight - 32.0f){
                    classPlayer.setPositionY(Global::screenHeight - 32.0f);
                    inAir = false;
                    jumpVelocity = 0.0f;
                }
            }
            else{
                // current background
                bgY += scrollSpeed * dt;
                if (bgY >= Global::screenHeight) bgY -= Global::screenHeight;

                // incoming background
                if(changeBackground){
                    bgY2 += scrollSpeed *dt;
                    if (bgY2 >= 0.0f) {
                        changeBackground = false;
                        currentBackground = nextBackground;
                        nextBackground = &backgrounds[backgroundIndex];

                        platforms.setCurrentColorIndex(platforms.getNextColorIndex());
                        platforms.setNextColorIndex(backgroundIndex);

                        backgroundIndex++;
                        if(backgroundIndex > 4) backgroundIndex = 0; // loop through backgrounds (temp for now)
                        bgY2 = -static_cast<float>(Global::screenHeight); // reset position for next level change
                    }
                }

                // change level based on height/score
                scoreHeight += scoreSpeed * dt;
                if(!changeBackground && static_cast<int>(scoreHeight) >= nextLevel) {
                    changeBackground = true;
                    nextLevel += 20;
                }

                if(classPlayer.getPositionY() >= static_cast<int>(Global::screenHeight) + 128.0f){
                    gameOver = true;
                    inAir = false;
                    jumpVelocity = 0.0f;
                }

                if(inAir) classPlayer.setState(3);

                // platforms 
                for (auto& platform : platforms.platformArr) {
                    platform.rect.y += scrollSpeed * dt;
                                
                    // if platform is below the camera view
                    if (platform.rect.y > Global::screenHeight) {
                        platform.rect.x = static_cast<float>(GetRandomValue(10, 170));
                        platform.rect.y = nextPlatformY;
                        platform.rect.width =  static_cast<float>(GetRandomValue(20, 70)); 
                    }    

    
                }
            }
        }

        // second pass - lightMap
        BeginTextureMode(lightMap);
            ClearBackground(screenTint); // screen tint (black for complete dark, etc.)
            BeginBlendMode(BLEND_ADDITIVE); 
                //drawLight(virtualMouse, 150.0f, Color{255, 240, 200, 255});
                Functions::drawLight({classPlayer.getPositionX() + 16.0f, classPlayer.getPositionY() + 16.0f}, 75.0f, Color{255, 191, 0, 255});
            EndBlendMode();
        EndTextureMode();

        // third pass - creating target texture
        BeginTextureMode(target);  
            ClearBackground(BLACK);

            // 2D mode - where game is drawn to virtual screen   
            BeginMode2D(gameCamera.camera); 
                ClearBackground(BLACK);

                //backgrounds
                DrawTexture(*currentBackground, 0, static_cast<int>(bgY), WHITE);
                DrawTexture(*currentBackground, 0, (static_cast<int>(bgY) - Global::screenHeight), WHITE);
                if(changeBackground) {
                    DrawTexture(*nextBackground, 0, static_cast<int>(bgY2), WHITE);
                    DrawTexture(*nextBackground, 0, (static_cast<int>(bgY2) - Global::screenHeight), WHITE);
                }
                
                //platforms
                for (auto& platform : platforms.platformArr) {
                    DrawRectangleLinesEx(platform.rect, 2.0f, platforms.platformColors[platform.colorIndex]);
                    if(CheckCollisionRecs(platform.rect, classPlayer.getHitboxRect())) {
                        DrawRectangleLinesEx(platform.rect, 1.0f, RED);  
                    }  
                }
                
                //player
                classPlayer.updateSprite();

                // UI
                DrawText(TextFormat("Height: %i", static_cast<int>(scoreHeight)), 20, 20, 20, YELLOW);
                DrawText(TextFormat("FPS: %i", GetFPS()), 160, 20, 20, RED);
                if(gameOver) {
                    screenTint = MAROON;
                    DrawText("GAME OVER!", 70.0f, 240.0f, 20, BLACK); 
                }

            EndMode2D();

            BeginBlendMode(BLEND_MULTIPLIED); 
                DrawTextureRec(lightMap.texture, Rectangle{0.0f, 0.0f, static_cast<float>(lightMap.texture.width), -static_cast<float>(lightMap.texture.height)}, Vector2{0.0f, 0.0f}, RAYWHITE);
            EndBlendMode(); 
        EndTextureMode();

        // fourth pass - draw the completed virtual-resolution game texture
        BeginDrawing();
            ClearBackground(BLACK);

            Functions::drawSideArt(GetScreenWidth(), GetScreenWidth());

            scale = std::max(1.0f, std::min(static_cast<float>(GetRenderWidth())  / static_cast<float>(Global::screenWidth),
                                            static_cast<float>(GetRenderHeight()) / static_cast<float>(Global::screenHeight)));      
                                                 
            offsetX = (static_cast<float>(GetRenderWidth()) - static_cast<float>(Global::screenWidth) * scale) / 2.0f;
            offsetY = (static_cast<float>(GetRenderHeight()) - static_cast<float>(Global::screenHeight) * scale) / 2.0f;
            
            // final draw to screen
            DrawTexturePro (
                target.texture,
                { 0.0f, 0.0f, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height) },
                { offsetX, offsetY, static_cast<float>(Global::screenWidth) * scale, static_cast<float>(Global::screenHeight) * scale },
                { 0.0f, 0.0f },
                0.0f,
                WHITE );

        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}