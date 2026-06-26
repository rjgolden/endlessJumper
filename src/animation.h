#pragma once

#include <raylib.h>
#include <array>
#include <cstdint>
#include <string>

class Animation {

    public: 

        Animation(Texture2D texture, uint8_t frameCount, float positionX, float positionY, bool random);
        Animation();
        ~Animation();
        
        // sprite
        virtual void drawSprite();
        virtual void updateSprite();
        virtual void drawHitbox();
        void drawAnimationBoxLines();
        void animateSprite();
        void animateSpriteRandom();
    
        // getters
        float getPositionX();
        float getPositionY();
        float getWidth();
        float getHeight();
        Vector2 getPosition();
        Vector2 getDefaultDimensions();
        Rectangle getHitboxRect();

        // setters
        void setPosition(Vector2 position);
        void setRotation(float rotation);
        void setHitboxDimensions(Vector2 dimensions);
        void setHitboxOffset(Vector2 pos);
        
    protected:

        Texture2D m_texture; // textures for the animation 
        Rectangle m_animationRect{0.0f}; // source rectangle for the animation
        Rectangle m_hitboxRect{0.0f}; // hitbox for the animation
        uint8_t m_frameCount{0}; 
        uint8_t m_currentFrame{0}; // current frame of the animation
        bool m_random{false}; // whether or not to play frames randomly
        float m_defaultWidth{0.0f};
        float m_defaultHeight{0.0f};
        float m_runningTime{0.0f};  // time accumulator for the animation
        float m_animationTime{12.0f}; // time animtion plays for - default is 12.0f (1/12) or 83.33ms
        float m_updateTime{1.0f/m_animationTime}; // time between frame updates
        float m_positionX{0.0f}; 
        float m_positionY{0.0f}; 
        float m_rotation{0.0f};
        float m_offsetX{0.0f};
        float m_offsetY{0.0f};

};