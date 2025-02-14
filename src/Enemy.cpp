#include "Enemy.h"

Enemy::Enemy(Texture_Manager &manager, const std::string &texture_name, int x, int y, GameObject* playerTarget, int health) : GameObject(manager, texture_name, x, y, 64, 64, 16, 16, 1, 0.2f, health), target(playerTarget){}

void Enemy::Update(float deltaTime) {
    if (!target || !IsAlive()) return;

    //pot do playerja
    SDL_Rect targetPos = target->GetPosition();
    float dx = targetPos.x - positionrect.x;
    float dy = targetPos.y - positionrect.y;

    //normaliziran vektor
    float lenght = sqrt(dx*dx+dy*dy);
    if (lenght > 0 ) {
        velocityX = (dx / lenght) * moveSpeed;
        velocityY = (dy / lenght) * moveSpeed;
    }

    GameObject::Update(deltaTime);
}