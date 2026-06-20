#include <raylib.h>
#include <algorithm> 
#include <array>
#include <vector>
#include <iostream>
#include "gameCamera.h"
#include "global.h"

void drawLight(Vector2 position, float radius, Color color)
{
    DrawCircleGradient(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        radius,
        color,
        BLANK );
}

void drawSideArt(int screenW, int screenH)
{
    for (int y = 0; y < screenH; y += 40)
    {
        DrawRectangle(0, y, screenW, 20, Color{45, 45, 70, 255});
    }
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

int main(){
    
    // initial setup
    InitWindow(Global::startScreenWidth, Global::startScreenHeight, "endlessJumper");
    SetTargetFPS(120);

    // screen
    float scale{1.0f};
    float offsetX{0.0f};
    float offsetY{0.0f};

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
    bool changeBG{false};
    int backgroundIndex{2}; // init 2 for next background
    int nextLevel{20};

    // player
    Rectangle player{Global::screenWidth / 2.0f - 16.0f, 360.0f, 32.0f, 32.0f};

    // camera
    GameCamera gameCamera;
    gameCamera.camera.zoom = 1.0f;
    gameCamera.camera.offset = {static_cast<float>(Global::halfScreenWidth), 
                                static_cast<float>(Global::halfScreenHeight)};

    // platforms
    std::vector<Rectangle> platforms;
    float nextPlatformY{500};
    for (int i = 0; i < 10; i++) {
        platforms.push_back({
            static_cast<float>(GetRandomValue(20, 160)), // x
            400.0f - static_cast<float>(i * 160),               // y
            static_cast<float>(GetRandomValue(50, 100)),                            // width
            20.0f                              // height
        });
    }

    nextPlatformY = platforms.back().y - 80.0f;

    // light map
    RenderTexture2D lightMap = LoadRenderTexture(Global::screenWidth, Global::screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    RenderTexture2D target = LoadRenderTexture(Global::screenWidth, Global::screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    while (!WindowShouldClose()){

        // first pass - game logic
        if(IsKeyPressed(KEY_F)) toggleFullscreenWindow();

        // mouse for world
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = {
            (mouse.x - offsetX) / scale,
            (mouse.y - offsetY) / scale
        };

        // player
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_LEFT)) player.x -= Global::playerSpeed * dt;
        if (IsKeyDown(KEY_RIGHT)) player.x += Global::playerSpeed * dt;
        if (IsKeyDown(KEY_UP)) player.y -= Global::playerSpeed * dt;
        if (IsKeyDown(KEY_DOWN)) player.y += Global::playerSpeed * dt;

        if(player.x > Global::screenWidth + 32.0f) player.x = -64.0f;
        if(player.x < -64.0f) player.x = Global::screenWidth + 32.0f;

        if(player.y < 96) {
            scrollSpeed = 240.0f; 
            scoreSpeed = 4.0f;
        }
        else {
            scrollSpeed = 120.0f; 
            scoreSpeed = 2.0f;
        }

        // move background downward and loop back
        // current background
        bgY += scrollSpeed * dt;
        if (bgY >= Global::screenHeight) bgY -= Global::screenHeight;

        // incoming background
        if(changeBG){
            bgY2 += scrollSpeed *dt;
            if (bgY2 >= 0.0f) {
                changeBG = false;
                currentBackground = nextBackground;
                nextBackground = &backgrounds[backgroundIndex];
                backgroundIndex++;
                if(backgroundIndex > 4) backgroundIndex = 0; // loop through backgrounds (temp for now)
                bgY2 = -480.0f; // reset position for next level change
            }
        }

        scoreHeight += scoreSpeed * dt;
        if(static_cast<int>(scoreHeight) >= nextLevel) {
            changeBG = true;
            std::cout << nextLevel << "m\n";
            nextLevel += 20;
            std::cout << "next level: " << nextLevel << "\n";
        }

        // second pass - lightMap
        BeginTextureMode(lightMap);
            ClearBackground(LIGHTGRAY); // screen tint (black for complete dark, etc.)
            BeginBlendMode(BLEND_ADDITIVE); 
                drawLight(virtualMouse, 150.0f, Color{255, 240, 200, 255});
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
                
                if(changeBG) {
                    DrawTexture(*nextBackground, 0, static_cast<int>(bgY2), WHITE);
                    DrawTexture(*nextBackground, 0, (static_cast<int>(bgY2) - Global::screenHeight), WHITE);
                }

                DrawRectangle(player.x, player.y, player.width, player.height, RED);
            
                for (Rectangle& platform : platforms)
                {
                    // If platform is below the camera view
                    if (platform.y > Global::screenHeight)
                    {
                        // Move it above the screen
                        platform.x = static_cast<float>(GetRandomValue(20, 160));
                        platform.y = nextPlatformY;

                        nextPlatformY -= 80.0f;
                    }
                }

                for (Rectangle& platform : platforms) {
                    platform.y += scrollSpeed * dt;
                    DrawRectangleRec(platform, DARKGREEN);
                }

                DrawText(TextFormat("Height: %i", static_cast<int>(scoreHeight)), 20, 20, 20, YELLOW);
                
                DrawText(TextFormat("FPS: %i", GetFPS()), 160, 20, 20, RED);
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