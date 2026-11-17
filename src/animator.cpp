
#include "animator.hpp"

/*--------PLAY----------------------*/
void Animator::play(AnimationClip& clip) {

    if (m_currentClip == &clip) return;

    m_currentClip = &clip;
    m_currentFrame = 0;
    m_runningTime = 0.0f;
    m_finished = false;
}

/*--------UPDATE-----------------*/
void Animator::update(float deltaTime) {

    if (!m_currentClip || m_finished) return;

    const AnimationClip& clip = *m_currentClip;

    if (clip.fps <= 0.0f || clip.frameCount <= 0) return;

    m_runningTime += deltaTime;

    const float frameTime = 1.0f / clip.fps;

    while (m_runningTime >= frameTime) {

        m_runningTime -= frameTime;

        if (clip.random) {
            m_currentFrame = GetRandomValue(0, clip.frameCount - 1);
            continue;
        }

        m_currentFrame++;

        if (m_currentFrame < clip.frameCount) continue;

        if (clip.loop) {
            m_currentFrame = 0;
            continue;
        }

        m_currentFrame = clip.frameCount - 1;
        m_finished = true;
        break;
    }
}

/*--------DRAW-------------------*/
void Animator::draw(Vector2 position, float rotation) {

    if (!m_currentClip || !m_currentClip->texture) return;

    const AnimationClip& clip = *m_currentClip;

    if (clip.frameCount <= 0 || clip.texture->id == 0) return;

    const float frameWidth = static_cast<float>(clip.texture->width) / clip.frameCount;
    const float frameHeight = static_cast<float>(clip.texture->height);

    Rectangle source = {m_currentFrame*frameWidth, 0.0f,frameWidth,  frameHeight};

    Rectangle destination = {position.x, position.y, frameWidth, frameHeight};

    DrawTexturePro(*clip.texture, source, destination, {0.0f, 0.0f}, rotation, WHITE);
}
