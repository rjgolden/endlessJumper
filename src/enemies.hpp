#pragma once
#include <array>
#include "raylib.h"
#include "globals.hpp"

struct Enemy {
    Vector2 position{};
    Vector2 velocity{75.0f, 75.0f};
    Vector2 knockbackVelocity{};
    Rectangle healthBarRect{};
    enum class State {IDLE, CHASE, DEAD};
    State currentState{State::CHASE}; 
    float radius{16.0f};
    float attackRadius{270.0f};
    float maxHealth{70.0f};
    float currentHealth{};
};

class Enemies {

    public: 
    
        Enemies();
        void update();
        void resetEnemies();
        void chasePlayer(Vector2 playerPosition);
        void drawEnemies();
        void drawHealthBars(Enemy& enemy);
        
        Rectangle generateEnemies(const Rectangle& previous);
        std::array<Enemy, Globals::enemyCount> enemyArr{};

};
