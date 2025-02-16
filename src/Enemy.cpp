#include "Enemy.hpp"

Enemy::Enemy(Texture_Manager &manager, const std::string &texture_name, int x, int y,
            GameObject* playerTarget, int health)
    : GameObject(manager, texture_name, x, y,64, 64,64, 64,1,0.0f,health),target(playerTarget) //lahko so animacije
{
    isAnimated = false; // Za animacije
    currentFrame = {0, 0, 64, 64}; // Cela textura
}
void Enemy::Update(float deltaTime) {
    if (!IsActive() || !IsAlive()) return; // Enemyji imajo taprave statuse
    if (!target) return; // Ali player obstaja

    SDL_Rect targetPos = target->GetPosition();

    // Najde smer
    float dx = targetPos.x - positionrect.x;
    float dy = targetPos.y - positionrect.y;
    float distance = sqrt(dx*dx + dy*dy);

    // Normaliziran
    if (distance > 0) {
        velocityX = (dx / distance) * moveSpeed;
        velocityY = (dy / distance) * moveSpeed;
    }

    // Debug:
    SDL_Log("Enemy velocity: X=%.1f, Y=%.1f | Target: %d,%d",
           velocityX, velocityY, targetPos.x, targetPos.y);

    // Premik
    GameObject::Update(deltaTime);
}
// revivam iz poola
void Enemy::Revive(int x, int y, int level) {
    this->level = level;
    SetPosition(x, y);
    isActive = true;

    switch (level) {
        case 1:
            maxHealth = 50;
        moveSpeed = 200.0f;
        break;
        case 2:
            maxHealth = 99;
        moveSpeed = 120.0f;
        break;
        case 3:
            maxHealth = 199;
        moveSpeed = 90.0f;
        break;
        default:
            maxHealth = 50;
        moveSpeed = 100.0f;
    }
    health = maxHealth;

    std::string textureID = "enemy" + std::to_string(level);
    texture = manager.GetTexture(textureID);
}
void Enemy::Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) {
    SDL_Rect destRect = {
        GetPosition().x - cameraViewport.x,
        GetPosition().y - cameraViewport.y,
        GetPosition().w,
        GetPosition().h
    };

    // Ali je textura
    if (texture && IsActive()) {
        SDL_RenderCopyEx(
            renderer, texture,
            &currentFrame, &destRect,
            0.0, nullptr, flipType
        );
    }
}
void Enemy::SetTarget(GameObject* newTarget) {
    target = newTarget;
}