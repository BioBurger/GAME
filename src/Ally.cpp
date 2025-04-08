#include "Ally.hpp"

Ally::Ally(Texture_Manager& tm, const std::string& textureID, int x, int y)
    : GameObject(tm, textureID, x, y, 64, 64, 32, 32, 1, 0.1f) {}

GameObject::Direction Ally::DetermineDirection() {
    if (abs(velocityX) > abs(velocityY)) {
        return (velocityX > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    return (velocityY > 0) ? Direction::DOWN : Direction::UP;
}
void Ally::Revive(int x, int y, int health) {
    SetPosition(x, y);
    SetHealth(health);
    SetActive(true);
    currentState = State::IDLE;
    animationTimer = 0.0f;
    currentFrameIndex = 0;
}
void Ally::ResetAnimation() {
    currentState = State::IDLE;
    animationTimer = 0.0f;
    currentFrameIndex = 0;
}

void Ally::Update(float deltaTime) {
    // Prejšni state
    static State previousState = currentState;
    static Direction lastDirection = Direction::DOWN; // Default

    // Update current state
    currentState = (velocityX != 0 || velocityY != 0) ? State::MOVING : State::IDLE;

    // Update direction
    if (currentState == State::MOVING) {
        lastDirection = DetermineDirection();
    }

    // Reset animation when state different
    if (currentState != previousState) {
        currentFrameIndex = 0;
        animationTimer = 0.0f;
        previousState = currentState;
    }

    // Set animation parameters
    if (currentState == State::IDLE) {
        totalFrames = 4;
        animationSpeed = 0.15f;
    } else {
        totalFrames = 6;
        animationSpeed = 0.1f;
    }

    // Animation row based on direction
    switch (lastDirection) {
        case Direction::DOWN:
            animationRow = (currentState == State::IDLE) ? 0 : 3;
            flipType = SDL_FLIP_NONE;
            break;
        case Direction::RIGHT:
            animationRow = (currentState == State::IDLE) ? 1 : 4;
            flipType = SDL_FLIP_NONE;
            break;
        case Direction::LEFT:
            animationRow = (currentState == State::IDLE) ? 1 : 4;
            flipType = SDL_FLIP_HORIZONTAL;
            break;
        case Direction::UP:
            animationRow = (currentState == State::IDLE) ? 2 : 5;
            flipType = SDL_FLIP_NONE;
            break;
    }

    // Update animation frames
    animationTimer += deltaTime;
    if (animationTimer >= animationSpeed) {
        currentFrameIndex = (currentFrameIndex + 1) % totalFrames;
        animationTimer = 0.0f;
    }

    // Update frame coordinates
    currentFrame.x = currentFrameIndex * frameWidth;
    currentFrame.y = animationRow * frameHeight;

    this->totalFrames = (currentState == State::IDLE) ? 4 : 6;
    this->animationSpeed = (currentState == State::IDLE) ? 0.15f : 0.1f;

    positionX += velocityX * deltaTime * speedMultiplier;
    positionY += velocityY * deltaTime * speedMultiplier;
    positionrect.x = static_cast<int>(positionX);
    positionrect.y = static_cast<int>(positionY);
}
