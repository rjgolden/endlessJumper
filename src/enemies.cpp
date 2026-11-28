#include "enemies.hpp"
#include <cmath>

Enemies::Enemies(){
    
    TraceLog(LOG_INFO, "Enemies constructor called");
    constexpr std::array<const char*, Enemies::ANIMATION_COUNT> animationPaths = {{
        "src/resources/Animations/flyEnemy.png",
        "src/resources/Animations/flyEnemy.png",
        "src/resources/Animations/flyEnemy.png"
    }};

    for (std::size_t i = 0; i < ANIMATION_COUNT; i++) {
        m_textures[i] = LoadTexture(animationPaths[i]);
        int frameCount = m_textures[i].width / 16.0f;
        m_animations[i] = AnimationClip{&m_textures[i], frameCount, 12.0f, true, false};
    }
    resetEnemies();
}

Enemies::~Enemies(){
     for (Texture2D& texture : m_textures) {
        if (texture.id != 0) UnloadTexture(texture);
    }
}

void Enemies::resetEnemies() {
    for(int i{}; i<Globals::enemyCount; i++){
        float x{static_cast<float>(GetRandomValue(170, 600))};
        float y{static_cast<float>(GetRandomValue(0, 540))};
        enemyArr[i].position.x = x;
        enemyArr[i].position.y = y;
        enemyArr[i].currentState = Enemy::State::IDLE;
        enemyArr[i].currentHealth = enemyArr[i].maxHealth;
        enemyArr[i].healthBarRect = {x-16.0f, y-32.0f, 32.0f, 10.0f};
        enemyArr[i].animator.play(m_animations[0]);
    }
}

void Enemies::chasePlayer(Vector2 &playerPosition){
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
        
         AnimationClip& clip = m_animations[static_cast<int>(enemy.currentState)];
        if (clip.texture && clip.texture->id != 0 && clip.frameCount > 0) {
            float frameWidth = static_cast<float>(clip.texture->width) / clip.frameCount;
            float frameHeight = static_cast<float>(clip.texture->height);
            enemy.animator.draw({enemy.position.x - frameWidth * 0.5f, enemy.position.y - frameHeight * 0.5f});
        }

         drawHealthBars(enemy);
         //DrawCircleLines(enemy.position.x, enemy.position.y, enemy.radius, RED); // hitbox
         //DrawCircleLines(enemy.position.x, enemy.position.y, enemy.attackRadius, BLUE); // attack radius
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

void Enemies::updateAnimationState() {
    for(Enemy& enemy : enemyArr){
        if (enemy.currentState == Enemy::State::DEAD) continue;
        Enemy::State next = Enemy::State::CHASE;
        //set states here s
        setAnimationState(next, enemy);
        enemy.animator.update(GetFrameTime());
    }
}

void Enemies::setAnimationState(Enemy::State newState, Enemy& enemy) {
    if (newState == enemy.currentState) return;
    enemy.currentState = newState;
    enemy.animator.play(m_animations[static_cast<std::size_t>(newState)]);
}

void Enemies::update(Vector2 &playerPosition){
    chasePlayer(playerPosition);
    updateAnimationState(); 
}
