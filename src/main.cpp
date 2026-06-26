#include <raylib.h>
#include <algorithm> 
#include <array>
#include <iostream>
#include "gameCamera.h"
#include "global.h"
#include "animation.h"

void drawLight(Vector2 position, float radius, Color color) {
    DrawCircleGradient(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        radius,
        color,
        BLANK );
}

void drawSideArt(int screenW, int screenH) {
    static Texture2D sideArt = LoadTexture("src/resources/sideArt.png");
    DrawTexturePro( sideArt, 
                    {0.0f, 0.0f, static_cast<float>(screenW), static_cast<float>(screenH)}, 
                    {0.0f, 0.0f, static_cast<float>(sideArt.width), static_cast<float>(sideArt.width)}, 
                    {0.0f, 0.0f}, 
                    0.0f, 
                    WHITE);
    /*for (int y = 0; y < screenH; y += 40)s
    {
        DrawRectangle(0, y, screenW, 20, RED);
        DrawRectangle(0, y+20, screenW, 20, WHITE);
    }*/
}

void toggleFullscreenWindow(){
    if (!IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else {
        ToggleFullscreen();
        SetWindowSize(Global::startScreenWidth, Global::startScreenHeight);
    } 
}

void resetPlatforms(auto& platforms){
    for (size_t i{0}; i < platforms.size(); i++) {
        platforms[i].rect = {
            static_cast<float>(GetRandomValue(10, 170)), 
            400.0f - (static_cast<float>(i)*30.0f*3) - 30.0f,               
            static_cast<float>(GetRandomValue(20, 70)),                          
            10.0f                              
        };
        platforms[i].colorIndex = 0;
    }
}

struct Platform {
    Rectangle rect;
    int colorIndex;
};

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
    Rectangle player{Global::screenWidth / 2.0f - 16.0f, 360.0f, 32.0f, 32.0f};
    Texture2D playerTexture = LoadTexture("src/resources/player.png"); 
    Texture2D fireAnimation = LoadTexture("src/resources/fireSpriteAnimation.png");
    Animation fire(fireAnimation, 6, Global::screenWidth / 2.0f - 16.0f, 360.0f, true);
    bool inAir{true};
    float jumpVelocity{0.0f};
    float gravity{1500.0f};
    bool onPlatform{false};
    bool gameOver{false};

    // camera
    GameCamera gameCamera;
    gameCamera.camera.zoom = 1.0f;
    gameCamera.camera.offset = {static_cast<float>(Global::halfScreenWidth), 
                                static_cast<float>(Global::halfScreenHeight)};

    // platforms
    int currentColorIndex{0};
    int nextColorIndex{1};
    float spacing{30.0f};
    std::array<Color, 5> platformColors = {GRAY, DARKBLUE, YELLOW, PINK, BROWN};
    std::array<Platform, 20> platforms;
    for (size_t i{0}; i < platforms.size(); i++) {
        platforms[i].rect = {
            static_cast<float>(GetRandomValue(10, 170)), 
            400.0f - (static_cast<float>(i)*spacing*3) - spacing,               
            static_cast<float>(GetRandomValue(20, 70)),                          
            10.0f                              
        };
        platforms[i].colorIndex = 0;
    }
    float nextPlatformY = platforms.back().rect.y - spacing;

    // light map
    RenderTexture2D lightMap = LoadRenderTexture(Global::screenWidth, Global::screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    RenderTexture2D target = LoadRenderTexture(Global::screenWidth, Global::screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    // ------------------------ //
    while (!WindowShouldClose()){

        // first pass - game logic
        if(IsKeyPressed(KEY_F11)) toggleFullscreenWindow();

        if(IsKeyPressed(KEY_R) && gameOver) {
            resetPlatforms(platforms);
            scoreHeight = 0;
            isScrolling = false;
            gameOver = false;
            screenTint = LIGHTGRAY;
            currentBackground = &backgrounds[0];
            nextBackground = &backgrounds[1];
            backgroundIndex = 1;
            currentColorIndex = 0;
            nextColorIndex = 1;
            bgY = 0.0f;
            bgY2 = -480.0f;
            nextLevel = 20;
        }

        // mouse for world
        /*Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {
            (mouse.x - offsetX) / scale,
            (mouse.y - offsetY) / scale
        };*/

        if(!gameOver){
            // player
            float dt = GetFrameTime();
            if (IsKeyDown(KEY_A)) player.x -= Global::playerSpeed * dt;
            if (IsKeyDown(KEY_D)) player.x += Global::playerSpeed * dt;

            if(player.x > Global::screenWidth + 32.0f) player.x = -64.0f;
            if(player.x < -64.0f) player.x = Global::screenWidth + 32.0f;

            fire.setPosition({player.x, player.y -  16.0f});

            if(player.y < 96) {
                scrollSpeed = 240.0f; 
                scoreSpeed = 4.0f;
            }
            else {
                scrollSpeed = 120.0f; 
                scoreSpeed = 2.0f;
            }
            
            if(IsKeyPressed(KEY_SPACE) && !inAir){
                inAir = true;
                jumpVelocity -= 600.0f;
                isScrolling = true;
            }

            float previousPlayerY = player.y;
            if(inAir || onPlatform) jumpVelocity += gravity * dt;
            player.y += jumpVelocity * dt;

            if(!isScrolling){
                if(player.y >= Global::screenHeight - 32.0f){
                    player.y = Global::screenHeight - 32.0f;
                    inAir = false;
                    jumpVelocity = 0.0f;
                }
            }
        
            if(isScrolling){
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

                        currentColorIndex = nextColorIndex;
                        nextColorIndex = backgroundIndex;

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

                if(player.y >= Global::screenHeight + 128.0f){
                    gameOver = true;
                    inAir = false;
                    jumpVelocity = 0.0f;
                }

                // platforms 
                for (Platform& platform : platforms) {
                    platform.rect.y += scrollSpeed * dt;
                                
                    // if platform is below the camera view
                    if (platform.rect.y > Global::screenHeight) {
                        platform.rect.x = static_cast<float>(GetRandomValue(10, 170));
                        platform.rect.y = nextPlatformY;
                        platform.rect.width =  static_cast<float>(GetRandomValue(20, 70)); 
                    }    

                    if (changeBackground) {
                        float transitionY = bgY2 + static_cast<float>(Global::screenHeight);
                        float platformEdgeY = platform.rect.y + platform.rect.height;
                        if (platformEdgeY < transitionY) platform.colorIndex = nextColorIndex;
                        else platform.colorIndex = currentColorIndex;
                    } 

                    float platformTop = platform.rect.y;
                    if (jumpVelocity > 0.0f && previousPlayerY + player.height <= platformTop && player.y + player.height >= platformTop && CheckCollisionRecs(player, platform.rect)){
                            player.y = platform.rect.y - player.height;
                            jumpVelocity = 0.0f;
                            inAir = false;
                            onPlatform = true;
                            jumpVelocity -= static_cast<float>(GetRandomValue(500, 700));
                    }
                }
            }
        }

        // second pass - lightMap
        BeginTextureMode(lightMap);
            ClearBackground(screenTint); // screen tint (black for complete dark, etc.)
            BeginBlendMode(BLEND_ADDITIVE); 
                //drawLight(virtualMouse, 150.0f, Color{255, 240, 200, 255});
                drawLight({player.x + 16.0f, player.y + 16.0f}, 75.0f, Color{255, 191, 0, 255});
            EndBlendMode();
        EndTextureMode();

        // third pass - creating target texture
        BeginTextureMode(target);  
            ClearBackground(BLACK);

            // 2D mode - where game is drawn to virtual screen   
            BeginMode2D(gameCamera.camera); 
                ClearBackground(BLACK);
                DrawTexture(*currentBackground, 0, static_cast<int>(bgY), WHITE);
                DrawTexture(*currentBackground, 0, (static_cast<int>(bgY) - Global::screenHeight), WHITE);
                
                if(changeBackground) {
                    DrawTexture(*nextBackground, 0, static_cast<int>(bgY2), WHITE);
                    DrawTexture(*nextBackground, 0, (static_cast<int>(bgY2) - Global::screenHeight), WHITE);
                }
    
                for (Platform& platform : platforms) {
                    DrawRectangleRec(platform.rect, platformColors[platform.colorIndex]);
                    if(CheckCollisionRecs(platform.rect, player)) {
                        DrawRectangleLinesEx(platform.rect, 1.0f, RED);  
                    }  
                }
                
                DrawTexture(playerTexture, player.x, player.y, WHITE);
                fire.updateSprite();

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

            drawSideArt(GetScreenWidth(), GetScreenWidth());

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