#include "enemies.hpp"
#include <cmath>

Enemies::Enemies(){
    resetEnemies();
}

void update(){
    // TODO
}

void Enemies::resetEnemies() {
    for(int i{}; i<Globals::enemyCount; i++){
        float x{static_cast<float>(GetRandomValue(170, 600))};
        float y{static_cast<float>(GetRandomValue(0, 540))};
        enemyArr[i].position.x = x;
        enemyArr[i].position.y = y;
        enemyArr[i].currentState = Enemy::State::CHASE;
        enemyArr[i].currentHealth = enemyArr[i].startingHealth;
        enemyArr[i].healthBarRect = {x-16.0f, y-32.0f, 32.0f, 10.0f};
    }
}

void Enemies::chasePlayer(Vector2 playerPosition){
    float deltaTime = GetFrameTime();
     for (Enemy& enemy : enemyArr) {
        if(enemy.currentState == Enemy::State::DEAD) continue;
        float dx = playerPosition.x - enemy.position.x;
        float dy = playerPosition.y - enemy.position.y;
        float length = std::sqrt(dx*dx + dy*dy);
        if (length > 0) {
            dx /= length;
            dy /= length;
        }

        // Chase + knockback
        enemy.position.x += (dx * enemy.velocity.x + enemy.knockbackVelocity.x) * deltaTime;
        enemy.position.y += (dy * enemy.velocity.y + enemy.knockbackVelocity.y) * deltaTime;

        // Knockback fades out (higher number = shorter shove)
        float decay = std::exp(-8.0f * deltaTime);
        enemy.knockbackVelocity.x *= decay;
        enemy.knockbackVelocity.y *= decay;
    }
}

void Enemies::drawEnemies(){
    for (Enemy& enemy : enemyArr) {
         if(enemy.currentState == Enemy::State::DEAD) continue;
         DrawCircleLines(enemy.position.x, enemy.position.y, enemy.radius, RED);
         drawHealthBars(enemy);
    }
}

void Enemies::drawHealthBars(Enemy& enemy){
        // draw background
        DrawRectangle(enemy.position.x-16.0f, enemy.position.y-32.0f, enemy.healthBarRect.width, enemy.healthBarRect.height, GRAY);
        
        // calculate health bar width based on current health
        float healthWidth = (enemy.currentHealth / enemy.startingHealth) * enemy.healthBarRect.width;
        
        // draw health bar
        DrawRectangle(enemy.position.x-16.0f, enemy.position.y-32.0f, healthWidth, enemy.healthBarRect.height, RED);
        DrawRectangleLines(enemy.position.x-16.0f, enemy.position.y-32.0f, enemy.healthBarRect.width, enemy.healthBarRect.height, BLACK);
}