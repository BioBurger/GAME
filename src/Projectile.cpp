#include "Projectile.hpp"
#include <cmath>

Projectile::Projectile(Texture_Manager &manager, GameObject *target, int x, int y, float speed, int damage, float maxDistance)
: GameObject(manager, "bullet", x, y, 16, 16, 16, 16, 1, 0.0f),
target(target),
speed(speed),
damage(damage),
maxDistance(maxDistance),
startPosition(x, y){}

void Projectile::Update(float deltaTime) {
    if (!target || !target->IsAlive()) return;  // Ali je živ

    float dx = target->GetPosition().x - positionrect.x;
    float dy = target->GetPosition().y - positionrect.y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        velocityX = (dx / distance) * speed;
        velocityY = (dy / distance) * speed;
    }

    GameObject::Update(deltaTime);
}


bool Projectile::ShouldRemove() const {
    // Dolžina premika
    float traveled = sqrt(pow(positionrect.x - positionrect.x, 2) + pow(positionrect.y - positionrect.y, 2));
    return traveled > maxDistance || !target->IsAlive();
}
void Projectile::ReloadTexture(Texture_Manager& tm, const std::string& textureID) {
    texture = tm.GetTexture(textureID);
    if (!texture) {
        SDL_Log("Failed to reload projectile texture");
    }
}

