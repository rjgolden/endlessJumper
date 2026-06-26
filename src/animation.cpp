#include "animation.h"

/*-------------------------------*/
Animation::Animation(Texture2D texture, uint8_t frameCount, float positionX, float positionY, bool random){

    // load textures
    m_texture = texture;

    m_defaultWidth = static_cast<float>(m_texture.width) / static_cast<float>(frameCount);
    m_defaultHeight = static_cast<float>(m_texture.height);

    // rectangles 
    m_animationRect = { 0.0f, 0.0f, m_defaultWidth, m_defaultHeight }; 
    m_hitboxRect = { 0.0f, 0.0f, m_defaultWidth, m_defaultHeight };

    // frame stuff
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_random = random;
    
    // position and speed
    m_positionX = positionX; 
    m_positionY = positionY; 

}

Animation::Animation() {}

Animation::~Animation() {}

/*--------SPRITE-METHODS-----------*/
void Animation::animateSprite(){
    float deltaTime = GetFrameTime();
    m_runningTime += deltaTime;
    if (m_runningTime >= m_updateTime){
        m_runningTime = 0.0f;
        m_animationRect.x = static_cast<float>(m_currentFrame) * m_animationRect.width;
        m_currentFrame++;
        if (m_currentFrame > m_frameCount) m_currentFrame = 0; 
    }
}

void Animation::animateSpriteRandom(){
    float deltaTime = GetFrameTime();
    m_runningTime += deltaTime;
    if (m_runningTime >= m_updateTime){
        m_runningTime = 0.0f;
        m_animationRect.x = static_cast<float>(m_currentFrame) * m_animationRect.width;
        m_currentFrame++;
        if (m_currentFrame > m_frameCount) m_currentFrame = rand() % m_frameCount; // for random  
    }
}

void Animation::drawSprite(){
    DrawTexturePro(m_texture, m_animationRect, {m_positionX, m_positionY, static_cast<float>(m_texture.width) / static_cast<float>(m_frameCount), static_cast<float>(m_texture.height)}, {0.0f, 0.0f}, m_rotation, WHITE);
}

void Animation::drawAnimationBoxLines(){
    DrawRectangleLines(m_positionX, m_positionY, m_animationRect.width, m_animationRect.height, GREEN);
}

void Animation::drawHitbox(){
    DrawRectangleLines(m_positionX + m_offsetX, m_positionY + m_offsetY, m_hitboxRect.width, m_hitboxRect.height, RED);
}

void Animation::updateSprite(){
    if(m_random) animateSpriteRandom();
    else animateSprite();
    drawSprite();
    drawHitbox();
    drawAnimationBoxLines();
}

/*-------------GETTERS------------*/
float Animation::getPositionX() { 
    return m_positionX;
}

float Animation::getPositionY() { 
    return m_positionY;
}

float Animation::getWidth() { 
    return m_hitboxRect.width; 
}

float Animation::getHeight() { 
    return m_hitboxRect.height; 
}

Vector2 Animation::getPosition() { 
    return Vector2{m_positionX, m_positionY};
}

Vector2 Animation::getDefaultDimensions(){
    return Vector2{m_defaultWidth, m_defaultHeight};
}

Rectangle Animation::getHitboxRect() { 
    return Rectangle{m_positionX + m_offsetX, m_positionY + m_offsetY, m_hitboxRect.width, m_hitboxRect.height};
}

/*-------------SETTERS------------*/
void Animation::setPosition(Vector2 position){
    m_positionX = position.x;
    m_positionY = position.y;
}

void Animation::setRotation(float rotation){
    m_rotation = rotation;
}

void Animation::setHitboxDimensions(Vector2 dimensions){
    m_hitboxRect.width = dimensions.x;
    m_hitboxRect.height = dimensions.y;
}

void Animation::setHitboxOffset(Vector2 pos){
    m_offsetX = pos.x;
    m_offsetY = pos.y;
}