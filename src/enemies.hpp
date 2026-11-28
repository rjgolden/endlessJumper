#pragma once
#include <array>
#include "raylib.h"
#include "globals.hpp"
#include "animator.hpp"

struct Enemy {
    Vector2 position{};
    Vector2 velocity{75.0f, 75.0f};
    Vector2 knockbackVelocity{};
    Rectangle healthBarRect{};
    enum class State {IDLE, CHASE, DEAD, COUNT};
    State currentState{State::CHASE}; 
    float radius{16.0f};
    float attackRadius{270.0f};
    float maxHealth{20.0f};
    float currentHealth{};
    Animator animator;
};

class Enemies {

    public: 
    
        Enemies();
        ~Enemies();
        void update(Vector2 &playerPosition);
        void resetEnemies();
        void chasePlayer(Vector2 &playerPosition);
        void drawEnemies();
        void drawHealthBars(Enemy& enemy);
        
        Rectangle generateEnemies(const Rectangle& previous);
        std::array<Enemy, Globals::enemyCount> enemyArr{};


        static constexpr std::size_t ANIMATION_COUNT = static_cast<std::size_t>(Enemy::State::COUNT);

        void updateAnimationState();
        void setAnimationState(Enemy::State newState, Enemy& enemy);

        std::array<Texture2D, ANIMATION_COUNT> m_textures{};
        std::array<AnimationClip, ANIMATION_COUNT> m_animations{};
};
