#pragma once
#include <array>
#include "raylib.h"
#include "globals.hpp"

struct Enemy {
    Vector2 position{};
    Vector2 velocity{50.0f, 50.0f};
    Vector2 knockbackVelocity{};
    Rectangle healthBarRect{};
    enum class State {IDLE, CHASE, DEAD};
    State currentState{State::CHASE}; 
    float radius{16.0f};
    float startingHealth{70.0f};
    float currentHealth{};
    int hits = 7;
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
