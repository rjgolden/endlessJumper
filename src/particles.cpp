#include <cmath>
#include "particles.hpp"
#include "globals.hpp"
#include "floor.hpp"

Particles::Particles(){
    particleTextures[0] = (LoadTexture("src/resources/goldNugget.png"));
    //particleTextures[1] = (LoadTexture("src/resources/blood2.png"));
    //particleTextures[2] = (LoadTexture("src/resources/blood3.png"));
}

Particles::~Particles(){
    UnloadTexture(particleTextures[0]);
    //UnloadTexture(particleTextures[1]);
    //UnloadTexture(particleTextures[2]);
}

void Particles::explodeNuggets(Vector2 pos, Color tint, int textureIndex) {
    for (int i{0}; i < 10; i++) {
        for (Particle& particle : particlesArr) {
            if (particle.life <= 0.0f) {
                particle = {pos, {static_cast<float>(GetRandomValue(-100,100)), static_cast<float>(GetRandomValue(-100,100))}, tint, 1.0f, static_cast<float>(GetRandomValue(0, 360)), static_cast<float>(GetRandomValue(1,2)), textureIndex};
                break;
            }
        }
    }
}

void Particles::resetParticles(){
    for(Particle& particle : particlesArr) {
        particle.life = 0.0f;
    }
}

void Particles::drawParticles(Floor& floor) {
    float deltaTime = GetFrameTime();
    constexpr float gravity = 300.0f;
    for (Particle& particle : particlesArr) {
        if (particle.life > 0.0f) {

            Texture2D& texture = particleTextures[particle.textureIndex];
            float halfWidth = texture.width/2;
            float halfHeight = texture.height/2;
            particle.vel.y += gravity * deltaTime;
            particle.pos.x += particle.vel.x * deltaTime;
            particle.pos.y += particle.vel.y * deltaTime;
            particle.rotation += particle.vel.x * deltaTime;

            if(particle.vel.x > 0.0f) particle.vel.x -= 0.5f;
            if(particle.vel.x < 0.0f) particle.vel.x += 0.5f;
    
            if(particle.pos.y >= Globals::floorY) {
                particle.pos.y = Globals::floorY; 
                //particle.life -= deltaTime / 5.0f;
                //particle.tint.a = std::max(0.0f, particle.tint.a - 51.0f * deltaTime);
            }

            if (particle.pos.x - halfWidth <= Globals::leftEdge)
            {
                particle.pos.x = Globals::leftEdge +  halfWidth;
                particle.vel.x = 0.0f;
            }

            if (particle.pos.x + halfWidth >= Globals::rightEdge)
            {
                particle.pos.x = Globals::rightEdge - halfWidth;
                particle.vel.x = 0.0f;

            }
            
            for (Platform& platform : floor.platforms.platformArr) {

                if(platform.falsePlatform) continue;

                float platformTop = platform.rect.y;
                float playerBottom = particle.pos.y + halfHeight;

                bool horizontalOverlap = particle.pos.x + halfWidth > platform.rect.x && particle.pos.x - halfWidth < platform.rect.x + platform.rect.width;
                bool nearPlatform = std::abs(playerBottom - platformTop) < 5.0f;

                if (horizontalOverlap && nearPlatform && particle.vel.y >= 0.0f) {   
                    particle.pos.y = platformTop - halfHeight+2.0f;
                    particle.vel.y = 0.0f;
                    platform.goldNuggetsOnPlatform = true;
                    //particle.life -= deltaTime / 5.0f;
                    //particle.tint.a = std::max(0.0f, particle.tint.a - 51.0f * deltaTime);
                }
                else platform.goldNuggetsOnPlatform = false;
                
            }

            /*   for (Particle& particle2 : particlesArr) {
                    if (particle.life <= 0.0f) continue;
                    bool check = CheckCollisionCircles(particle.pos, particle.radius*particle.scale, particle2.pos, particle2.radius*particle2.scale);
                    if(check){
                        float dx = particle2.pos.x - particle.pos.x;
                        float dy = particle2.pos.y - particle.pos.y;

                        float distance = sqrtf(dx * dx + dy * dy);
                        float minDistance = particle.radius + particle2.radius;

                        if (distance < minDistance && distance > 0.0f) {
                            float overlap = minDistance - distance;
                            float nx = dx / distance;
                            float ny = dy / distance;
                            float pushStrength = 0.2f;

                            particle.pos.x -= nx * overlap * pushStrength;
                            particle.pos.y -= ny * overlap * pushStrength;
                            particle2.pos.x += nx * overlap * pushStrength;
                            particle2.pos.y += ny * overlap * pushStrength;
                        }
                    }
                }   */

            particle.source = {0.0f, 0.0f, static_cast<float>(texture.width), static_cast<float>(texture.height)};
            particle.dest = {particle.pos.x, particle.pos.y, texture.width * particle.scale, texture.height * particle.scale};
            particle.origin = {texture.width * particle.scale * 0.5f, texture.height * particle.scale * 0.5f};
            DrawTexturePro(texture, particle.source, particle.dest, particle.origin, particle.rotation, particle.tint);
            //DrawCircleLines(particle.dest.x, particle.dest.y, particle.radius*particle.scale, YELLOW);

        }
    }
}