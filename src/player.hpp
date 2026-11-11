#pragma once

#include <array>
#include <cstddef>
#include <vector>
#include "raylib.h"
#include "globals.hpp"
#include "animator.hpp"

class Player {

    public:

        Player();
        ~Player();

        Player(const Player&) = delete;
        Player& operator=(const Player&) = delete;
        Player(Player&&) = delete;
        Player& operator=(Player&&) = delete;

        void update(std::array<Platform, 90>& platforms);
        void updateMovement();
        void checkCollisions(std::array<Platform, 90>& platforms);
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
        
        const float gravity{0.5f};
        const float groundAcceleration{0.9f};
        const float groundFriction{0.75f};
        const float airAcceleration{0.6f};
        const float airFriction{0.85f};
        const float wallRiseFriction{1.00f};
        const float wallFallFriction{0.70f};
        const float movementPressDelay{5.0f / Globals::referenceFPS}; // 5 frames/sec
        const float maxJumpHoldTime{10.0f / Globals::referenceFPS};
        const float jumpHoldForce{0.9f};
        
        Vector2 position{480.0f, 440.0f}; // center of player (not sprite top-left)
        Vector2 velocity{};

        std::vector<Bullet> bullets;
        std::vector<Rectangle> bulletRects;
        Texture2D bulletTexture{};
        float bulletRotation{};
        float bulletSpeed{600.0f};
        float bulletRadius{10.0f};
        float fireRate{10.0f};
        float shootTimer{};
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

        enum class State {
            IDLE,
            LEFT,
            RIGHT,
            UP,
            ATTACK_LEFT,
            ATTACK_RIGHT,
            ATTACK_DOWN,
            COUNT
        };

        static constexpr std::size_t ANIMATION_COUNT =
            static_cast<std::size_t>(State::COUNT);

        void updateAnimationState();
        void setAnimationState(State newState);

        std::array<Texture2D, ANIMATION_COUNT> m_textures{};
        std::array<AnimationClip, ANIMATION_COUNT> m_animations{};
        Animator m_animator;
        State m_currentState{State::IDLE};
};
