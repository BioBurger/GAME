#pragma once
#include "GameObject.hpp"
#include "GameObject.hpp"

class Enemy : public GameObject {
private:
    GameObject* target;//na playerja
    float moveSpeed = 100.0f;
public:
    Enemy(Texture_Manager& manager, const std::string& texture_name, int x, int y, GameObject* playerTarget, int health = 50);
    void Update(float deltaTime) override;
};