#pragma once
#include "animation.h"
#include "globals.h"

class Player : public Animation {

    public:

        Player();

        // sprite
        void drawSprite() override;
        void updateSprite() override;
        void drawHitbox() override;
        void setState(int newState);

        void handleKeyboardMovement();
        void handleControllerMovement();
        void handleLogic();

        // getters
        float getPlayerSpeed();

        // states
        enum {IDLE, LEFT, RIGHT, JUMP, SPIN};

    private:
    
        // player variables
        float m_playerSpeed{220.0f}; 
        bool m_inAir{true};
        float m_jumpVelocity{0.0f};
        float m_gravity{1500.0f};
        bool m_onPlatform{false};
        bool m_gameOver{false};
    
        // frame variables
        bool m_idle{true}; 
        int m_state{0}; 

        // texture stuff
        Texture2D* m_currentTexture{nullptr};  // pointer to current texture
        int8_t m_currentState{0};// track current state of textures 

        // animation & sound stuff
        std::array<Texture2D, 7> m_animationTextures;

};