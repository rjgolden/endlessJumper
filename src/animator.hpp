#pragma once
#include <raylib.h>

struct AnimationClip {
    const Texture2D* texture = nullptr;
    int frameCount = 1;
    float fps = 12.0f;
    bool loop = true;
    bool random = false;
};

class Animator {
public:
    void play(const AnimationClip& clip);
    void update(float deltaTime);
    void draw(Vector2 position, float rotation = 0.0f) const;

    bool isFinished() const { return m_finished; }

private:
    const AnimationClip* m_currentClip = nullptr;

    int m_currentFrame = 0;
    float m_runningTime = 0.0f;
    bool m_finished = false;
};