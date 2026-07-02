#include "player.h"

/*-------------------------------*/
Player::Player() {

    // std::array derived from animation class
    m_animationTextures[0] = LoadTexture("src/resources/starIdle.png"); 
    m_animationTextures[1] = LoadTexture("src/resources/starMoveLeft.png");
    m_animationTextures[2] = LoadTexture("src/resources/starMoveRight.png");
    m_animationTextures[3] = LoadTexture("src/resources/starJump.png");
    m_currentTexture = &m_animationTextures[0]; // default is idle

    // derived from animation
    m_frameCount = 5;
    //m_defaultWidth = static_cast<float>(m_animationTextures[0].width) / static_cast<float>(m_frameCount);
    //m_defaultHeight = static_cast<float>(m_animationTextures[0].height);

    // rectangles - derived from animation class
    m_animationRect = { 0.0f, 0.0f, 32.0f, 32.0f }; 
    m_hitboxRect = { 0.0f, 0.0f, 32.0f, 32.0f };

    // position - derived from animation class
    m_positionX = static_cast<float>(Global::halfScreenWidth);
    m_positionY = static_cast<float>(Global::halfScreenHeight); 

}

/*--------SPRITE-METHODS-----------*/
void Player::drawSprite() {
    DrawTexturePro(*m_currentTexture, m_animationRect, {static_cast<float>(m_positionX), static_cast<float>(m_positionY), static_cast<float>(m_animationTextures[0].width) / static_cast<float>(m_frameCount), static_cast<float>(m_animationTextures[0].height)}, {0.0f, 0.0f}, m_rotation, WHITE);
}

void Player::drawHitbox() {
    DrawRectangleLines(m_positionX, m_positionY, 32.0f, 32.0f, RED);
}

void Player::setState(int newState){

    m_updateTime = 1.0f / m_animationTime;

    if (newState != m_currentState) {  

        m_currentState = newState;

        switch(m_currentState) {
            case LEFT: m_currentTexture = &m_animationTextures[1]; m_animationTime = 12.0f; break;  // left
            case RIGHT: m_currentTexture = &m_animationTextures[2]; m_animationTime = 12.0f; break;  // right
            case JUMP: m_currentTexture = &m_animationTextures[3]; m_animationTime = 12.0f; break;  // up
            case SPIN: m_currentTexture = &m_animationTextures[0]; m_animationTime = 12.0f; break;  // down
            default: m_currentTexture = &m_animationTextures[0]; m_animationTime = 12.0f; break; // idle 
        }
    }

}

/*-------------------------------*/
void Player::handleLogic(){
    //float dt = GetFrameTime();
    if(m_positionX > Global::screenWidth + 32.0f) m_positionX = -64.0f;
    if(m_positionX < -64.0f) m_positionX= Global::screenWidth + 32.0f;

    if(m_positionX > Global::screenWidth + 32.0f) m_positionX = -64.0f;
    if(m_positionX  < -64.0f) m_positionX  = Global::screenWidth + 32.0f;
}

void Player::handleKeyboardMovement(){
    bool wasIdle = m_idle; 
    m_state = IDLE;
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_A)) {
        m_idle = false;
        m_state = LEFT; 
        m_positionX -= m_playerSpeed * dt;
    }
    else if (IsKeyDown(KEY_D)){ 
        m_idle = false;
        m_state = RIGHT;
        m_positionX += m_playerSpeed * dt;
    }

    if (IsKeyDown(KEY_SPACE)){ 
        m_idle = false;
        m_state = JUMP;
        m_positionY -= m_playerSpeed * dt;
    }

    // Reset animation if state changed
    if (wasIdle != m_idle) {
        m_currentFrame = 0;
        m_runningTime = 0.0f;
    }
    
}

void Player::handleControllerMovement(){
    
   
}

void Player::updateSprite() {

    handleKeyboardMovement();
    //if(IsGamepadAvailable(0)){
        //handleControllerMovement();
    //}
    setState(m_state);  
    animateSprite();
    drawSprite(); 
    drawHitbox();

}

/*-------------GETTERS------------*/
float Player::getPlayerSpeed(){
    return m_playerSpeed;
}
