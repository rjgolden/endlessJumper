#include "player.hpp"
#include <algorithm>
#include <cmath>
#include "raymath.h"

Player::Player() {

    constexpr std::array<const char*, Player::ANIMATION_COUNT> animationPaths = {{
        "src/resources/Animations/eyeball-Idle.png",
        "src/resources/Animations/eyeballMoveLeft.png",
        "src/resources/Animations/eyeballMoveRight.png",
        "src/resources/Animations/eyeballMoveUp.png",
        "src/resources/Animations/eyeball-attackleft.png",
        "src/resources/Animations/eyeball-attackRight.png",
        "src/resources/Animations/eyeball-attackDown.png"
    }};

    bulletTexture = LoadTexture("src/resources/bullet.png");

    for (std::size_t i = 0; i < ANIMATION_COUNT; i++) {
        m_textures[i] = LoadTexture(animationPaths[i]);
        
        // AnimationClip fields: texture pointer, frame count, fps, loop, random.
        m_animations[i] = AnimationClip{&m_textures[i], 8, 12.0f, true, false};
    }
    
}

Player::~Player() {
    for (Texture2D& texture : m_textures) {
        if (texture.id != 0) UnloadTexture(texture);
    }
    if (bulletTexture.id != 0) UnloadTexture(bulletTexture);
}

void Player::update(std::array<Platform, Globals::platformCount>& platforms) {
    
    if(debugMovement) {
        updateMovementDebug();
    }   
    else{
        updateMovement();
        checkCollisions(platforms);
    }
    
    getShootingInputKeyboard();
    if (IsGamepadAvailable(0)) getShootingInputController();

    shootBullets();
    updateAnimationState(); 
    m_animator.update(GetFrameTime());
}

void Player::updateAnimationState() {
    State next = State::IDLE;

    if (shootingUp)                         next = State::UP;
    else if (shootingDown)                  next = State::ATTACK_DOWN;
    else if (shootingLeft)                  next = State::ATTACK_LEFT;
    else if (shootingRight)                 next = State::ATTACK_RIGHT;
    else if (movingLeft || isGlidingLeft)   next = State::LEFT;
    else if (movingRight || isGlidingRight) next = State::RIGHT;
    else if (!isOnGround)                   next = State::UP;

    setAnimationState(next);
}

void Player::setAnimationState(State newState) {
    if (newState == m_currentState) return;

    m_currentState = newState;
    m_animator.play(m_animations[static_cast<std::size_t>(newState)]);
}

void Player::updateMovementDebug(){
    getMovementInputKeyboard();
    if(movingLeft) position.x -= 5.0f;
    if(movingRight) position.x += 5.0f;
    if(movingUp) position.y -= 5.0f;
    if(movingDown) position.y += 5.0f;
}

void Player::updateMovement(){

    float deltaTime = GetFrameTime();
    deltaTime = std::min(deltaTime, 0.05f);
    const float frameScale = deltaTime * Globals::referenceFPS;
    Vector2 appliedForces = {0.0f, gravity};

    // ================== //
    // INPUT              //
    // ================== //
    
    getMovementInputKeyboard();
    if(IsGamepadAvailable(0)) getMovementInputController();
    
    // ================== //
    // L+R KEY HOLD TIMES //
    // ================== //

    if (movingRight) rightHoldTime += deltaTime;
    else rightHoldTime = 0.0f;

    if (movingLeft) leftHoldTime += deltaTime;
    else leftHoldTime = 0.0f;

    // ================== //
    // JUMP INPUT         //
    // ================== // 

    if (jumpPressed)
    {
        if (isOnGround || isGlidingLeft || isGlidingRight)
        {
            canExtendJump = true;
            jumpHoldTime = 0.0f;
            jumpRequested = true;
        }
    }

    if (canExtendJump && jumpButtonHeld) jumpHoldTime += deltaTime;
    if (jumpReleased) canExtendJump = false;
    
    // ================== //
    // INITIAL JUMP       //
    // ================== //

    if (jumpRequested)
    {
        // Ground jump
        if (!isInAir && !isGlidingLeft && !isGlidingRight) velocity.y = -6.0f;
          
        // Double Jump
        //if (isInAir && !isGlidingLeft && !isGlidingRight && ) velocity.y = -6.0f;

        // Left wall jump
        else if (isGlidingLeft  && leftWallJumped)
        {
            velocity.y = -16.0f;
            velocity.x += 12.0f;

            leftWallJumped = false;
            rightWallJumped = true;
            isWallJumping = true;
            
            airFriction = 0.95f;
        }

        // Right wall jump
        else if (isGlidingRight && rightWallJumped)
        {
            velocity.y = -16.0f;
            velocity.x -= 12.0f;

            rightWallJumped = false;
            leftWallJumped = true;
            isWallJumping = true;
            
            airFriction = 0.95f;
        }

        isInAir = true;
        isOnGround = false;
        jumpRequested = false;

    }

    // ================== //
    // DNYMC JUMP HEIGHT  //
    // ================== //

    if (canExtendJump && (jumpHoldTime <maxJumpHoldTime) && isInAir && (velocity.y < 0.0f) && !isWallJumping)
    {
        appliedForces.y -= jumpHoldForce;
    }

    // ================== //
    // GROUND CONTROL     //
    // ================== //

    if (movingRight && (rightHoldTime > movementPressDelay) && !isGlidingRight && isOnGround)
    {
        appliedForces.x += groundAcceleration;
        isGlidingLeft = false;
    }

    if (movingLeft && (leftHoldTime > movementPressDelay) && !isGlidingLeft && isOnGround)
    {
        appliedForces.x -= groundAcceleration;
        isGlidingRight = false;
    }

    // ================== //
    // AIR CONTROL        //
    // ================== //

    if (movingRight && (rightHoldTime > movementPressDelay) && !isGlidingRight && !isOnGround)
    {
        if(isWallJumping) appliedForces.x += airAcceleration - airFriction;
        appliedForces.x += airAcceleration;
        isGlidingLeft = false;
    }

    if (movingLeft && (leftHoldTime > movementPressDelay) && !isGlidingLeft && !isOnGround)
    {   
        if(isWallJumping) appliedForces.x -= airAcceleration - airFriction;
        appliedForces.x -= airAcceleration;
        isGlidingRight = false;
    }

    // ================== //
    // APPLY FORCES       //
    // ================== //

    // applied forces twice; similar to meatboy physics behavior.
    velocity.x += 2.0f * appliedForces.x * frameScale;
    velocity.y += 2.0f * appliedForces.y * frameScale;

    // ================== //
    // FRICTION           //
    // ================== //

    float horizontalFriction = isOnGround ? groundFriction : airFriction;
    float verticalFriction = 1.0f;

    if (isGlidingLeft || isGlidingRight)
    {
        if (velocity.y <= 0.0f)
        {
            verticalFriction = wallRiseFriction;
        }
        else
        {
            verticalFriction = wallFallFriction;
        }
    }

    // pow(friction, frameScale) instead preserves the friction rate over real time.
    const float scaledHorizontalFriction = std::pow(horizontalFriction, frameScale);
    const float scaledVerticalFriction = std::pow(verticalFriction, frameScale);

    velocity.x *= scaledHorizontalFriction;
    velocity.y *= scaledVerticalFriction;

    // ================== //
    // MOVE               //
    // ================== //

    if (appliedForces.x == 0.0f && std::abs(velocity.x) < 1.0f) velocity.x = 0.0f;

    position.x += velocity.x * frameScale;

    previousY = position.y;

    position.y += velocity.y * frameScale;
}

void Player::checkCollisions(std::array<Platform, Globals::platformCount>& platforms){
    // ======================//
    // RESET COLLISION STATE //
    // ======================//

    isOnGround = false;
    isGlidingLeft = false;
    isGlidingRight = false;

    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;

    // ================== //
    // PLATFORM COLLISION //
    // ================== //

    for (auto& platform : platforms)
    {

        if(platform.falsePlatform) continue;
        if(movingDown) continue;

        float platformTop = platform.rect.y;
        float playerBottom = position.y + halfHeight;

        bool horizontalOverlap =
            position.x + halfWidth > platform.rect.x &&
            position.x - halfWidth < platform.rect.x + platform.rect.width;

        bool nearPlatform = std::abs(playerBottom - platformTop) < 5.0f;

        if (horizontalOverlap && nearPlatform && velocity.y >= 0.0f)
        {   
            position.y = platformTop - halfHeight+2.0f;
            velocity.y = 0.0f;
            isOnGround = true;
            isInAir = false;
            leftWallJumped = true;
            rightWallJumped = true;
            platform.onPlatform = true;
            isWallJumping = false;
            airFriction = 0.85f;
            break;
        }
        if(!isOnGround) platform.onPlatform = false;
    }

    // ================== //
    // FLOOR COLLISION    //
    // ================== //

    if (position.y + halfHeight >= Globals::floorY)
    {
        position.y = Globals::floorY - halfHeight;
        velocity.y = 0.0f;

        isInAir = false;
        isOnGround = true;

        leftWallJumped = true;
        rightWallJumped = true;
        isWallJumping = false;

        canExtendJump = false;
        jumpHoldTime = 0.0f;

        airFriction = 0.85;
    }

    // ================== //
    // LEFT WALL          //
    // ================== //

    if (position.x <= 176.0f)
    {
        position.x = 176.0f;
        velocity.x = 0.0f;

        if (!isOnGround) isGlidingLeft = true;
    }

    // ================== //
    // RIGHT WALL         //
    // ================== //

    if (position.x >= 784.0f)
    {
        position.x = 784.0f;
        velocity.x = 0.0f;

        if (!isOnGround) isGlidingRight = true;
    }

    // ================== //
    // CEILING            //
    // ================== //

    if (position.y - halfHeight < Globals::ceilingHeight)
    {
        position.y = Globals::ceilingHeight;

        if (velocity.y < 0.0f)
            velocity.y = 0.0f;
    }
}

void Player::shootBullets(){

    float dt = GetFrameTime();
    shootTimer -= dt;

    Vector2 center{position.x, position.y};

    if (shootTimer <= 0.0f) {
        Vector2 direction{0.0f, 0.0f};

        if (shootingUp){
            bulletRotation = 90.0f;
            direction = {0.0f, -1.0f};
        }
        else if (shootingDown){
            bulletRotation = 270.0f;
            direction = {0.0f, 1.0f};
        }
        else if (shootingLeft){
            bulletRotation = 180.0f;
            direction = {-1.0f, 0.0f};
        }
        else if (shootingRight){
            bulletRotation = 0.0f;
            direction = {1.0f, 0.0f};
        }

        if (Vector2Length(direction) > 0.0f) {
            direction.y += (velocity.y / 200.0f) * angleStrength;
            direction = Vector2Normalize(direction);
            bullets.push_back({center, direction, bulletRotation});
            bulletRects.push_back({center.x, center.y, 16.0f, 16.0f});
            shootTimer = 1.0f / fireRate;
        }
    }

    for (size_t i{}; i<bullets.size(); i++) {
        bullets[i].position.x += bullets[i].direction.x * bullets[i].bulletSpeed * dt;
        bullets[i].position.y += bullets[i].direction.y * bullets[i].bulletSpeed * dt;
        bulletRects[i].x = bullets[i].position.x;
        bulletRects[i].y = bullets[i].position.y;
    }

    // Remove bullets at window boundaries
    for (int i = static_cast<int>(bullets.size()) - 1; i >= 0; --i) {
        Vector2 pos = bullets[i].position;
        
        if (pos.x - bullets[i].bulletRadius <= 160 ||
            pos.x + bullets[i].bulletRadius >= 800 ||
            pos.y - bullets[i].bulletRadius <= -5400 ||
            pos.y + bullets[i].bulletRadius >= 500){
            bullets.erase(bullets.begin() + i);
            bulletRects.erase(bulletRects.begin() + i);
            continue;
        }
    }
}

void Player::getMovementInputKeyboard(){
    movingLeft = IsKeyDown(KEY_A);
    movingRight = IsKeyDown(KEY_D);
    movingDown = IsKeyDown(KEY_S);
    movingUp = IsKeyDown(KEY_W);
    jumpPressed = IsKeyPressed(KEY_SPACE);
    jumpButtonHeld = IsKeyDown(KEY_SPACE);
    jumpReleased =  IsKeyReleased(KEY_SPACE);
}

void Player::getMovementInputController(){
    if(!movingLeft) movingLeft = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    if(!movingRight) movingRight = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    if(!movingDown) movingDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    if(!jumpPressed) jumpPressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    if(!jumpButtonHeld) jumpButtonHeld = IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    if(!jumpReleased) jumpReleased =  IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);

    //left stick movement
    float axisX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    if (std::fabs(axisX) > 0.1f) {
        if(axisX > 0.3f) movingRight = true;
        else if(axisX < -0.3f) movingLeft = true;       
    }
}

void Player::getShootingInputKeyboard() {
    shootingUp = false;
    shootingDown = false;
    shootingLeft = false;
    shootingRight = false;

    if (IsKeyDown(KEY_UP))         shootingUp = true;
    else if (IsKeyDown(KEY_DOWN))  shootingDown = true;
    else if (IsKeyDown(KEY_LEFT))  shootingLeft = true;
    else if (IsKeyDown(KEY_RIGHT)) shootingRight = true;
}

void Player::getShootingInputController() {
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP))
        shootingUp = true;
    else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
        shootingDown = true;
    else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
        shootingLeft = true;
    else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
        shootingRight = true;
}

void Player::drawPlayer() {
    AnimationClip& clip = m_animations[static_cast<int>(m_currentState)];

    if (clip.texture && clip.texture->id != 0 && clip.frameCount > 0) {
        float frameWidth = static_cast<float>(clip.texture->width) / clip.frameCount;
        float frameHeight = static_cast<float>(clip.texture->height);
        m_animator.draw({position.x - frameWidth * 0.5f, position.y - frameHeight * 0.5f});
    }
}

void Player::drawBullets() {
    for (const auto& bullet : bullets)
        DrawCircleLines(bullet.position.x, bullet.position.y, bullet.bulletRadius, GREEN);

    for (auto& bullet : bullets) {
        bullet.rotation += 2.0;
        if(bullet.rotation > 360.0f) bullet.rotation = 0.0f;
        DrawTextureEx(bulletTexture, bullet.position, bullet.rotation, 1.0f, WHITE);
    }
}
