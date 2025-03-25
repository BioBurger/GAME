#include "Ally.hpp"

Ally::Ally(Texture_Manager& manager, const std::string& textureID, int x, int y)
    : GameObject(manager, textureID, x, y, 64, 64, 32, 32, 1, 0.1f) {
    frameWidth = 32;
    frameHeight = 32;
    isAnimated = true;
}

GameObject::Direction Ally::DetermineDirection() {
    if (abs(velocityX) > abs(velocityY)) {
        return (velocityX > 0) ? Direction::RIGHT : Direction::LEFT;
    }
    return (velocityY > 0) ? Direction::DOWN : Direction::UP;
}

void Ally::Update(float deltaTime) {
    // Update state
    currentState = (velocityX != 0 || velocityY != 0) ? State::MOVING : State::IDLE;

    // Animation
    Direction dir = DetermineDirection();
    switch(currentState) {
        case State::IDLE:
            totalFrames = 4;
        animationSpeed = 0.2f;
        switch(dir) {
            case Direction::DOWN: animationRow = 0; break;
            case Direction::RIGHT:
            case Direction::LEFT:
                animationRow = 1;
            flipType = (dir == Direction::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            break;
            case Direction::UP: animationRow = 2; break;
        }
        break;

        case State::MOVING:
            totalFrames = 6;
        animationSpeed = 0.1f;
        switch(dir) {
            case Direction::DOWN: animationRow = 3; break;
            case Direction::RIGHT:
            case Direction::LEFT:
                animationRow = 4;
            flipType = (dir == Direction::LEFT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            break;
            case Direction::UP: animationRow = 5; break;
        }
        break;
    }

    GameObject::Update(deltaTime);
}
