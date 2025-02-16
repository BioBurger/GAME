#pragma once
#include "GameObject.hpp"
#include "GameObject.hpp"

class Enemy : public GameObject {
private:
    GameObject* target;//na playerja
    float moveSpeed = 100.0f;
    bool isActive = false;
    int level;
public:
    Enemy(Texture_Manager& manager, const std::string& texture_name, int x, int y, GameObject* playerTarget, int health = 50);
    void Update(float deltaTime) override;
    void Revive(int x, int y, int level);
    void SetSpeed(float speed) { moveSpeed = speed; }
    float GetSpeed() const { return moveSpeed; }
    bool IsActive() const { return isActive; }
    void Render(SDL_Renderer* renderer, const SDL_Rect& cameraViewport) override;
    void SetTarget(GameObject* newTarget);
    int GetLevel() const {return level;}
};