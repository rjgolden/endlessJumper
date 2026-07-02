#include "gameCamera.h"

GameCamera::GameCamera(){

    camera = {0};
    Vector2 middle ={ Global::halfScreenWidth, Global::halfScreenHeight };
    camera.target = middle;
    camera.offset = middle;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
}