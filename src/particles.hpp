#pragma once
#include "raylib.h"
#include <array>

class Floor;
class Platform;
class Platforms;

struct Particle { 
    Vector2 pos{}; 
    Vector2 vel{}; 
    Color tint{};
    float life{}; 
    float rotation{}; 
    float scale{}; 
    int textureIndex{}; 
    Rectangle source{};
    Rectangle dest{};
    Vector2 origin{};
    float radius{16.0f};
};

class Particles {

    public:

        Particles();
        ~Particles();

        void drawParticles(Floor& floor);
        void resetParticles();
        void explodeNuggets(Vector2 pos, Color tint, int textureIndex);

        std::array<Texture2D, 1> particleTextures;
        std::array<Particle, 1000> particlesArr;

};