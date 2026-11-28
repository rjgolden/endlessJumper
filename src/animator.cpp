
#include "animator.hpp"

/*--------PLAY----------------------*/
void Animator::play(AnimationClip& clip) {

    if (currentClip == &clip) return;

    currentClip = &clip;
    currentFrame = 0;
    runningTime = 0.0f;
    finished = false;
}

/*--------UPDATE-----------------*/
void Animator::update(float deltaTime) {

    if (!currentClip || finished) return;

    const AnimationClip& clip = *currentClip;

    if (clip.fps <= 0.0f || clip.frameCount <= 0) return;

    runningTime += deltaTime;

    const float frameTime = 1.0f / clip.fps;

    while (runningTime >= frameTime) {

        runningTime -= frameTime;
        currentFrame++;

        if (clip.random) {currentFrame = GetRandomValue(0, clip.frameCount - 1); continue;}

        if (currentFrame < clip.frameCount) continue;

        if (clip.loop) {currentFrame = 0; continue;}

        currentFrame = clip.frameCount - 1;
        finished = true;
        break;
    }
}

/*--------DRAW-------------------*/
void Animator::draw(Vector2 position, float rotation) {

    if (!currentClip || !currentClip->texture) return;

    const AnimationClip& clip = *currentClip;

    if (clip.frameCount <= 0 || clip.texture->id == 0) return;

    const float frameWidth = static_cast<float>(clip.texture->width) / clip.frameCount;
    const float frameHeight = static_cast<float>(clip.texture->height);

    Rectangle source = {currentFrame*frameWidth, 0.0f,frameWidth,  frameHeight};

    Rectangle destination = {position.x, position.y, frameWidth, frameHeight};

    DrawTexturePro(*clip.texture, source, destination, {0.0f, 0.0f}, rotation, WHITE);
}
