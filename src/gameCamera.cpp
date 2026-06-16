#include "gameCamera.h"

GameCamera::GameCamera(){

    camera = {0};
    Vector2 middle ={ 270.0f/2.0f, 480.0f/2.0f };
    camera.target = middle;
    camera.offset = middle;
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
}