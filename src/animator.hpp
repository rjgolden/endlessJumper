#pragma once
#include <raylib.h>

struct AnimationClip {
    Texture2D* texture = nullptr;
    int frameCount{1};
    float fps{12.0f};
    bool loop{true};
    bool random{false};
};

class Animator {

    public:
       
        void play(AnimationClip& clip);
        void update(float deltaTime);
        void draw(Vector2 position, float rotation = 0.0f);
        bool isFinished() { return m_finished; }

    private:
       
        AnimationClip* m_currentClip{nullptr};
        int m_currentFrame{0};
        float m_runningTime{0.0f};
        bool m_finished{false};
};