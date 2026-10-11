#pragma once

#include <raylib.h>
#include <array>
#include <cstdint>
#include <string>

class Animation {

    public: 

        Animation(Texture2D texture, Vector2 position, int frameCount, int width, int height, bool random);
        Animation();
        ~Animation();
        
        // sprite
        virtual void drawSprite();
        virtual void updateSprite();
        virtual void drawHitbox();
        void animateSprite();
        void animateSpriteRandom();
    
    protected:

        Texture2D m_texture; // textures for the animation 
        Rectangle m_animationRect{0.0f}; // source rectangle for the animation
        Rectangle m_hitboxRect{0.0f}; // hitbox for the animation
        uint8_t m_frameCount{0}; 
        uint8_t m_currentFrame{0}; // current frame of the animation
        bool m_random{false}; // whether or not to play frames randomly
        float m_runningTime{0.0f};  // time accumulator for the animation
        float m_animationTime{12.0f}; // time animtion plays for - default is 12.0f (1/12) or 83.33ms
        float m_updateTime{1.0f/m_animationTime}; // time between frame updates

        float m_rotation{0.0f};
        Vector2 m_position{};
        Vector2 m_offset{};

};