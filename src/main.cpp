#include <raylib.h>
#include <algorithm> 
#include "gameCamera.h"

constexpr int screenWidth = 270;
constexpr int screenHeight = 480;
constexpr float gravity = 1200.0f;
constexpr float jump = -620.0f;
constexpr float speed = 220.0f;
constexpr float scrollSpeed = 180.0f;

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
        SetWindowSize(screenWidth, screenHeight);
    } 
}


int main()
{
    
    // initial setup
    InitWindow(screenWidth, screenHeight, "endlessJumper");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("src/resources/background.png");
    float scale{1.0f};
    float offsetX{0.0f};
    float offsetY{0.0f};
    float scoreHeight{0};

    // player
    Rectangle player{screenWidth / 2.0f - 16.0f, 360.0f, 32.0f, 32.0f};
    float bgY = 0.0f;
    float scrollSpeed = 120.0f;

    // camera
    GameCamera gameCamera;
    gameCamera.camera.zoom = 1.0f;
    gameCamera.camera.offset = {135.0f, 240.0f};

    // light map
    RenderTexture2D lightMap = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(lightMap.texture, TEXTURE_FILTER_POINT);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); 

    while (!WindowShouldClose())
    {

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

        if(player.x > screenWidth + 32.0f) player.x = -64.0f;
        if(player.x < -64.0f) player.x = screenWidth + 32.0f;

        // move background downward and loop back
        bgY += scrollSpeed * dt;
        if (bgY >= screenHeight) bgY -= screenHeight;
        
        // 2 per second
        scoreHeight += 2.0f * dt;

        // second pass - lightMap
        BeginTextureMode(lightMap);  
            ClearBackground(LIGHTGRAY); // screen tint (black for complete dark, etc.)
            BeginBlendMode(BLEND_ADDITIVE); 
                //Vector2 screenPos = GetWorldToScreen2D({static_cast<float>(GetMouseX()), static_cast<float>(GetMouseY())}, gameCamera.camera);
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
                DrawTexture(background, 0, (int)bgY, WHITE);
                DrawTexture(background, 0, (int)(bgY - screenHeight), WHITE);
                DrawRectangle(player.x, player.y, player.width, player.height, RED);
                DrawText(TextFormat("Height: %i", (int)scoreHeight), 20, 20, 20, YELLOW);
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