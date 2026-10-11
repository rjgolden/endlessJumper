#pragma once
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include "raylib.h"
#include "raymath.h"
#include "globals.h"

class Player {

    public: 

        Player();
        ~Player();
        void update(std::array<Globals::Platform, 20>& platforms);
        void updateMovement();
        void checkCollisions(std::array<Globals::Platform, 20>& platforms);
        void shootBullets();
        void drawPlayer();
        void drawBullets();
        void getShootingInputKeyboard();
        void getShootingInputController();
        void getMovementInputKeyboard();
        void getMovementInputController();

        float width{32.0f};
        float height{32.0f};
        float maxHorizontalVelocity{13.0f};
        float maxUpVelocity{-30.0f};
        float maxDownVelocity{60.0f};
        float jumpHeight{10.0f};
        float leftHoldTime{};
        float rightHoldTime{};
        float jumpHoldTime{};
        float previousY{};
        Vector2 position{480.0f, 440.0f};   
        Vector2 velocity{};

         // bullets
        std::vector<Globals::Bullet> bullets;
        std::vector<Rectangle> bulletRects;
        Texture2D bulletTexture{};
        float bulletRotation{};
        float bulletSpeed{600.0f};
        float bulletRadius{5.0f};
        float fireRate{10.0f}; // 10 per second
        float shootTimer{0.0f};
        float angleStrength{0.5f};
        bool shootingLeft{};
        bool shootingRight{};
        bool shootingUp{};
        bool shootingDown{};

        bool jumped{};
        bool jumpHeld{};
        bool isJumping{};
        bool isOnGround{};
        bool jumpPressed{};
        bool jumpDown{};
        bool jumpReleased{};
        bool movingLeft{};
        bool movingRight{};
        bool isGlidingLeft{};
        bool isGlidingRight{};
        bool leftWallJumped{true};
        bool rightWallJumped{true};

};