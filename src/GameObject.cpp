#include "GameObject.hpp"

#include <bits/stl_algo.h>

GameObject::GameObject(Texture_Manager& manager, const std::string& texture_name , int x, int y, int w, int h, int frameWidth, int frameHeight, int totalFrames, float animationSpeed, int startHealth)
    : manager(manager),
        texture(manager.GetTexture(texture_name)),
        positionrect{x,y,w,h},velocityX(0),velocityY(0),positionX(static_cast<float>(x)),positionY(static_cast<float>(y)),
        frameWidth(frameWidth), frameHeight(frameHeight),
        currentFrameIndex(0), totalFrames(totalFrames),
        animationSpeed(animationSpeed), animationTimer(0.0f),
        isAnimated(totalFrames > 1),
        idleAnimationSpeed(animationSpeed * 2.0f),
        moveAnimationSpeed(animationSpeed),
        health(startHealth),
        maxHealth(startHealth){
    texture = manager.GetTexture(texture_name);
    currentFrame = {0,0, frameWidth,frameHeight};
    if (!texture) {
        SDL_Log("TEXTURA '%s' NI NALOZENA", texture_name.c_str());
    }
    currentState = State::IDLE;
    currentDirection = Direction::DOWN;
    flipType = SDL_FLIP_NONE;
    animationRow = 0;

}

GameObject::~GameObject() {}
void GameObject::Update(float deltaTime) {
    if (!IsAlive()) {
        return;
    }
    // Update location
    positionX += velocityX * deltaTime * speedMultiplier;
    positionY += velocityY * deltaTime * speedMultiplier;

    positionrect.x = static_cast<int>(positionX);
    positionrect.y = static_cast<int>(positionY);

    // State and speed
    if (velocityX != 0 || velocityY != 0) {
        currentState = State::MOVING;

        if (abs(velocityX) > abs(velocityY)) {
            currentDirection = (velocityX > 0) ? Direction::RIGHT : Direction::LEFT;
        } else {
            currentDirection = (velocityY > 0) ? Direction::DOWN : Direction::UP;
        }
    } else {
        currentState = State::IDLE;
    }

    switch (currentState) {
        case State::IDLE:
            if (currentDirection == Direction::LEFT) {
                animationRow = 1;
                flipType = SDL_FLIP_HORIZONTAL;
            } else {
                animationRow = static_cast<int>(currentDirection);
                flipType = SDL_FLIP_NONE;
            }
        totalFrames = 5;
        break;

        case State::MOVING:
            if (currentDirection == Direction::DOWN) {
                animationRow = 3;
            } else if (currentDirection == Direction::UP) {
                animationRow = 5;
            } else if (currentDirection == Direction::RIGHT) {
                animationRow = 4;
            } else {
                animationRow = 4;
                flipType = SDL_FLIP_HORIZONTAL;
            }
        totalFrames = 8;
        break;
    }

    // Animacija
    float actualSpeed = ( currentState == State::IDLE) ? idleAnimationSpeed : moveAnimationSpeed;
    if (isAnimated) {
        animationTimer += deltaTime;
        if (animationTimer >= actualSpeed) {
            currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
            currentFrame.x = currentFrameIndex * frameWidth;
            currentFrame.y = animationRow * frameHeight;

            animationTimer = 0.0f;
        }
    }
}

void GameObject::Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) {
    SDL_Rect destRect = {
        positionrect.x - cameraViewport.x,
        positionrect.y - cameraViewport.y,
        positionrect.w,
        positionrect.h
    };
    if (!IsAlive()) {
        currentFrame.y = animationRow * frameHeight;
    }
    SDL_RenderCopyEx(renderer, texture, &currentFrame, &destRect, 0.0, nullptr, flipType);
}
void GameObject::SetPosition(int x, int y) {
    positionrect.x = x;
    positionrect.y = y;
    positionX = static_cast<float>(x);
    positionY = static_cast<float>(y);
}
void GameObject::SetVelocity(float vx, float vy) {
    velocityX = vx;
    velocityY = vy;
}
void GameObject::SetDirection(Direction newDir) {
    if (currentDirection != newDir) {
        currentDirection = newDir;
        flipType = (newDir == Direction::LEFT) ?  SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    }
}
GameObject::Direction GameObject::GetDirection()const {
    return currentDirection;
}
void GameObject::TakeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}
SDL_Rect GameObject::GetCollisionBox() const {
    return {
        positionrect.x + collisionOffsetX,
        positionrect.y + collisionOffsetY,
        collisionWidth,
        collisionHeight
    };
}
Vector2f GameObject::GetCenterPosition() const {
    return Vector2f(positionrect.x + positionrect.w / 2.0f,
                positionrect.y + positionrect.h / 2.0f);
}
void GameObject::ReloadTexture(Texture_Manager& tm, const std::string& textureID) {
    texture = tm.GetTexture(textureID);
    if (!texture) {
        SDL_Log("Failed to reload texture: %s", textureID.c_str());
    }
}