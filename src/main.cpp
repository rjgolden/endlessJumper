#include <raylib.h>
#include <algorithm> 
#include <array>
#include <iostream>
#include "gameCamera.h"

constexpr int screenWidth{270};
constexpr int screenHeight{480};
constexpr int startScreenWidth{540};
constexpr int startScreenHeight{960};
constexpr int halfScreenWidth{135};
constexpr int halfScreenHeight{240};
constexpr float gravity{1200.0f};
constexpr float jump{-620.0f};
constexpr float speed{220.0f};


void drawLight(Vector2 position, float radius, Color color)
{
    DrawCircleGradient(
        static_cast<int>(position.x),
        static_cast<int>(position.y),
        radius,
        color,
        BLANK
    );
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
        SetWindowSize(startScreenWidth, startScreenHeight);
    } 
}


int main(){
    
    // initial setup
    InitWindow(startScreenWidth, startScreenHeight, "endlessJumper");
    SetTargetFPS(120);
    std::array<Texture2D, 5> backgrounds = {
        LoadTexture("src/resources/background.png"),
        LoadTexture("src/resources/background2.png"),
        LoadTexture("src/resources/background3.png"),
        LoadTexture("src/resources/background4.png"),
        LoadTexture("src/resources/background5.png"),
    }; 
    Texture2D* currentBackground = &backgrounds[0];
    Texture2D* nextBackground = &backgrounds[1];
    float scale{1.0f};
    float offsetX{0.0f};
    float offsetY{0.0f};
    float scoreHeight{0};

    // player
    Rectangle player{screenWidth / 2.0f - 16.0f, 360.0f, 32.0f, 32.0f};
    float bgY{0.0f};
    float bgY2{-480.0f};
    float scrollSpeed{120.0f};
    float scoreSpeed{2.0f}; // 2 per second
    bool changeBG{false};
    int i{2};
    int nextLevel{20};

    // camera
    GameCamera gameCamera;
    gameCamera.camera.zoom = 1.0f;
    gameCamera.camera.offset = {static_cast<float>(halfScreenWidth), 
                                static_cast<float>(halfScreenHeight)};

    // light map
    RenderTexture2D lightMap = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
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
        if (IsKeyDown(KEY_LEFT)) player.x -= speed * dt;
        if (IsKeyDown(KEY_RIGHT)) player.x += speed * dt;
        if (IsKeyDown(KEY_UP)) player.y -= speed * dt;
        if (IsKeyDown(KEY_DOWN)) player.y += speed * dt;

        if(player.x > screenWidth + 32.0f) player.x = -64.0f;
        if(player.x < -64.0f) player.x = screenWidth + 32.0f;

        if(player.y < 96) {
            scrollSpeed = 240.0f; 
            scoreSpeed = 4.0f;
        }
        else {
            scrollSpeed = 120.0f; 
            scoreSpeed = 2.0f;
        }

        // move background downward and loop back
        bgY += scrollSpeed * dt;
        if (bgY >= screenHeight) bgY -= screenHeight;

        if(changeBG){
            bgY2 += scrollSpeed *dt;
            if (bgY2 >= 0.0f) {
                changeBG = false;
                currentBackground = nextBackground;
                nextBackground = &backgrounds[i];
                i+=1;
                if(i > 4) i = 0;
                bgY2 = -480.0f;
                std::cout << "background finished setting" << "\n";
            }
        }
        // 2 per second
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

        // third pass - world space
        BeginTextureMode(target);  
            ClearBackground(BLACK);

            // 2D mode - where game is drawn to virtual screen   
            BeginMode2D(gameCamera.camera); 
                ClearBackground(BLACK);
                DrawTexture(*currentBackground, 0, static_cast<int>(bgY), WHITE);
                DrawTexture(*currentBackground, 0, (static_cast<int>(bgY) - screenHeight), WHITE);
                
                if(changeBG) {
                    DrawTexture(*nextBackground, 0, bgY2, WHITE);
                    DrawTexture(*nextBackground, 0, (static_cast<int>(bgY2) - screenHeight), WHITE);
                }

                DrawRectangle(player.x, player.y, player.width, player.height, RED);

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

            scale = std::max(1.0f, std::min(static_cast<float>(GetRenderWidth())  / static_cast<float>(screenWidth),
                                            static_cast<float>(GetRenderHeight()) / static_cast<float>(screenHeight)));      
                                                 
            offsetX = (static_cast<float>(GetRenderWidth()) - static_cast<float>(screenWidth) * scale) / 2.0f;
            offsetY = (static_cast<float>(GetRenderHeight()) - static_cast<float>(screenHeight) * scale) / 2.0f;
            
            // final draw to screen
            DrawTexturePro(
                target.texture,
                { 0.0f, 0.0f, static_cast<float>(target.texture.width), -static_cast<float>(target.texture.height) },
                { offsetX, offsetY, static_cast<float>(screenWidth) * scale, static_cast<float>(screenHeight) * scale },
                { 0.0f, 0.0f },
                0.0f,
                WHITE
            );
        EndDrawing();
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}