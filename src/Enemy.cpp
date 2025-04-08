#include "Enemy.hpp"

// Enemy.cpp constructor
Enemy::Enemy(Texture_Manager& manager,
             const std::string& textureName,
             int x, int y,
             int width, int height,
             int collisionWidth, int collisionHeight,
             int renderOrder,
             float speed,
             GameObject* target,
             int health)
    : GameObject(manager, textureName,
                 x, y,
                 128, 128,  // Render veči kot je
                 32, 32,    // Originalna velikost
                 5,         // Total frames
                 0.15f,     // Animation hitrost
                 health),
      target(target)
{
    useVerticalAnimation = false;

    // Centered collision box
    collisionOffsetX = (128 - 40) / 2;
    collisionOffsetY = (128 - 40) / 2;
    this->collisionWidth = 40;
    this->collisionHeight = 40;

    moveSpeed = speed;
    isActive = true;
}

void Enemy::Update(float deltaTime) {
    if (!IsActive() || !IsAlive()) return;
    if (!target) return;

    // Calculate movement
    Vector2f targetCenter = target->GetCenterPosition();
    Vector2f ourCenter = GetCenterPosition();

    float dx = targetCenter.x - ourCenter.x;
    float dy = targetCenter.y - ourCenter.y;
    float distance = sqrt(dx*dx + dy*dy);

    // Normalize velocity
    if (distance > 0) {
        velocityX = (dx / distance) * moveSpeed;
        velocityY = (dy / distance) * moveSpeed;
    }

    // Update base class
    GameObject::Update(deltaTime);

    // Force horizontal animation
    currentFrame.y = 0;  // Lock to first row

    // Ensure proper frame wrapping for 5-frame animation
    currentFrameIndex = currentFrameIndex % 5;
}

void Enemy::Revive(int x, int y, int level) {
    this->level = level;
    SetPosition(x, y);
    isActive = true;

    // Configure enemy stats
    switch(level) {
        case 1:
            maxHealth = 50;
            moveSpeed = 200.0f;
            break;
        case 2:
            maxHealth = 99;
            moveSpeed = 120.0f;
            break;
        case 3:
            maxHealth = 151;
            moveSpeed = 90.0f;
            break;
    }

    SetHealth(maxHealth);
    std::string textureID = "enemy" + std::to_string(level);
    ReloadTexture(manager, textureID);

    // Reset animation state
    currentFrameIndex = 0;
    animationTimer = 0.0f;
    currentFrame = {0, 0, 32, 32};  // Source frame
}

void Enemy::Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) {
    if (!texture || !IsActive()) return;

    // Scaled-up render dimensions
    SDL_Rect destRect = {
        GetPosition().x - cameraViewport.x,
        GetPosition().y - cameraViewport.y,
        128,
        128
    };

    // Source frame from texture (32x32)
    SDL_Rect sourceRect = {
        currentFrameIndex * 32,
        0,
        32,
        32
    };

    SDL_RenderCopyEx(renderer, texture, &sourceRect, &destRect,
                    0.0, nullptr, flipType);
}


void Enemy::SetTarget(GameObject* newTarget) {
    target = newTarget;
}

void Enemy::ReloadTexture(Texture_Manager& tm, const std::string& baseTexture) {
    std::string texName = "enemy" + std::to_string(level);
    GameObject::ReloadTexture(tm, texName);

    // Verify texture dimensions
    if (texture) {
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Log("Enemy %d texture: %dx%d", level, w, h);
    }
}