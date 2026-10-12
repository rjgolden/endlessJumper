#include "player.hpp"

Player::Player(){
    bulletTexture = LoadTexture("src/bullet.png");
}

Player::~Player(){
    UnloadTexture(bulletTexture);
}

void Player::update(std::array<Globals::Platform, 20>& platforms){
    updateMovement();
    checkCollisions(platforms);
    shootBullets();
}

void Player::updateMovement(){

    float deltaTime = GetFrameTime();
    // Avoid a giant physics step if the window hangs, gets dragged, debugger pauses, etc.
    deltaTime = std::min(deltaTime, 0.05f);
    // At 60 FPS: deltaTime = 1/60, frameScale = 1
    const float frameScale = deltaTime * Globals::referenceFPS;
    Vector2 appliedForces = {0.0f, Globals::gravity};

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
            jumpHeld = true;
            jumpHoldTime = 0.0f;
            jumped = true;
        }
    }

    if (jumpHeld && jumpDown) jumpHoldTime += deltaTime;
    if (jumpReleased)jumpHeld = false;
    
    // ================== //
    // INITIAL JUMP       //
    // ================== //

    if (jumped)
    {
        // Ground jump
        if (!isJumping && !isGlidingLeft && !isGlidingRight)
            velocity.y = -6.0f;

        // Left wall jump
        else if (isGlidingLeft  && leftWallJumped)
        {
            velocity.y = -4.0f;
            leftWallJumped = false;
            rightWallJumped = true;
            maxHorizontalVelocity = 999.0f;

            if (movingRight) velocity.x += 12.0f;
            else velocity.x += 12.0f;
        }

        // Right wall jump
        else if (isGlidingRight && rightWallJumped)
        {
            velocity.y = -4.0f;
            rightWallJumped = false;
            leftWallJumped = true;
            maxHorizontalVelocity = 999.0f;

            if (movingLeft) velocity.x -= 12.0f;
            else velocity.x -= 12.0f;
        }

        isJumping = true;
        isOnGround = false;
        jumped = false;
    }

    // ================== //
    // DNYMC JUMP HEIGHT  //
    // ================== //

    if (jumpHeld && (jumpHoldTime < Globals::maxJumpHoldTime) && isJumping && velocity.y < 0.0f)
    {
        appliedForces.y -= Globals::jumpHoldForce;
    }

    // ================== //
    // GROUND CONTROL     //
    // ================== //

    if (movingRight && (rightHoldTime > Globals::movementPressDelay) && !isGlidingRight && isOnGround)
    {
        appliedForces.x += Globals::groundAcceleration;
        isGlidingLeft = false;
    }

    if (movingLeft && (leftHoldTime > Globals::movementPressDelay) &&!isGlidingLeft && isOnGround)
    {
        appliedForces.x -= Globals::groundAcceleration;
        isGlidingRight = false;
    }

    // ================== //
    // AIR CONTROL        //
    // ================== //

    if (movingRight && (rightHoldTime > Globals::movementPressDelay) && !isGlidingRight && !isOnGround)
    {
        appliedForces.x += Globals::airAcceleration;
        isGlidingLeft = false;
    }

    if (movingLeft && (leftHoldTime > Globals::movementPressDelay)  && !isGlidingLeft && !isOnGround)
    {
        appliedForces.x -= Globals::airAcceleration;
        isGlidingRight = false;
    }

    // ================== //
    // APPLY FORCES       //
    // ================== //

    velocity.x += appliedForces.x * frameScale;
    velocity.y += appliedForces.y * frameScale;
    velocity.x += appliedForces.x * frameScale;
    velocity.y += appliedForces.y * frameScale;

    // ================== //
    // FRICTION           //
    // ================== //

    float horizontalFriction = isOnGround ? Globals::groundFriction : Globals::airFriction;
    float verticalFriction = 1.0f;

    if (isGlidingLeft || isGlidingRight)
    {
        if (velocity.y <= 0.0f)
        {
            verticalFriction = Globals::wallRiseFriction;
        }
        else
        {
            verticalFriction = Globals::wallFallFriction;
        }
    }

    // pow(friction, frameScale) instead preserves the friction rate over real time.
    const float scaledHorizontalFriction = std::pow(horizontalFriction, frameScale);
    const float scaledVerticalFriction = std::pow(verticalFriction, frameScale);

    velocity.x *= scaledHorizontalFriction;
    velocity.y *= scaledVerticalFriction;

    // ================== //
    // STOP SMALL MVMNT   //
    // ================== //
    /*
    if (appliedForces.x == 0.0f && std::abs(player.velocity.x) < 1.0f)
    {
        player.velocity.x = 0.0f;
    }
    */
    // ================== //
    // SPEED CAPS         //
    // ================== //

    velocity.x = std::clamp(velocity.x, -maxHorizontalVelocity, maxHorizontalVelocity);
    velocity.y = std::clamp(velocity.y, maxUpVelocity, maxDownVelocity);

    // ================== //
    // MOVE               //
    // ================== //

    position.x += velocity.x * frameScale;

    previousY = position.y;

    position.y += velocity.y * frameScale;

    maxHorizontalVelocity = 13.0f;
}

void Player::checkCollisions(std::array<Globals::Platform, 20>& platforms){
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

        if(platform.blocking) continue;

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
            isJumping = false;
            leftWallJumped = true;
            rightWallJumped = true;
            platform.onPlatform = true;
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

        isJumping = false;
        isOnGround = true;

        leftWallJumped = true;
        rightWallJumped = true;

        jumpHeld = false;
        jumpHoldTime = 0.0f;
    }

    // ================== //
    // LEFT WALL          //
    // ================== //

    if (position.x <= 176.0f)
    {
        position.x = 176.0f;
        velocity.x = 0.0f;

        if (!isOnGround)
            isGlidingLeft = true;
    }

    // ================== //
    // RIGHT WALL         //
    // ================== //

    if (position.x >= 784.0f)
    {
        position.x = 784.0f;
        velocity.x = 0.0f;

        if (!isOnGround)
            isGlidingRight = true;
    }

    // ================== //
    // CEILING            //
    // ================== //

    if (position.y - halfHeight < 0.0f)
    {
        position.y = halfHeight;

        if (velocity.y < 0.0f)
            velocity.y = 0.0f;
    }
}

void Player::shootBullets(){

    float dt = GetFrameTime();
    
    Vector2 center{
        position.x,
        position.y
    };

    shootTimer -= dt;

    if (shootTimer <= 0.0f) {
        Vector2 direction{0.0f, 0.0f};

        getShootingInputKeyboard();
        if(IsGamepadAvailable(0)) getShootingInputController();

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
        bullets[i].position.x += bullets[i].direction.x * bulletSpeed * dt;
        bullets[i].position.y += bullets[i].direction.y * bulletSpeed * dt;
        bulletRects[i].x = bullets[i].position.x;
        bulletRects[i].y = bullets[i].position.y;
    }

    // Remove bullets at window boundaries
    for (int i = static_cast<int>(bullets.size()) - 1; i >= 0; --i) {
        Vector2 pos = bullets[i].position;
        
        /*
        if(CheckCollisionCircleRec(pos, bulletRadius, enemy)){
            bullets.erase(bullets.begin() + i);
            enemy.x = GetRandomValue(60, 600);
            enemy.y = GetRandomValue(20, 300);
            continue;
        }*/

        if (pos.x - bulletRadius <= 160 ||
            pos.x + bulletRadius >= 800 ||
            pos.y - bulletRadius <= 0 ||
            pos.y + bulletRadius >= 500){
            bullets.erase(bullets.begin() + i);
            bulletRects.erase(bulletRects.begin() + i);
            continue;
        }
    }
}

void Player::getMovementInputKeyboard(){
    movingLeft = IsKeyDown(KEY_A);
    movingRight = IsKeyDown(KEY_D);
    jumpPressed = IsKeyPressed(KEY_SPACE);
    jumpDown = IsKeyDown(KEY_SPACE);
    jumpReleased =  IsKeyReleased(KEY_SPACE);
}

void Player::getMovementInputController(){
    if(!movingLeft) movingLeft = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    if(!movingRight) movingRight = IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    if(!jumpPressed) jumpPressed = IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    if(!jumpDown) jumpDown = IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    if(!jumpReleased) jumpReleased =  IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);

    //left stick movement
    float axisX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    if (std::fabs(axisX) > 0.1f) {
        if(axisX > 0.3f) movingRight = true;
        else if(axisX < -0.3f) movingLeft = true;       
    }
}

void Player::getShootingInputKeyboard(){
    if (IsKeyDown(KEY_UP))
        shootingUp = true;
    else if (IsKeyDown(KEY_DOWN))
        shootingDown = true;
    else if (IsKeyDown(KEY_LEFT))
        shootingLeft = true;
    else if (IsKeyDown(KEY_RIGHT))
        shootingRight = true;
    else{
        shootingUp = false;
        shootingDown = false;
        shootingLeft = false;
        shootingRight = false;
    }
}

void Player::getShootingInputController(){
    if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)){
        shootingUp = true;     
    }
    else if(IsGamepadButtonDown(0,  GAMEPAD_BUTTON_RIGHT_TRIGGER_2)){
        shootingDown = true;     
    }
    else if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)){
        shootingLeft = true;     
    }
    else if(IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT)){
        shootingRight = true;     
    }
    else{
        if(!shootingUp) shootingUp = false;
        if(!shootingDown) shootingDown = false;
        if(!shootingLeft) shootingLeft = false;
        if(!shootingRight) shootingRight = false;
    }
}

void Player::drawPlayer(){
    DrawRectangle(
        static_cast<int>(position.x - 16.0f),
        static_cast<int>(position.y - 16.0f),
        32,32, RED
    );
}

void Player::drawBullets() {
    for (const auto& bullet : bullets)
        DrawCircleLines(bullet.position.x, bullet.position.y, bulletRadius, GREEN);

    for (const auto& bullet : bullets)
        DrawTextureEx(bulletTexture, bullet.position, bullet.rotation, 1.0f, WHITE);
}