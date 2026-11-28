#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include "raylib.h"
#include "globals.hpp"
#include "animator.hpp"

class Collectible;
class Platform;
class Floor;

class Player {

    public:

        Player();
        ~Player();

        void update(Floor& floor);
        void updateMovement();
        void updateMovementDebug();
        void checkCollisions(Floor& floor);
        void shootBullets();
        void checkBulletCollisions(Floor& floor);

        void drawPlayer();
        void drawBullets();

        void getShootingInputKeyboard();
        void getShootingInputController();
        void getMovementInputKeyboard();
        void getMovementInputController();
        
        Vector2 position{480.0f, 440.0f}; // center of player (not sprite top-left)
        Vector2 velocity{};

        bool debugMovement{false};

    private:

        float width{32.0f};
        float height{32.0f};

        float maxHorizontalVelocity{7.5f};
        float maxUpVelocity{-15.0f};
        float maxDownVelocity{25.0f};

        float leftHoldTime{};
        float rightHoldTime{};
        float jumpHoldTime{};
        float previousY{};
        
        float airFriction{0.80f};
        float airAcceleration{0.5f};
        const float gravity{0.5f};
        const float groundAcceleration{0.7f};
        const float groundFriction{0.75f};
        const float wallRiseFriction{1.00f};
        const float wallFallFriction{0.70f};

        const float movementPressDelay{5.0f / Globals::referenceFPS}; // 5 frames/sec
        const float maxJumpHoldTime{14.0f / Globals::referenceFPS};
        const float jumpHoldForce{0.7f};
        
        std::vector<Globals::Projectile> bullets;
        std::vector<Rectangle> bulletRects;
        Texture2D bulletTexture{};
        float bulletRotation{};
        float fireRate{4.0f};
        float shootTimer{};
        float angleStrength{0.5f};
        
        bool shootingLeft{};
        bool shootingRight{};
        bool shootingUp{};
        bool shootingDown{};
        bool shootingNE{};
        bool shootingSE{};
        bool shootingSW{};
        bool shootingNW{};

        bool jumpRequested{};
        bool canExtendJump{};
        bool isInAir{};
        bool isOnGround{};
        bool jumpPressed{};
        bool jumpButtonHeld{};
        bool jumpReleased{};
        bool isWallJumping{};
        
        bool lastDirection{}; // false = left, true = right
        bool movingLeft{};
        bool movingRight{};
        bool movingDown{};
        bool movingUp{};
        bool isSlidingLeft{};
        bool isSlidingRight{};
        bool leftWallJumped{true};
        bool rightWallJumped{true};

        enum class State {IDLE_LEFT, IDLE_RIGHT, MOVE_LEFT, MOVE_RIGHT, 
                         ATTACK_UP, ATTACK_LEFT, ATTACK_RIGHT, ATTACK_DOWN,
                         SLIDE_LEFT, SLIDE_RIGHT,
                         JUMP_LEFT, JUMP_RIGHT,
                         FALL_LEFT, FALL_RIGHT,
                         COUNT};

        static constexpr std::size_t ANIMATION_COUNT = static_cast<std::size_t>(State::COUNT);

        void updateAnimationState();
        void setAnimationState(State newState);

        std::array<Texture2D, ANIMATION_COUNT> m_textures{};
        std::array<AnimationClip, ANIMATION_COUNT> m_animations{};
        Animator animator;
        State currentState{State::IDLE_RIGHT};
};
