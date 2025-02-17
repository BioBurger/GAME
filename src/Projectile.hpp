#pragma once
#include "GameObject.hpp"
#include "Vector2f.hpp"

class Projectile : public GameObject {
private:
    GameObject* target;
    float speed;
    int damage;
    float maxDistance;
    Vector2f startPosition;
public:
    Projectile(Texture_Manager& manager, GameObject* target, int x, int y, float speed, int damage, float maxDistance);
    void Update(float deltaTime) override;
    bool ShouldRemove() const;
    GameObject* GetTarget() const {return target;};
    void ReloadTexture(Texture_Manager& tm, const std::string& textureID);
};