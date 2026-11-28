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
        bool isFinished() { return finished; }

    private:
       
        AnimationClip* currentClip{nullptr};
        int currentFrame{0};
        float runningTime{0.0f};
        bool finished{false};
};