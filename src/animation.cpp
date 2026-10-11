#include "animation.h"

/*-------------------------------*/
Animation::Animation(Texture2D texture, Vector2 position, int frameCount, int width, int height, bool random){

    // load texturess
    m_texture = texture;

    // rectangles 
    m_animationRect = { 0.0f, 0.0f, width, height }; 
    m_hitboxRect = { 0.0f, 0.0f, width, height };

    // frame stuff
    m_frameCount = frameCount; // Number of frames in the idle animation
    m_random = random;
    
    // position and speed
    m_position.x = position.x; 
    m_position.x = position.y; 

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
    DrawTexturePro(m_texture, 
                   m_animationRect, 
                   {m_position.x, m_position.y, static_cast<float>(m_texture.width) / static_cast<float>(m_frameCount), static_cast<float>(m_texture.height)}, 
                   {0.0f, 0.0f}, 
                   m_rotation, 
                   WHITE);
}

void Animation::drawHitbox(){
    DrawRectangleLines(m_position.x + m_offset.x, m_position.y + m_offset.y, m_hitboxRect.width, m_hitboxRect.height, RED);
}

void Animation::updateSprite(){
    if(m_random) animateSpriteRandom();
    else animateSprite();
    drawSprite();
    drawHitbox();
}

