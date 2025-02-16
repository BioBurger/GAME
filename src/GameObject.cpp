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
        currentState = State::DEAD;
        PlayDeathAnimation();
        return;
    }
    // Posodobi pozicijo
    positionX += velocityX * deltaTime * speedMultiplier;
    positionY += velocityY * deltaTime * speedMultiplier;

    // Posodobi SDL_Rect za renderiranje
    positionrect.x = static_cast<int>(positionX);
    positionrect.y = static_cast<int>(positionY);

    // Določi stanje in smer glede na hitrost
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

    // Nastavi animacijsko vrstico in število frame-ov
    switch (currentState) {
        case State::IDLE:
            if (currentDirection == Direction::LEFT) {
                animationRow = 1;  // Uporabi idle right + flip
                flipType = SDL_FLIP_HORIZONTAL;
            } else {
                animationRow = static_cast<int>(currentDirection);
                flipType = SDL_FLIP_NONE;
            }
        totalFrames = 5; // Število frame-ov za idle
        break;

        case State::MOVING:
            if (currentDirection == Direction::DOWN) {
                animationRow = 3;  // Premik dol je v vrstici 3
            } else if (currentDirection == Direction::UP) {
                animationRow = 5;  // Premik gor je v vrstici 5
            } else if (currentDirection == Direction::RIGHT) {
                animationRow = 4;  // Premik desno je v vrstici 4
            } else {  // LEFT
                animationRow = 4; // Uporabi premik desno + flip
                flipType = SDL_FLIP_HORIZONTAL;
            }
        totalFrames = 8; // Število frame-ov za premikanje
        break;
    }

    // Animacija
    float actualSpeed = ( currentState == State::IDLE) ? idleAnimationSpeed : moveAnimationSpeed;
    if (isAnimated) {
        animationTimer += deltaTime;
        if (animationTimer >= actualSpeed) {
            currentFrameIndex = (currentFrameIndex + 1) % totalFrames;  // Premik na naslednji frame
            currentFrame.x = currentFrameIndex * frameWidth;  // Spreminjanje X glede na frame index
            currentFrame.y = animationRow * frameHeight;  // Spreminjanje Y glede na vrstico animacije

            animationTimer = 0.0f;  // Resetiraj timer
        }
    }
}

void GameObject::Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) {
    SDL_Rect destRect = {
        positionrect.x - cameraViewport.x, // Korekcija za kamero
        positionrect.y - cameraViewport.y,
        positionrect.w,
        positionrect.h
    };
    //Za death animacijo
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
void GameObject::GetHealth(int healAmount) {
    health = std::min(maxHealth, health + healAmount);
}
void GameObject::PlayDeathAnimation() {
    isAnimated = true;
    totalFrames = 1;
    animationRow = 0;
    animationSpeed = 0.15f;
    currentFrameIndex = 0;
    velocityX = 0;
    velocityY = 0;
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
    return position;
}