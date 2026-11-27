#include "player.hpp"
#include <algorithm>
#include <cmath>
#include "raymath.h"
#include "collectibles.hpp"
#include "platforms.hpp"
#include "floor.hpp"

Player::Player() {

    constexpr std::array<const char*, Player::ANIMATION_COUNT> animationPaths = {{
        "src/resources/Animations/playerIdle.png",
        "src/resources/Animations/playerRunLeft.png",
        "src/resources/Animations/playerRunRight.png",
        "src/resources/Animations/playerAttackUp.png",
        "src/resources/Animations/playerAttackLeft.png",
        "src/resources/Animations/playerAttackRight.png",
        "src/resources/Animations/playerAttackDown.png",
        "src/resources/Animations/playerWallSlideLeft.png",
        "src/resources/Animations/playerWallSlideRight.png",
        "src/resources/Animations/playerJumpLeft.png",
        "src/resources/Animations/playerJumpRight.png",
        "src/resources/Animations/playerFallLeft.png",
        "src/resources/Animations/playerFallRight.png"
    }};

    bulletTexture = LoadTexture("src/resources/bullet.png");
   
    for (std::size_t i = 0; i < ANIMATION_COUNT; i++) {
        m_textures[i] = LoadTexture(animationPaths[i]);
        int frameCount = m_textures[i].width / 32.0f;
        m_animations[i] = AnimationClip{&m_textures[i], frameCount, 12.0f, true, false};
    }
    
}

Player::~Player() {
    for (Texture2D& texture : m_textures) {
        if (texture.id != 0) UnloadTexture(texture);
    }
    if (bulletTexture.id != 0) UnloadTexture(bulletTexture);
}

void Player::update(Floor& floor) {
    
    if(debugMovement) {
        updateMovementDebug();
    }   
    else{
        updateMovement();
        checkCollisions(floor);
    }
    
    getShootingInputKeyboard();
    if (IsGamepadAvailable(0)) getShootingInputController();

    shootBullets();
    checkBulletCollisions(floor);
    updateAnimationState(); 
    animator.update(GetFrameTime());
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
        if (isOnGround || isSlidingLeft || isSlidingRight)
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
        if (!isInAir && !isSlidingLeft && !isSlidingRight) velocity.y = -6.0f;
          
        // Double Jump
        //if (isInAir && !isGlidingLeft && !isGlidingRight && ) velocity.y = -6.0f;

        // Left wall jump
        else if (isSlidingLeft  && leftWallJumped)
        {
            velocity.y = -16.0f;
            velocity.x += 12.0f;

            leftWallJumped = false;
            rightWallJumped = true;
            isWallJumping = true;
            
            airFriction = 0.95f;
        }

        // Right wall jump
        else if (isSlidingRight && rightWallJumped)
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

    if (movingRight && (rightHoldTime > movementPressDelay) && !isSlidingRight && isOnGround)
    {
        appliedForces.x += groundAcceleration;
        isSlidingLeft = false;
    }

    if (movingLeft && (leftHoldTime > movementPressDelay) && !isSlidingLeft && isOnGround)
    {
        appliedForces.x -= groundAcceleration;
        isSlidingRight = false;
    }

    // ================== //
    // AIR CONTROL        //
    // ================== //

    if (movingRight && (rightHoldTime > movementPressDelay) && !isSlidingRight && !isOnGround)
    {
        if(isWallJumping) appliedForces.x += airAcceleration - airFriction;
        appliedForces.x += airAcceleration;
        isSlidingLeft = false;
    }

    if (movingLeft && (leftHoldTime > movementPressDelay) && !isSlidingLeft && !isOnGround)
    {   
        if(isWallJumping) appliedForces.x -= airAcceleration - airFriction;
        appliedForces.x -= airAcceleration;
        isSlidingRight = false;
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

    if (isSlidingLeft || isSlidingRight)
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

void Player::checkCollisions(Floor& floor) {
    // ======================//
    // RESET COLLISION STATE //
    // ======================//

    isOnGround = false;
    isSlidingLeft = false;
    isSlidingRight = false;

    const float halfWidth = width / 2.0f;
    const float halfHeight = height / 2.0f;

    // ================== //
    // PLATFORM COLLISION //
    // ================== //

    for (Platform& platform : floor.platforms.platformArr)
    {

        if(platform.falsePlatform) continue;
        if(movingDown) continue;

        float platformTop = platform.rect.y;
        float playerBottom = position.y + halfHeight;

        bool horizontalOverlap = position.x + halfWidth > platform.rect.x && position.x - halfWidth < platform.rect.x + platform.rect.width;
        bool nearPlatform = std::abs(playerBottom - platformTop) < 5.0f;

        if (horizontalOverlap && nearPlatform && velocity.y >= 0.0f)
        {   
            position.y = platformTop - halfHeight+2.0f;
            velocity.y = 0.0f;

            isInAir = false;
            isOnGround = true;
            platform.onPlatform = true;

            leftWallJumped = true;
            rightWallJumped = true;
            isWallJumping = false;
        
            airFriction = 0.85f;
        }
        if(!isOnGround) {
            platform.onPlatform = false;
        }
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
    
    if (position.x - halfWidth <= Globals::leftEdge)
    {
        position.x = 176.0f;
        velocity.x = 0.0f;

        if (!isOnGround) isSlidingLeft = true;
    }

    // ================== //
    // RIGHT WALL         //
    // ================== //

    if (position.x + halfWidth >= Globals::rightEdge)
    {
        position.x = 784.0f;
        velocity.x = 0.0f;

        if (!isOnGround) isSlidingRight = true;
    }

    // ================== //
    // CEILING            //
    // ================== //

    if (position.y - halfHeight <= Globals::ceilingHeight)
    {
        position.y = Globals::ceilingHeight;

        if (velocity.y < 0.0f) velocity.y = 0.0f;
    }

    // ================== //
    // COLLECTIBLES       //
    // ================== //

    for(Collectible& collectible : floor.collectibles.collectibleArr){
        if(collectible.collected) continue;
        bool check = CheckCollisionCircleRec(collectible.position, collectible.radius, {position.x, position.y, width, height}); 
        if(check){
            //TraceLog(LOG_INFO, "Collected!");
            floor.score += 10;
            collectible.collected = true;
        }
    }
    
    for (Particle& particle : floor.particles.particlesArr) {
        if (particle.life <= 0.0f) continue;
        bool check = CheckCollisionCircleRec(particle.pos, 16.0f, {position.x, position.y, width, height});
        if(check) {
            //TraceLog(LOG_INFO, "Gold Collected!");
            particle.life = 0.0f;
            floor.score += 2 * particle.scale;
        }
    }
}

void Player::shootBullets(){

    float deltaTime = GetFrameTime();
    shootTimer -= deltaTime;

    Vector2 center{position.x, position.y};

    if (shootTimer <= 0.0f) {
        Vector2 direction{0.0f, 0.0f};

        if (shootingUp){
            direction = {0.0f, -1.0f};
        }
        else if (shootingDown){
            direction = {0.0f, 1.0f};
        }
        else if (shootingLeft){
            direction = {-1.0f, 0.0f};
        }
        else if (shootingRight){
            direction = {1.0f, 0.0f};
        }
        else if (shootingNE){
            direction = {1.0f, -1.0f};
        }
        else if (shootingSE){
            direction = {1.0f, 1.0f};
        }
        else if (shootingSW){
            direction = {-1.0f, 1.0f};
        }
        else if (shootingNW){
            direction = {-1.0f, -1.0f};
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
        bullets[i].position.x += bullets[i].direction.x * bullets[i].shotSpeed * deltaTime;
        bullets[i].position.y += bullets[i].direction.y * bullets[i].shotSpeed * deltaTime;
        bulletRects[i].x = bullets[i].position.x;
        bulletRects[i].y = bullets[i].position.y;
    }
}

void Player::checkBulletCollisions(Floor& floor){

    // Remove bullets at window boundaries
    for (int i = static_cast<int>(bullets.size()) - 1; i >= 0; --i) {
        if (bullets[i].position.x - bullets[i].radius <= 160.0f ||
            bullets[i].position.x + bullets[i].radius >= 800.0f ||
            bullets[i].position.y - bullets[i].radius <= -5400.0f ||
            bullets[i].position.y + bullets[i].radius >= 550.0f){
            bullets.erase(bullets.begin() + i);
            bulletRects.erase(bulletRects.begin() + i);
            continue;
        }
    }

    for(Enemy& enemy : floor.enemies.enemyArr){
        if(enemy.currentState == Enemy::State::DEAD) continue;
        for (int i = static_cast<int>(bullets.size()) - 1; i >= 0; --i) {    
            if(CheckCollisionCircles(enemy.position, enemy.radius, {bullets[i].position.x, bullets[i].position.y + 8.0f}, bullets[i].radius)){

                // Direction away from bullet
                Vector2 direction = {enemy.position.x - bullets[i].position.x, enemy.position.y - bullets[i].position.y};
                
                // Normalize direction
                float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
                
                if (length > 0.001f) { direction.x /= length; direction.y /= length; } 
                else direction = {enemy.position.x - bullets[i].position.x, enemy.position.y - bullets[i].position.y}; 
        
                enemy.knockbackVelocity.x = direction.x * bullets[i].knockbackStrength;
                enemy.knockbackVelocity.y = direction.y * bullets[i].knockbackStrength;
                
                // Remove bullet...   
                enemy.currentHealth -= bullets[i].damage;
                bullets.erase(bullets.begin() + i);
                bulletRects.erase(bulletRects.begin() + i);
            
                if(enemy.currentHealth <= 0.0f) {
                    //floor.particles.explode(enemy.position, RED, rand() % floor.particles.particleTextures.size()); //blood
                    floor.particles.explodeNuggets(enemy.position, WHITE, 0);
                    enemy.currentState = Enemy::State::DEAD;
                }
                continue;  
            }
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
    shootingNE = false;
    shootingSE = false; 
    shootingSW = false;
    shootingNW = false;
    
    if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT)) shootingNE = true; // never
    else if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT)) shootingSE = true; // eat
    else if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT)) shootingSW = true; // soggy
    else if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT)) shootingNW = true; // waffles
    else if (IsKeyDown(KEY_UP)) shootingUp = true;
    else if (IsKeyDown(KEY_DOWN)) shootingDown = true;
    else if (IsKeyDown(KEY_LEFT)) shootingLeft = true;
    else if (IsKeyDown(KEY_RIGHT)) shootingRight = true;
}

void Player::getShootingInputController() {
    if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) shootingUp = true;
    else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2)) shootingDown = true;
    else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) shootingLeft = true;
    else if (IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)) shootingRight = true;
}

void Player::updateAnimationState() {
    State next = State::IDLE;

    if (shootingUp)                         next = State::ATTACK_UP;
    else if (shootingDown)                  next = State::ATTACK_DOWN;
    else if (isSlidingLeft)                 next = State::SLIDE_LEFT;
    else if (isSlidingRight)                next = State::SLIDE_RIGHT;
    else if (shootingLeft && !isSlidingLeft)  next = State::ATTACK_LEFT;
    else if (shootingRight && !isSlidingRight) next = State::ATTACK_RIGHT;
    else if (movingLeft && isInAir && velocity.y >= 0.0f)  next = State::FALL_LEFT;
    else if (movingRight && isInAir && velocity.y >= 0.0f)  next = State::FALL_RIGHT;
    else if(isInAir && velocity.y >= 0.0f)  next = State::FALL_RIGHT;
    else if (movingLeft && isInAir)         next = State::JUMP_LEFT;
    else if (movingRight && isInAir)        next = State::JUMP_RIGHT;
    else if (movingLeft)                    next = State::MOVE_LEFT;
    else if (movingRight)                   next = State::MOVE_RIGHT;

    setAnimationState(next);
}

void Player::setAnimationState(State newState) {
    if (newState == currentState) return;

    currentState = newState;
    animator.play(m_animations[static_cast<std::size_t>(newState)]);
}

void Player::drawPlayer() {
    AnimationClip& clip = m_animations[static_cast<int>(currentState)];

    if (clip.texture && clip.texture->id != 0 && clip.frameCount > 0) {
        float frameWidth = static_cast<float>(clip.texture->width) / clip.frameCount;
        float frameHeight = static_cast<float>(clip.texture->height);
        animator.draw({position.x - frameWidth * 0.5f, position.y - frameHeight * 0.5f});
    }
}

void Player::drawBullets() {
    /*
    for (const auto& bullet : bullets)
        DrawCircleLines(bullet.position.x, bullet.position.y + 8.0f, bullet.radius, BLUE);
    */
    for (auto& bullet : bullets) {
        //bullet.rotation += 2.0f;
        //if(bullet.rotation > 360.0f) bullet.rotation = 0.0f;
        DrawTextureEx(bulletTexture, {bullet.position.x - 8.0f, bullet.position.y}, bullet.rotation, 1.0f, WHITE);
    }
}
