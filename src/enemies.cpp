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
        enemyArr[i].currentState = Enemy::State::IDLE;
        enemyArr[i].currentHealth = 70.0f;
        //enemyArr[i].currentHealth = enemyArr[i].startingHealth;
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

        bool check = CheckCollisionCircleRec(enemy.position, enemy.attackRadius, {playerPosition.x, playerPosition.y, 32.0f, 32.0f}); 
        if(check) enemy.currentState = Enemy::State::CHASE;
        else enemy.currentState = Enemy::State::IDLE;

        if(enemy.currentState == Enemy::State::CHASE){
            enemy.position.x += (dx * enemy.velocity.x) * deltaTime;
            enemy.position.y += (dy * enemy.velocity.y) * deltaTime;
        }

        // Apply knockback
        enemy.position.x += (enemy.knockbackVelocity.x) * deltaTime;
        enemy.position.y += (enemy.knockbackVelocity.y) * deltaTime;

        // Knockback decay
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
         //DrawCircleLines(enemy.position.x, enemy.position.y, enemy.attackRadius, BLUE);
    }
}

void Enemies::drawHealthBars(Enemy& enemy){
        // Draw background
        DrawRectangle(enemy.position.x-16.0f, enemy.position.y-32.0f, enemy.healthBarRect.width, enemy.healthBarRect.height, GRAY);
        
        // Calculate health bar width based on current health
        float healthWidth = (enemy.currentHealth / enemy.maxHealth) * enemy.healthBarRect.width;
        
        // Draw health bar
        DrawRectangle(enemy.position.x-16.0f, enemy.position.y-32.0f, healthWidth, enemy.healthBarRect.height, RED);
        DrawRectangleLines(enemy.position.x-16.0f, enemy.position.y-32.0f, enemy.healthBarRect.width, enemy.healthBarRect.height, BLACK);
}